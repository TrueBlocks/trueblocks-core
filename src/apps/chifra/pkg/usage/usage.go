package usage

import (
	"errors"
	"strconv"
	"strings"
)

func Usage(msg string, values ...string) error {
	ret := msg
	for index, val := range values {
		rep := "{" + strconv.FormatInt(int64(index), 10) + "}"
		ret = strings.Replace(ret, rep, val, -1)
	}
	return errors.New(ret)
}

func Deprecated(cmd string, rep string) error {
	msg := "The {0} flag has been deprecated."
	if len(rep) > 0 {
		msg += " Use {1} instead."
	}
	return Usage(msg, cmd, rep)
}
