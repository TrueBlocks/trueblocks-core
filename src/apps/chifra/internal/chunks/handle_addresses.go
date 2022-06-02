// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"
	"io"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
)

func (opts *ChunksOptions) showAddresses(path string, first bool) (bool, error) {
	path = index.ToIndexPath(path)

	indexChunk, err := index.NewChunkData(path)
	if err != nil {
		return false, err
	}
	defer indexChunk.Close()

	_, err = indexChunk.File.Seek(int64(index.HeaderWidth), io.SeekStart)
	if err != nil {
		return false, err
	}

	cnt := 0
	for i := 0; i < int(indexChunk.Header.AddressCount); i++ {
		if opts.Globals.TestMode && i > maxTestItems {
			continue
		}
		obj, err := indexChunk.ReadAddressRecord()
		if err != nil {
			return false, err
		}

		if opts.shouldShow(obj) {
			if opts.Globals.LogLevel < 4 || opts.Belongs {
				err = opts.Globals.RenderObject(obj, first && cnt == 0)
				if err != nil {
					return false, err
				}
				cnt++
				if opts.Belongs {
					apps, err := indexChunk.ReadAppearanceRecordsAndResetOffset(&obj)
					if err != nil {
						return false, err
					}
					for _, a := range apps {
						err = opts.Globals.RenderObject(a, false)
						if err != nil {
							return false, err
						}
					}
				}

			} else {
				apps, err := indexChunk.ReadAppearanceRecordsAndResetOffset(&obj)
				if err != nil {
					return false, err
				}
				var lead string
				scanBar := progress.NewScanBar(uint64(len(apps)), 1, uint64(len(apps)), (1. / 3.))
				for _, a := range apps {
					lead = fmt.Sprintf("%s % 6d % 6d", strings.ToLower(obj.Address.Hex()), obj.Offset, obj.Count)
					msg := fmt.Sprintf("%d.%d", a.BlockNumber, a.TransactionId)
					scanBar.Report(os.Stderr, lead, msg)
				}
				scanBar.Report(os.Stderr, lead, "                         ")
			}
		}
	}

	return true, nil
}

func (opts *ChunksOptions) shouldShow(obj index.AddressRecord) bool {
	if !opts.Belongs {
		return true
	}
	return strings.ToLower(obj.Address.Hex()) == opts.Addrs[0]
}
