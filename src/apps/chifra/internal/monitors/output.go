// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package monitorsPkg

// EXISTING_CODE
import (
	"io"
	"net/http"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunMonitors(cmd *cobra.Command, args []string) error {
	opts := MonitorsFinishParse(args)

	err := opts.ValidateMonitors()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	if opts.Clean {
		return opts.HandleClean()
	}

	if opts.HandleCrudCommands(os.Stdout) {
		return nil
	}
	return opts.Globals.PassItOn("acctExport", opts.ToCmdLine())
	// EXISTING_CODE
}

func ServeMonitors(w http.ResponseWriter, r *http.Request) bool {
	opts := FromRequest(w, r)

	err := opts.ValidateMonitors()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	if opts.Clean {
		err = opts.HandleClean()
		if err != nil {
			opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		}
		return true
	}

	if !opts.Globals.TestMode { // our test harness does not use DELETE
		delOptions := "--delete, --undelete, or --remove"
		if r.Method == "DELETE" {
			if !opts.Delete && !opts.Undelete && !opts.Remove {
				err = validate.Usage("Specify one of {0} when using the DELETE route.", delOptions)
			}
		} else {
			if opts.Delete || opts.Undelete || opts.Remove {
				delOptions = strings.Replace(delOptions, " or ", " and ", -1)
				err = validate.Usage("The {0} options are not valid when using the GET route.", delOptions)
			}
		}
		if err != nil {
			opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
			return true
		}
	}
	return opts.HandleCrudCommands(w)
	// EXISTING_CODE
}

// EXISTING_CODE
func (opts *MonitorsOptions) HandleCrudCommands(w io.Writer) bool {
	if !(opts.Delete || opts.Undelete || opts.Remove) {
		return false
	}

	for _, addr := range opts.Addrs {
		m := monitor.NewMonitor(opts.Globals.Chain, addr, false, opts.Globals.TestMode)
		if !m.IsMonitor() {
			msg := "Monitor not found for address " + addr + "."
			logger.Log(logger.Info, msg)
			return true
		} else {
			if opts.Delete {
				m.Delete()
				msg := "Monitor " + addr + " was deleted but not removed."
				logger.Log(logger.Info, msg)
			} else if opts.Undelete {
				m.UnDelete()
				msg := "Monitor " + addr + " was undeleted."
				logger.Log(logger.Info, msg)
			}

			if opts.Remove {
				wasRemoved, err := m.Remove()
				if !wasRemoved || err != nil {
					msg := "Monitor for " + addr + " was not  removed. " + err.Error()
					logger.Log(logger.Info, msg)
					return true
				} else {
					msg := "Monitor for " + addr + " was permanently removed."
					logger.Log(logger.Info, msg)
				}
			}
		}
	}
	return true
}

// EXISTING_CODE
