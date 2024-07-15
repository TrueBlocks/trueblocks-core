// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package abisPkg

import (
	"context"
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
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
			go walk.WalkCacheFolder(context.Background(), chain, walk.Cache_Abis, nil, filenameChan)
			for result := range filenameChan {
				switch result.Type {
				case walk.Cache_Abis:
					path := result.Path
					skip := !walk.IsCacheType(path, walk.Cache_Abis, true /* checkExt */)
					if !skip {
						size := file.FileSize(path)
						if size <= int64(len(abi.AbiNotFound)+5) {
							// Over time, the contents of an empty ABI file have taken on various shapes. We clean that up here before comparing.
							clean := func(in string) string {
								return strings.Replace(strings.Replace(in, " ", "", -1), "\"", "", -1)
							}
							contents := clean(file.AsciiFileToString(path))
							empty := clean(abi.AbiNotFound)
							if len(contents) == 0 || contents == empty {
								if file.FileExists(path) {
									_, fn := filepath.Split(path)
									if err := os.Remove(path); err != nil {
										msg := fmt.Sprintf("%sCould not remove abi file %s: %s.%s", colors.Red, fn, err, colors.Off)
										logger.Warn(msg)
									} else {
										msg := fmt.Sprintf("%sAbi file %s removed: %d bytes.%s", colors.Green, fn, size, colors.Off)
										logger.Warn(msg)
									}
								}
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
		for _, addr := range opts.Addrs {
			path := config.PathToCache(chain) + "abis/" + addr + ".json"
			if file.FileExists(path) {
				if err := os.Remove(path); err != nil {
					logger.Warn(colors.Red+"Could not remove abi for address", addr, ":", err, "."+colors.Off)
				} else {
					logger.Info(colors.Green+"Abi file for address", addr, "removed."+colors.Off)
				}
			}
		}
	}

	return nil
}
