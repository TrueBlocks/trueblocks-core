/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

package server

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
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

func NewScraper(color, name string, secs int64) Scraper {
	scraper := new(Scraper)
	scraper.Color = color
	scraper.Name = name
	scraper.SleepSecs = secs
	scraper.Running = scraper.LoadStateFromCache()
	scraper.Verbose = 4
	return *scraper
}

func (scraper *Scraper) ShowStateChange(from, to string) {
	log.Print(scraper.Color, scraper.Name, ": [", from, " --> ", to, "]: ", scraper.Counter, utils.Off, "\n")
}

func (scraper *Scraper) ToJson() string {
	e, err := json.Marshal(scraper)
	if err != nil {
		fmt.Println(err)
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
