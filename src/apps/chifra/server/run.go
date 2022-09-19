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

	chain := opts.Globals.Chain
	configPath := config.GetPathToRootConfig()
	cachePath := config.GetPathToCache(chain)
	chainConfigPath := config.GetPathToChainConfig(chain) // order matters
	indexPath := config.GetPathToIndex(chain)             // order matters
	rpcProvider := config.GetRpcProvider(chain)
	meta, err := rpcClient.GetMetaData(chain, false)

	logger.Log(logger.InfoC, pad("Server URL:"), apiUrl)
	logger.Log(logger.InfoC, pad("RPC Provider:"), rpcProvider)
	logger.Log(logger.InfoC, pad("Root Config Path:"), configPath)
	logger.Log(logger.InfoC, pad("Chain Config Path:"), chainConfigPath)
	logger.Log(logger.InfoC, pad("Cache Path:"), cachePath)
	logger.Log(logger.InfoC, pad("Index Path:"), indexPath)
	if err != nil {
		log.Fatalf("%s%-18.18s%sCould not load RPC provider. %s %s\n", colors.Green, "Progress:", colors.Red, err, colors.Off)
	} else {
		msg := fmt.Sprintf("%d, %d, %d, %d", meta.Latest, meta.Finalized, meta.Staging, meta.Unripe)
		logger.Log(logger.InfoC, pad("Progress:"), msg)
	}

	log.Fatal(RunInternal(opts.Port))

	return nil
}

func pad(strIn string) string {
	return utils.PadRight(strIn, 18, ' ')
}
