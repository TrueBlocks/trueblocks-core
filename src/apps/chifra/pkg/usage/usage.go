package usage

import (
	"errors"
	"strconv"
	"strings"
)

// Replace accepts an string and an arbitrary number of additional string parameters. It replaces
// {N} in the string with the Nth additional string.
func Replace(msg string, values ...string) string {
	ret := msg
	for index, val := range values {
		rep := "{" + strconv.FormatInt(int64(index), 10) + "}"
		ret = strings.Replace(ret, rep, val, -1)
	}
	return ret
}

// Usage returns an error version of the Replace string
func Usage(msg string, values ...string) error {
	return errors.New(Replace(msg, values...))
}

// Deprecated can be used to mark a command option as deprecated.
func Deprecated(cmd string, rep string) error {
	msg := "The {0} flag has been deprecated."
	if len(rep) > 0 {
		msg += " Use {1} instead."
	}
	return Usage(msg, cmd, rep)
}
