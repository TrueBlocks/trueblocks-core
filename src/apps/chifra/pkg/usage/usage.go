package usage

import (
	"errors"
	"strconv"
	"strings"
)

// Usage accepts an error string and an arbitrary number of additional string parameters. The error string
// may contain replacement values of the form {N}, where N corresponds to an index into the additional
// strings. This allows, eventually, for internationalization of the usage strings. An error is returned.
func Usage(msg string, values ...string) error {
	ret := msg
	for index, val := range values {
		rep := "{" + strconv.FormatInt(int64(index), 10) + "}"
		ret = strings.Replace(ret, rep, val, -1)
	}
	return errors.New(ret)
}

// Deprecated can be used to mark a command option as deprecated.
func Deprecated(cmd string, rep string) error {
	msg := "The {0} flag has been deprecated."
	if len(rep) > 0 {
		msg += " Use {1} instead."
	}
	return Usage(msg, cmd, rep)
}
