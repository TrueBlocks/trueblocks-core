// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package daemonPkg

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *DaemonOptions) validateDaemon() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if len(opts.Globals.Chain) > 0 && opts.Globals.Chain != config.GetDefaultChain() {
		return validate.Usage("The {0} option is not supported by the {1} command.", "--chain", "daemon")
	}

	if len(opts.Port) > 0 && !strings.Contains(opts.Port, ":") {
		return validate.Usage("The {0} option ({1}) must {2}.", "--port", opts.Port, "start with ':'")
	}

	return nil
}
