// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package abisPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *AbisOptions) validateAbis() error {
	chain := opts.Globals.Chain
	proxy := base.HexToAddress(opts.ProxyFor)

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	if opts.Globals.Decache {
		if len(opts.Encode) > 0 {
			return validate.Usage("Please choose only one of {0}.", "--decache or --encode")
		}
		if len(opts.Find) > 0 {
			return validate.Usage("Please choose only one of {0}.", "--decache or --find")
		}
		if opts.Known {
			return validate.Usage("Please choose only one of {0}.", "--decache or --known")
		}
		if !proxy.IsZero() {
			return validate.Usage("Please choose only one of {0}.", "--decache or --proxy_for")
		}
	}

	if len(opts.Globals.File) == 0 &&
		len(opts.Encode) == 0 &&
		len(opts.Find) == 0 &&
		!opts.List &&
		!opts.Known &&
		!opts.Globals.Decache {
		// If we're not find and not known we better have at least one address
		err := validate.ValidateAtLeastOneAddr(opts.Addrs)
		if err != nil {
			return err
		}
	}

	if len(opts.Find) > 0 && len(opts.Encode) > 0 {
		return validate.Usage("Please choose only one of {0}.", "--find or --encode")
	}

	if len(opts.Addrs) != 1 && !proxy.IsZero() {
		return validate.Usage("The {0} option requires exactly one address.", "--proxy_for")
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
