package index

type ErrorType int

const (
	OKAY ErrorType = iota
	FILE_MISSING
	WRONG_SIZE
	WRONG_MAGIC
	WRONG_HASH
	FILE_ERROR
	NOT_IN_MANIFEST
	AFTER_MANIFEST
)

var Reasons = map[ErrorType]string{
	OKAY:            "okay",
	FILE_ERROR:      "file error",
	FILE_MISSING:    "file missing",
	WRONG_SIZE:      "wrong size",
	WRONG_MAGIC:     "wrong magic number",
	WRONG_HASH:      "wrong header hash",
	NOT_IN_MANIFEST: "not in manifest",
	AFTER_MANIFEST:  "range after manifest",
}
