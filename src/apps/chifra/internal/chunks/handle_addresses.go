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

func (opts *ChunksOptions) showAddresses(path string, first bool) error {
	path = index.ToIndexPath(path)

	indexChunk, err := index.NewChunkData(path)
	if err != nil {
		return err
	}
	defer indexChunk.Close()

	_, err = indexChunk.File.Seek(int64(index.HeaderWidth), io.SeekStart)
	if err != nil {
		return err
	}

	for i := 0; i < int(indexChunk.Header.AddressCount); i++ {
		if opts.Globals.TestMode && i > maxTestItems {
			continue
		}
		obj, err := indexChunk.ReadAddressRecord()
		if err != nil {
			return err
		}

		if opts.shouldShow(obj) {
			if opts.Globals.LogLevel >= 4 {
				offset, err := indexChunk.File.Seek(0, io.SeekCurrent)
				if err == nil {
					apps, _ := indexChunk.ReadAppearanceRecords(&obj)
					if err == nil {
						_, err = indexChunk.File.Seek(offset, io.SeekStart)
						if err == nil {
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
			} else {
				err = opts.Globals.RenderObject(obj, first && i == 0)
				if err != nil {
					return err
				}
			}
		}
	}

	return nil
}

func (opts *ChunksOptions) shouldShow(obj index.AddressRecord) bool {
	return !opts.Belongs || strings.ToLower(obj.Address.Hex()) == opts.Addrs[0]
}
