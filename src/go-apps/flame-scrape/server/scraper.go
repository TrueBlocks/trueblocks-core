package server

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os/user"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/utils"
)

type Scraper struct {
	Counter    uint64 `json:"Counter"`
	Running    bool   `json:"Running"`
	WasRunning bool   `json:""`
	SleepSecs  int64  `json:"SleepSecs"`
	Color      string `json:"Color"`
	Name       string `json:"Name"`
	Verbose    int64  `json:"Verbose"`
}

func NewScraper(color, name string, secs int, verbose int) Scraper {
	scraper := new(Scraper)
	scraper.Color = color
	scraper.Name = name
	scraper.SleepSecs = int64(secs)
	scraper.Running = scraper.LoadStateFromCache()
	scraper.Verbose = int64(verbose)
	return *scraper
}

func (scraper *Scraper) ShowStateChange(from, to string) {
	log.Print(scraper.Color, scraper.Name, ": [", from, " --> ", to, "]: ", scraper.Counter, utils.Off, "\n")
}

func (scraper *Scraper) ToJson() string {
	e, err := json.Marshal(scraper)
	if err != nil {
		fmt.Printf("%s", err)
		return ""
	}
	return string(e)
}

func (scraper *Scraper) ChangeState(onOff bool) bool {
	prev := scraper.Running
	scraper.Running = onOff
	str := "false"
	if onOff {
		str = "true"
	}
	fileName := Options.Status.CachePath + "tmp/" + scraper.Name + ".txt"
	err := ioutil.WriteFile(fileName, []byte(str), 0644)
	if err != nil {
		log.Fatal(err)
	}
	return prev
}

func (scraper *Scraper) LoadStateFromCache() bool {
	fileName := Options.Status.CachePath + "tmp/" + scraper.Name + ".txt"
	if !utils.FileExists(fileName) {
		return false
	}
	content, err := ioutil.ReadFile(fileName)
	if err != nil {
		log.Fatal(err)
	}
	return string(content) == "true"
}

func (scraper *Scraper) Pause() {
	halfSecs := scraper.SleepSecs * 2
	state := scraper.Running
	for i := 0; i < int(halfSecs); i++ {
		if state != scraper.Running {
			break
		}
		time.Sleep(time.Duration(500) * time.Millisecond)
	}
}

// isTestMode return true if we are running from the testing harness
func isTestMode(r *http.Request) bool {
	return r.Header.Get("User-Agent") == "testRunner"
}

// GetCommandPath returns full path the the given tool
func GetCommandPath(cmd string) string {
	usr, _ := user.Current()
	dir := usr.HomeDir
	return dir + "/.local/bin/chifra/" + cmd
}
