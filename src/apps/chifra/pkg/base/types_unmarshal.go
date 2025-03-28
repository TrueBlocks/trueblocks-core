package base

import (
	"strconv"
)

// Note that the routines ignore errors, assuming that the JSON data is well-formed.
// If not, we get zero values for the types.

func (v *Value) UnmarshalJSON(data []byte) error {
	str := safeUnquote(string(data))
	result, _ := strconv.ParseUint(str, 0, 64)
	*v = Value(result)
	return nil
}

func (t *Timestamp) UnmarshalJSON(data []byte) error {
	str := safeUnquote(string(data))
	result, _ := strconv.ParseInt(str, 0, 64)
	*t = Timestamp(result)
	return nil
}

// safeUnquote attempts to unquote a string if it is quoted.
// If the string is not quoted, it returns the original string.
func safeUnquote(s string) string {
	if len(s) > 1 && ((s[0] == '"' && s[len(s)-1] == '"') || (s[0] == '\'' && s[len(s)-1] == '\'')) {
		if unquoted, err := strconv.Unquote(s); err == nil {
			return unquoted
		}
	}
	return s
}
