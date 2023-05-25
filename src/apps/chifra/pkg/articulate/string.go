package articulate

import (
	"fmt"

	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

var AbiStringType abi.Type
var abiStringArguments abi.Arguments

func init() {
	var err error

	AbiStringType, err = abi.NewType("string", "", nil)
	if err != nil {
		panic(err)
	}
	abiStringArguments = abi.Arguments{
		{Type: AbiStringType},
	}
}

// ArticulateString tries to convert hex into a string of printable characters
// (ASCII only). If it was successful, then `success` is true.
func ArticulateString(hex string) (strResult string, success bool) {
	if len(hex) < 2 {
		return "", false
	}
	byteValue := common.Hex2Bytes(hex[2:])
	return articulateBytes(byteValue)
}

func articulateBytes(byteValue []byte) (strResult string, success bool) {
	hasPrintableCharacters := false
	result := make([]byte, 0, len(byteValue))
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

// ArticulateEncodedString translates EVM string into Go string
func ArticulateEncodedString(hex string) (result string, err error) {
	if len(hex) < 2 {
		result = ""
		return
	}
	byteValue := common.Hex2Bytes(hex[2:])
	unpacked, err := abiStringArguments.Unpack(byteValue)
	if err != nil {
		return
	}
	result = fmt.Sprint(unpacked[0])
	return
}

// ArticulateBytes32String turns bytes32 encoded string into Go string
func ArticulateBytes32String(hex string) (result string) {
	if len(hex) < 2 {
		return
	}
	input := common.Hex2Bytes(hex[2:])
	if len(input) == 0 {
		return ""
	}
	// Filter out invalid names, four-byte collisions (0x8406d0897da43a33912995c6ffd792f1f2125cd4)
	if input[0] == 0 {
		return ""
	}
	padStart := len(input)
	for i := (len(input) - 1); i >= 0; i-- {
		if input[i] != 0 {
			break
		}
		padStart = i
	}

	byteValue := input[0:padStart]
	result, _ = articulateBytes(byteValue)

	return
}

// ArticulateEncodedStringOrBytes32 tries to read string from either EVM string
// value or bytes32 hex
func ArticulateEncodedStringOrBytes32(hex string) (string, error) {
	if len(hex) < 2 {
		return "", nil
	}
	if len(hex[2:]) > 64 {
		return ArticulateEncodedString(hex)
	}

	return ArticulateBytes32String(hex), nil
}
