package slurpPkg

import (
	"errors"
)

func (opts *SlurpOptions) HandleCountKey() error {
	return errors.New("count not supported by Key")
}
