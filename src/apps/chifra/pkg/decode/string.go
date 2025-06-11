package decode

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	goAbi "github.com/ethereum/go-ethereum/accounts/abi"
)

// TODO: Why can't we just use the call package to call into the token directly? Answer: this code was built before the call package
var AbiStringType goAbi.Type
var abiStringArguments goAbi.Arguments

func init() {
	var err error
	AbiStringType, err = goAbi.NewType("string", "", nil)
	if err != nil {
		panic(err)
	}
	abiStringArguments = goAbi.Arguments{
		{Type: AbiStringType},
	}
}

// ArticulateString tries to convert hex into a string of printable characters
// (ASCII only). If it was successful, then `success` is true.
func ArticulateString(hex string) (strResult string, success bool) {
	if len(hex) < 2 {
		return "", false
	}
	byteValue := base.Hex2Bytes(hex[2:])
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
			if character > 20 {
				// ignore space
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
		// (`|` is the delimiter there)
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

// articulateEncodedString translates EVM string into Go string
func articulateEncodedString(hexStr string) (result string, err error) {
	if len(hexStr) < 2 {
		result = ""
		return
	}
	byteValue := base.Hex2Bytes(hexStr[2:])
	unpacked, err := abiStringArguments.Unpack(byteValue)
	if err != nil {
		return
	}
	result = fmt.Sprint(unpacked[0])
	return
}

// articulateBytes32String turns bytes32 encoded string into Go string
func articulateBytes32String(hexStr string) (result string) {
	if len(hexStr) < 2 {
		return
	}
	input := base.Hex2Bytes(hexStr[2:])
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

// ArticulateStringOrBytes tries to read string from either EVM string
// value or bytes32 hex
func ArticulateStringOrBytes(hexStr string) (string, error) {
	if len(hexStr) < 2 {
		return "", nil
	}
	if len(hexStr[2:]) > 64 {
		return articulateEncodedString(hexStr)
	}

	return articulateBytes32String(hexStr), nil
}
