// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package validate

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
)

// TODO: check if ParseUint has better performance
// TODO: TJR - I did the test - ParseUint is slower and
// TODO: TJR - does not handle hashes that are too long
func IsHex(str string) bool {
	return len(strings.Trim(str[2:], "0123456789abcdefABCDEF")) == 0
}

func Usage(msg string, values ...string) error {
	return usage.Usage(msg, values...)
}

func Deprecated(cmd, rep string) error {
	return usage.Deprecated(cmd, rep)
}

func Is0xPrefixed(str string) bool {
	if len(str) < 3 {
		return false
	}
	return str[:2] == "0x"
}

func IsValidHex(typ string, val string, nBytes int) (bool, error) {
	if !Is0xPrefixed(val) {
		return false, Usage("The {0} option ({1}) must {2}.", typ, val, "start with '0x'")
	} else if len(val) != (2 + nBytes*2) {
		return false, Usage("The {0} option ({1}) must {2}.", typ, val, fmt.Sprintf("be %d bytes long", nBytes))
	} else if !IsHex(val) {
		return false, Usage("The {0} option ({1}) must {2}.", typ, val, "be hex")
	}
	return true, nil
}

func IsValidFourByteE(val string) (bool, error) {
	return IsValidHex("fourbyte", val, 4)
}

func IsValidFourByte(val string) bool {
	ok, _ := IsValidHex("fourbyte", val, 4)
	return ok
}

func IsValidTopicE(val string) (bool, error) {
	return IsValidHex("topic", val, 32)
}

func IsValidTopic(val string) bool {
	ok, _ := IsValidHex("topic", val, 32)
	return ok
}

func IsValidAddress(val string) bool {
	if strings.Contains(val, ".eth") {
		return true
	}
	ok, _ := IsValidHex("address", val, 20)
	return ok
}

func IsValidAddressE(val string) (bool, error) {
	if strings.Contains(val, ".eth") {
		return true, nil
	}
	return IsValidHex("address", val, 20)
}

func IsZeroAddress(val string) bool {
	v := strings.Replace(val, "0", "", -1)
	return v == "x" || v == "X"
}

func ValidateAtLeastOneAddr(args []string) error {
	hasOne := false
	for _, arg := range args {
		if hasOne {
			break
		}
		hasOne = IsValidAddress(arg)
	}
	if hasOne {
		return nil
	}
	return Usage("Please specify at least one {0}.", "valid Ethereum address")
}

func ValidateEnum(field, value, valid string) error {
	if len(value) == 0 {
		return nil
	}
	valid = strings.Replace(valid, "[", "|", 1)
	valid = strings.Replace(valid, "]", "|", 1)
	if strings.Contains(valid, "|"+value+"|") {
		return nil
	}
	parts := strings.Split(strings.Trim(valid, "|"), "|")
	list := ""
	for _, part := range parts {
		if len(list) > 0 {
			list += " | "
		}
		list += part
	}
	return Usage("The {0} option ({1}) must be one of [ {2} ]", field, value, list)
}

func ValidateEnumSlice(field string, values []string, valid string) error {
	for _, value := range values {
		err := ValidateEnum(field, value, valid)
		if err != nil {
			return err
		}
	}
	return nil
}
