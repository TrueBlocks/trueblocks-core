package base

import "os"

func IsTestMode() bool {
	// This is fast, so we should not cache the result
	return os.Getenv("TEST_MODE") == "true"
}
