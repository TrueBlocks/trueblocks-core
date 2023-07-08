package base

import "encoding/hex"

// Hex2Bytes duplicates the same-named function found in common, because we want to better control dependencies
func Hex2Bytes(str string) []byte {
	h, _ := hex.DecodeString(str)
	return h
}

// Bytes2Hex duplicates the same-named function found in common, because we want to better control dependencies
func Bytes2Hex(d []byte) string {
	return hex.EncodeToString(d)
}
