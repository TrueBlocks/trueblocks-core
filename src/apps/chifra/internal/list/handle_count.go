// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"net/http"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
)

func (opts *ListOptions) HandleListCount() error {
	// TODO: BOGUS -- NEEDS TO FRESHEN FIRST OTHERWISE TESTS BREAK
	var result []monitor.MonitorLight
	for _, addr := range opts.Addrs {
		m := monitor.NewMonitorLight(opts.Globals.Chain, addr)
		result = append(result, m)
	}

	if opts.Globals.ApiMode {
		opts.Globals.Respond(opts.Globals.Writer, http.StatusOK, result)

	} else {
		err := opts.Globals.Output(os.Stdout, opts.Globals.Format, result)
		if err != nil {
			logger.Log(logger.Error, err)
		}
	}

	return nil
}
