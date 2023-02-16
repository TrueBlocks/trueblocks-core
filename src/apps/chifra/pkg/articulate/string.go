package articulate

import (
	"github.com/ethereum/go-ethereum/common"
)

// ArticulateString tries to convert hex into a string of printable characters
// (ASCII only). If it was successful, then `success` is true.
func ArticulateString(hex string) (strResult string, success bool) {
	byteValue := common.Hex2Bytes(hex[2:])
	hasPrintableCharacters := false
	result := make([]byte, 0, len(hex))
	for _, character := range byteValue {
		if character == '\\' || character == '\r' {
			continue
		}
		if character == '"' {
			result = append(result, '\'')
			continue
		}
		if character == '\n' {
			result = append(result, []byte("[n]")...)
			continue
		}
		if character == '\t' {
			result = append(result, []byte("[t]")...)
			continue
		}
		if character >= 20 && character <= 126 {
			result = append(result, byte(character))

			// ignore space
			if character > 20 {
				hasPrintableCharacters = true
			}
			continue
		}
		// The character is not ASCII
		return "", false
	}

	if hasPrintableCharacters {
		return string(result), true
	}

	return
}
