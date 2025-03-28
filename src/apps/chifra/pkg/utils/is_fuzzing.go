package utils

import "os"

var isFuzzing *bool = nil

func IsFuzzing() bool {
	if isFuzzing == nil {
		is := os.Getenv("TB_SDK_FUZZER") == "true"
		isFuzzing = &is
	}
	return *isFuzzing
}

func SetIsFuzzing(fuzz bool) {
	isFuzzing = &fuzz
}
