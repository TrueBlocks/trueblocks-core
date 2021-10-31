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

	fmt.Fprintf(os.Stderr, "TIME ~ CLOCK - <INFO>  : %s\n", name)
	for _, arg := range args {
		fmt.Fprintf(os.Stderr, "TIME ~ CLOCK - <INFO>  :   %s\n", arg)
	}
}

func TestLogBool(name string, val bool) {
	if !val || !IsTestMode() {
		return
	}

	fmt.Fprintf(os.Stderr, "TIME ~ CLOCK - <INFO>  : %s: %t\n", name, val)
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
