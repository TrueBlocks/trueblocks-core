// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package validate

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
)

func Usage(msg string, values ...string) error {
	return usage.Usage(msg, values...)
}

func IsValidHash(hash string) bool {
	ok, err := base.IsValidHex("hash", hash, 32)
	return ok && err == nil
}

func IsValidFourByteE(val string) (bool, error) {
	return base.IsValidHex("fourbyte", val, 4)
}

func IsValidFourByte(val string) bool {
	ok, _ := base.IsValidHex("fourbyte", val, 4)
	return ok
}

func IsValidTopicE(val string) (bool, error) {
	return base.IsValidHex("topic", val, 32)
}

func IsValidTopic(val string) bool {
	ok, _ := base.IsValidHex("topic", val, 32)
	return ok
}

func ValidateAddresses(args []string) error {
	for _, arg := range args {
		if !base.IsValidAddress(arg) {
			return Usage("The value {0} is not a valid address.", arg)
		}
	}
	return nil
}

func ValidateExactlyOneAddr(args []string) error {
	if err := ValidateAtLeastOneAddr(args); err != nil {
		return Usage("Please specify a valid Ethereum address.")
	}

	if len(args) > 1 {
		return Usage("Please specify only a single Ethereum address.")
	}

	if !base.IsValidAddress(args[0]) {
		return Usage("The value {0} is not a valid address.", args[0])
	}

	return nil
}

func ValidateAtLeastOneNonSentinal(args []string) error {
	if err := ValidateAtLeastOneAddr(args); err != nil {
		return err
	}
	for _, arg := range args {
		if arg == base.NotAMonitor.Hex() {
			return Usage("The address {0} is reserved for system use.", arg)
		}
	}
	return nil
}

func ValidateAtLeastOneAddr(args []string) error {
	hasOne := false
	for _, arg := range args {
		if hasOne {
			break
		}
		hasOne = base.IsValidAddress(arg)
	}
	if hasOne {
		return nil
	}
	return Usage("Please specify at least one {0}.", "valid Ethereum address")
}

func ValidateEnumRequired(field, value, valid string) error {
	if len(value) == 0 {
		return Usage("Please choose at least one of {0}.", valid)
	}
	return ValidateEnum(field, value, valid)
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

// HasArticulationKey returns true if the articulation key is present
func HasArticulationKey(on bool) bool {
	if !on {
		return true
	}
	return len(config.GetKey("etherscan").ApiKey) > 0
}
