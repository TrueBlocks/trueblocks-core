package utils

import (
	"io/ioutil"
	"log"
	"os"
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

func IsTestMode() bool {
	return os.Getenv("TEST_MODE") == "true"
}

func IsApiMode() bool {
	return os.Getenv("API_MODE") == "true"
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
