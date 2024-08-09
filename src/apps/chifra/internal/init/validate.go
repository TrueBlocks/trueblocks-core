// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/history"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *InitOptions) validateInit() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	if opts.Globals.TestMode {
		return validate.Usage("integration testing was skipped for chifra init")
	}

	if len(opts.Publisher) > 0 {
		err := validate.ValidateExactlyOneAddr([]string{opts.Publisher})
		if err != nil {
			return err
		}
	}

	if len(opts.Example) > 0 {
		cwd, _ := os.Getwd()
		if !strings.HasSuffix(cwd, "examples") {
			return fmt.Errorf("must be in the ./examples directory to run this command")
		}

		if valid, err := file.IsValidFolderName(opts.Example); err != nil {
			return usage.Usage("{0} {1}", opts.Example, fmt.Sprintf("%v", err))
		} else if !valid {
			return usage.Usage("{0} is not a valid folder name", opts.Example)
		}
		// if len(opts.Template) > 0 {
		// must exist
		// }
		// } else if len(opts.Template) > 0 {
		// 	return validate.Usage("The {0} option requires the {1} flag.", "--template", "--example")
	} else {
		historyFile := filepath.Join(config.PathToCache(chain), "tmp/history.txt")
		if history.FromHistoryBool(historyFile, "init") && !opts.All {
			return validate.Usage("You previously called chifra init --all. You must continue to do so.")
		}
	}

	return opts.Globals.Validate()
}
