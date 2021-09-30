package utils

import "os"

func FileExists(filename string) bool {
	info, err := os.Stat(filename)
	if os.IsNotExist(err) {
		return false
	}
	return !info.IsDir()
}

func IsTestMode() bool {
	return os.Getenv("TEST_MODE") == "true"
}
