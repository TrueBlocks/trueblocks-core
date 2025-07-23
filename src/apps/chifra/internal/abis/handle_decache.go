// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package abisPkg

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *AbisOptions) HandleDecache(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	if len(opts.Addrs) == 0 {
		if testMode {
			logger.Info("Cleaning empty abis is not tested in test mode.")
		} else {
			filenameChan := make(chan walk.CacheFileInfo)
			var nRoutines = 1
			go walk.WalkCacheFolder(rCtx.Ctx, chain, walk.Cache_Abis, nil, filenameChan)
			for result := range filenameChan {
				switch result.Type {
				case walk.Cache_Abis:
					path := result.Path
					skip := !walk.IsCacheType(path, walk.Cache_Abis, true /* checkExt */)
					if !skip {
						size := file.FileSize(path)
						if size <= int64(len(abi.AbiNotFound)+5) {
							clean := func(in string) string {
								return strings.Replace(strings.Replace(in, " ", "", -1), "\"", "", -1)
							}
							contents := clean(file.AsciiFileToString(path))
							empty := clean(abi.AbiNotFound)
							if len(contents) == 0 || contents == empty {
								_ = abi.CleanAbiCache(chain, path)
							}
						}
					}
				case walk.Cache_NotACache:
					nRoutines--
					if nRoutines == 0 {
						close(filenameChan)
					}
				}
			}
		}
	} else {
		for _, address := range opts.Addrs {
			_ = abi.CleanAbiCache(chain, abi.PathToAbisCache(chain, address))
		}
	}

	return nil
}
