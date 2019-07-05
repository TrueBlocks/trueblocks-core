package main

import (
	"strings"
)

func PadLeft(str string, totalLen int) string {
	if len(str) >= totalLen {
		return str
	}
	zeros := ""
	for i := 0; i < totalLen-len(str); i++ {
		zeros += "0"
	}
	return zeros + str
}

func GoodAddr(addr string) bool {
	if addr < "0x0000000000000000000000000000000000000009" {
		return false
	}
	return true
}

func PotentialAddress(addr string) bool {
	small := "00000000000000000000000000000000000000ffffffffffffffffffffffffff"
	largePrefix := "000000000000000000000000"
	if addr <= small || !strings.HasPrefix(addr, largePrefix) {
		return false
	}
	if strings.HasSuffix(addr, "00000000") {
		return false
	}
	return true
}
