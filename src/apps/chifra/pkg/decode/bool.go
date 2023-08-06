package decode

import (
	"encoding/hex"
	"errors"
)

// ArticulateBool translates EVM hex into Go boolean
func ArticulateBool(hexStr string) (result bool, err error) {
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
