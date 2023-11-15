package parser

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// Selector captures four byte function selector (e.g. 0xcdba2fd4)
// It's capture method makes sure that the hex value is a valid selector.
type Selector struct {
	Value string
}

func (s *Selector) Capture(values []string) error {
	literal := values[0]
	if valid, _ := base.IsValidHex("", literal, 4); !valid {
		return errInvalidSelector
	}

	s.Value = literal
	return nil
}
