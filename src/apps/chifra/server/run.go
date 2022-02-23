// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package servePkg

import (
	"log"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/spf13/cobra"
)

// Run runs serve command and sends any errors to log.Fatal
func RunServe(cmd *cobra.Command, args []string) error {
	opts := ServeFinishParse(args)

	err := Validate(cmd, args)
	if err != nil {
		return err
	}

	apiUrl := opts.Port
	if !strings.Contains(apiUrl, "http") {
		apiUrl = "http://localhost" + apiUrl
	}

	chain := opts.Globals.Chain
	configPath := config.GetPathToRootConfig()
	cachePath := config.GetPathToCache(chain)
	chainConfigPath := config.GetPathToChainConfig(chain) // order matters
	indexPath := config.GetPathToIndex(chain)             // order matters
	rpcProvider := config.GetRpcProvider(chain)
	meta := rpcClient.GetMetaData(chain, false)

	log.Printf("%s%-18.18s%s%s\n", colors.Green, "Server URL:", colors.Off, apiUrl)
	log.Printf("%s%-18.18s%s%s\n", colors.Green, "RootConfig Path:", colors.Off, configPath)
	log.Printf("%s%-18.18s%s%s\n", colors.Green, "ChainConfig Path:", colors.Off, chainConfigPath)
	log.Printf("%s%-18.18s%s%s\n", colors.Green, "Cache Path:", colors.Off, cachePath)
	log.Printf("%s%-18.18s%s%s\n", colors.Green, "Index Path:", colors.Off, indexPath)
	log.Printf("%s%-18.18s%s%s\n", colors.Green, "RPC Provider:", colors.Off, rpcProvider)
	log.Printf("%s%-18.18s%s%d, %d, %d, %d\n", colors.Green, "Progress:", colors.Off, meta.Latest, meta.Finalized, meta.Staging, meta.Unripe)

	log.Fatal(RunInternal(opts.Port))

	return nil
}
