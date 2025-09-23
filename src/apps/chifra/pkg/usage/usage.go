package usage

import (
	"errors"
	"strconv"
	"strings"
)

// Replace accepts a string and an arbitrary number of additional string parameters. It replaces
// {N} in the string with the Nth additional string.
func Replace(msg string, values ...string) string {
	ret := msg
	for index, val := range values {
		rep := "{" + strconv.FormatInt(int64(index), 10) + "}"
		ret = strings.ReplaceAll(ret, rep, val)
	}
	return ret
}

// Usage returns an error version of the Replace string
func Usage(msg string, values ...string) error {
	return errors.New(Replace(msg, values...))
}
