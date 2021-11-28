// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package utils

import (
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"os/exec"
	"os/user"
	"runtime"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"golang.org/x/crypto/ssh/terminal"
)

// GetCommandPath returns full path the the given tool
func GetCommandPath(cmd string) string {
	usr, _ := user.Current()
	dir := usr.HomeDir
	return dir + "/.local/bin/chifra/" + cmd
}

// IsTestModeServer return true if we are running from the testing harness
func IsTestModeServer(r *http.Request) bool {
	return r.Header.Get("User-Agent") == "testRunner"
}

func IsApiMode() bool {
	return os.Getenv("API_MODE") == "true"
}

func IsTerminal() bool {
	return terminal.IsTerminal(int(os.Stdout.Fd()))
}

func AsciiFileToString(fn string) string {
	if !file.FileExists(fn) {
		return ""
	}

	contents, err := ioutil.ReadFile(fn)
	if err != nil {
		log.Println(err)
		return ""
	}
	return string(contents)
}

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

func ToCamelCase(in string) string {
	if len(in) == 0 {
		return in
	}

	var arr []string
	fields := strings.Fields(in)
	for _, field := range fields {
		arr = append(arr, strings.Title(field))
	}
	arr[0] = strings.ToLower(arr[0])
	return strings.Join(arr, "")
}

// maximum uint64
const NOPOS = ^uint64(0)
