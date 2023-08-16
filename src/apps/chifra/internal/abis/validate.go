// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package abisPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *AbisOptions) validateAbis() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.Clean {
		if opts.Globals.IsApiMode() {
			return validate.Usage("The {0} option is not available{1}.", "--clean", " in API mode")
		}
		if len(opts.Encode) > 0 {
			return validate.Usage("Please choose only one of {0}.", "--clean or --encode")
		}
		if len(opts.Find) > 0 {
			return validate.Usage("Please choose only one of {0}.", "--clean or --find")
		}
		if opts.Known {
			return validate.Usage("Please choose only one of {0}.", "--clean or --known")
		}
	}

	if len(opts.Globals.File) == 0 && len(opts.Encode) == 0 && len(opts.Find) == 0 && !opts.Known && !opts.Clean {
		// If we're not find and not known we better have at least one address
		err := validate.ValidateAtLeastOneAddr(opts.Addrs)
		if err != nil {
			return err
		}
	}

	if len(opts.Find) > 0 && len(opts.Encode) > 0 {
		return validate.Usage("Please choose only one of {0}.", "--find or --encode")
	}

	for _, term := range opts.Find {
		ok1, err1 := validate.IsValidFourByteE(term)
		if !ok1 && len(term) < 10 {
			return err1
		}
		ok2, err2 := validate.IsValidTopicE(term)
		if !ok2 && len(term) > 66 {
			return err2
		}
		if !ok1 && !ok2 {
			if len(term) > 43 {
				// more than halfway reports topic
				return err2
			}
			return err1
		}
	}

	return opts.Globals.Validate()
}
