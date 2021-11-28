// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ExportOptions) ValidateExport() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if len(opts.Globals.File) == 0 {
		err := validate.ValidateAtLeastOneAddr(opts.Addrs)
		if err != nil {
			return err
		}
	}

	err := validate.ValidateEnum("--summarize_by", Options.SummarizeBy, "[yearly|quarterly|monthly|weekly|daily|hourly|blockly|tx]")
	if err != nil {
		return err
	}

	if len(Options.SummarizeBy) > 0 && !Options.Accounting {
		return validate.Usage("The {0} option is available only with {1}.", "--summarized_by", "--accounting")
	}

	return opts.Globals.ValidateGlobals()
}
