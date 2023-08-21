package utils

import "strings"

// TODO: Add queries for other node software (besu, nethermind, geth, etc.)

// IsClientErigon checks if `version` looks like Erigon
func IsClientErigon(version string) bool {
	return strings.Contains(strings.ToLower(version), "erigon")
}
