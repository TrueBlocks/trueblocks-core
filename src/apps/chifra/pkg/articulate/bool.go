package articulate

import (
	"encoding/hex"
	"errors"
)

// ArticulateBoolean translates EVM hex into Go boolean
func ArticulateBoolean(hexStr string) (result bool, err error) {
	// 0x + 64 characters
	if len(hexStr) < 66 {
		err = errors.New("hex string too short")
		return
	}
	byteValue, err := hex.DecodeString(hexStr[2:])
	if err != nil {
		return
	}
	result = byteValue[len(byteValue)-1] == 1
	return
}
