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
		sanitized, replaced := sanitizeByte(character)
		// if any character has been replaced, it was a special character
		if replaced > 0 {
			result = append(result, sanitized...)
			continue
		}
		// if we are here, the character is not a special one, so we need
		// to check if it's ASCII printable
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

func sanitizeByte(character byte) (replacement []byte, replaced int) {
	if character == '\\' || character == '\r' {
		return
	}
	if character == '"' {
		return []byte{'\''}, 1
	}
	if character == ',' {
		// C++ used `|` to replace commas, but it breaks compressed* fields format
		// (`|` is the delimeter there)
		return []byte{'_'}, 1
	}
	if character == '|' {
		return []byte{';'}, 1
	}
	if character == '\n' {
		return []byte("[n]"), 1
	}
	if character == '\t' {
		return []byte("[t]"), 1
	}
	return []byte{character}, 0
}

func SanitizeString(str string) (sanitized string) {
	for _, character := range str {
		sanitizedByte, _ := sanitizeByte(byte(character))
		sanitized += string(sanitizedByte)
	}
	return
}
