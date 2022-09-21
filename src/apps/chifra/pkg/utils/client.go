package utils

import "strings"

func IsClientErigon(version string) bool {
	return strings.Contains(strings.ToLower(version), "erigon")
}
