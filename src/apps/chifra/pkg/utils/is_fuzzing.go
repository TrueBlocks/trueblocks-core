package utils

import "os"

func IsFuzzing() bool {
	return os.Getenv("TB_SDK_FUZZER") == "true"
}
