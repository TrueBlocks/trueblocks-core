// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *MonitorsOptions) ValidateMonitors() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	//								 [Action]
	// [State]		|	Delete		Undelete		Remove
	// -------------|----------------------------------------
	// Not Deleted	|	Delete		Error			Error
	// Deleted		|	Error		Undelete		Remove
	// -------------|----------------------------------------

	isCrud := opts.Delete || opts.Undelete || opts.Remove
	if isCrud {
		chain := opts.Globals.Chain
		if opts.Undelete {
			if opts.Delete || opts.Remove {
				return validate.Usage("The --undelete option may not be used with --delete or --remove.")
			}
			for _, addr := range opts.Addrs {
				if !IsMonitorDeleted(chain, addr, opts.Globals.TestMode) {
					return validate.Usage("Monitor for {0} must be deleted before being undeleted.", addr)
				}
			}
		} else {
			// We have to handle the case where both --delete and --remove are sent in the same command
			if opts.Delete {
				for _, addr := range opts.Addrs {
					if IsMonitorDeleted(chain, addr, opts.Globals.TestMode) {
						return validate.Usage("Monitor for {0} is already deleted.", addr)
					}
				}
			} else if opts.Remove {
				for _, addr := range opts.Addrs {
					if !IsMonitorDeleted(chain, addr, opts.Globals.TestMode) {
						return validate.Usage("Monitor for {0} must be deleted before being removed.", addr)
					}
				}
			}
		}
	}

	if !opts.Globals.ApiMode && !opts.Clean {
		if len(opts.Globals.File) > 0 {
			// Do nothing
		} else {
			err := validate.ValidateAtLeastOneAddr(opts.Addrs)
			if err != nil {
				return err
			}
		}
	}

	return opts.Globals.ValidateGlobals()
}

func IsMonitorDeleted(chain, addr string, testMode bool) bool {
	m := monitor.NewMonitor(chain, addr, false /* create */, testMode)
	return m.IsDeleted()
}
