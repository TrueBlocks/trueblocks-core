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
package utils

import (
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"os/exec"
	"runtime"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func FileExists(filename string) bool {
	info, err := os.Stat(filename)
	if os.IsNotExist(err) {
		return false
	}
	return !info.IsDir()
}

// FolderExists help text todo
func FolderExists(path string) bool {
	info, err := os.Stat(path)
	if os.IsNotExist(err) {
		return false
	}
	return info.IsDir()
}

// IsTestModeServer return true if we are running from the testing harness
func IsTestModeServer(r *http.Request) bool {
	return r.Header.Get("User-Agent") == "testRunner"
}

func IsTestMode() bool {
	return os.Getenv("TEST_MODE") == "true"
}

func IsApiMode() bool {
	return os.Getenv("API_MODE") == "true"
}

func TestLogArgs(name string, args []string) {
	if !IsTestMode() || len(args) == 0 {
		return
	}

	logger.Log(logger.Info, name)
	for _, arg := range args {
		logger.Log(logger.Info, " ", arg)
	}
}

func TestLogBool(name string, val bool) {
	if !IsTestMode() || !val {
		return
	}

	logger.Log(logger.Info, name+":", val)
}

func AsciiFileToString(fn string) string {
	if !FileExists(fn) {
		return ""
	}

	contents, err := ioutil.ReadFile(fn)
	if err != nil {
		log.Println(err)
		return ""
	}
	return string(contents)
}

// maximum uint64
const NOPOS = ^uint64(0)

func OpenBrowser(url string) {
	var err error
	switch runtime.GOOS {
	case "linux":
		err = exec.Command("xdg-open", url).Start()
	case "windows":
		err = exec.Command("rundll32", "url.dll,FileProtocolHandler", url).Start()
	case "darwin":
		err = exec.Command("open", url).Start()
	default:
		err = fmt.Errorf("unsupported platform")
	}
	if err != nil {
		log.Fatal(err)
	}
}
