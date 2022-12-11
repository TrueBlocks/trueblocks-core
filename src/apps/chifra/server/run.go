// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package servePkg

import (
	"fmt"
	"log"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
)

// Run runs serve command and sends any errors to log.Fatal
func RunServe(cmd *cobra.Command, args []string) error {
	opts := serveFinishParse(args)

	err := Validate(cmd, args)
	if err != nil {
		return err
	}

	apiUrl := opts.Port
	if !strings.HasPrefix(apiUrl, "http") {
		apiUrl = "http://localhost" + apiUrl
	}

	pad := func(strIn string) string {
		return utils.PadRight(strIn, 18, ' ')
	}

	chain := opts.Globals.Chain
	meta, err := rpcClient.GetMetaData(chain, false)
	logger.Log(logger.InfoC, pad("Server URL:"), apiUrl)
	logger.Log(logger.InfoC, pad("RPC Provider:"), config.GetRpcProvider(chain))
	logger.Log(logger.InfoC, pad("Root Config Path:"), config.GetPathToRootConfig())
	logger.Log(logger.InfoC, pad("Chain Config Path:"), config.GetPathToChainConfig(chain))
	logger.Log(logger.InfoC, pad("Cache Path:"), config.GetPathToCache(chain))
	logger.Log(logger.InfoC, pad("Index Path:"), config.GetPathToIndex(chain))
	if err != nil {
		msg := fmt.Sprintf("%sCould not load RPC provider: %s%s", colors.Red, err, colors.Off)
		logger.Log(logger.InfoC, pad("Progress:"), msg)
		log.Fatalf("")
	} else {
		nTs, _ := tslib.NTimestamps(opts.Globals.Chain)
		msg := fmt.Sprintf("%d, %d, %d,  %d, ts: %d", meta.Latest, meta.Finalized, meta.Staging, meta.Unripe, nTs)
		logger.Log(logger.InfoC, pad("Progress:"), msg)
	}

	log.Fatal(ServeInternal(opts.Port))

	return nil
}
