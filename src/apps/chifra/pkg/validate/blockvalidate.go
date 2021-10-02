package validate

import (
	"fmt"
	"regexp"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/blockRange"
)

func IsBlockHash(str string) bool {
	if !Is0xPrefixed(str) {
		return false
	}

	if len(str[2:]) != 64 {
		return false
	}

	if !IsHex(str) {
		return false
	}

	return true
}

func IsBlockNumber(str string) bool {
	_, err := strconv.ParseUint(str, 10, 64)

	return err == nil
}

func IsSpecialBlock(str string) bool {
	// We have no way to tell if a string is valid special block
	// name without calling RPC, so we only check if it looks
	// like a valid name here
	_, err := strconv.Atoi(str)

	if err == nil {
		fmt.Println("Detected number")
		// numbers not allowed
		return false
	}

	valid, _ := regexp.MatchString(`^[a-z0-9]{3,}$`, str)
	return valid
}

func IsDateTimeString(str string) bool {
	bRange, err := blockRange.New(str)

	if err != nil {
		return false
	}

	return bRange.StartType == blockRange.BlockRangeDate
}

func IsRange(str string) bool {
	_, err := blockRange.New(str)

	return err == nil
}
