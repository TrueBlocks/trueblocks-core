// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package abisPkg

import (
	"fmt"
	"os"
	"path"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (opts *AbisOptions) HandleClean() error {
	if opts.Globals.IsApiMode() {
		return fmt.Errorf("clean is not supported in API mode")
	}

	if len(opts.Addrs) == 0 {
		// TODO: This code is not actually used
		// filenameChan := make(chan paths.CacheFileInfo)
		// var nRoutines int = 1
		// go paths.WalkCacheFolder(opts.Globals.Chain, paths.Cache_Abis, filenameChan)
		// for result := range filenameChan {
		// 	switch result.Type {
		// 	case paths.Cache_Abis:
		// 		skip := !strings.HasSuffix(result.Path, ".json")
		// 		if !skip {
		// 			if file.FileSize(result.Path) == 0 {
		// 				logger.Log(logger.Info, "Removing empty abi: "+strings.Replace(result.Path, config.GetPathToCache(opts.Globals.Chain)+"abis/", "", -1), file.FileSize(result.Path))
		// 				// if err := cleanOneAbi(result.Path, ""); err != nil {
		// 				// 	return err
		// 				// }
		// 			}
		// 		}
		// 	case paths.None:
		// 		nRoutines--
		// 		if nRoutines == 0 {
		// 			close(filenameChan)
		// 		}
		// 	}
		// }
	} else {
		for _, addr := range opts.Addrs {
			if err := cleanOneAbi(config.GetPathToCache(opts.Globals.Chain)+"abis/", addr); err != nil {
				return err
			}
		}
	}

	return nil
}

func cleanOneAbi(abiFolder, addr string) error {
	fn := abiFolder
	if !strings.HasSuffix(fn, ".json") {
		fn = path.Join(fn, addr+".json")
	}

	if !file.FileExists(fn) {
		logger.Log(logger.Info, colors.Red, "Abi", addr, "not found", colors.Off)
		return nil
	}

	if err := os.Remove(fn); err != nil {
		return err
	}

	logger.Log(logger.Info, colors.Green, "Abi for", addr, "removed", colors.Off)
	return nil
}
