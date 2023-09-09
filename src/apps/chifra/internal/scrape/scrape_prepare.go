package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/prefunds"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

// Prepare performs actions that need to be done prior to entering the
// forever loop. Returns true if processing should continue, false otherwise.
// The routine cleans the temporary folders (if any) and then makes sure the zero
// block (reads the allocation file, if present) is processed.
func (opts *ScrapeOptions) Prepare() (ok bool, err error) {
	chain := opts.Globals.Chain

	// We always clean the temporary folders (other than staging) when starting
	_ = index.CleanEphemeralIndexFolders(chain)

	// If the file already exists, we're done.
	bloomPath := config.GetPathToIndex(chain) + "blooms/000000000-000000000.bloom"
	if file.FileExists(bloomPath) {
		return true, nil
	}

	// If there are no prefunds, we're done.
	prefundPath := filepath.Join(config.GetPathToChainConfig(chain), "allocs.csv")
	prefunds, err := prefunds.LoadPrefunds(chain, prefundPath, nil)
	if err != nil {
		return false, err
	}

	appMap := make(index.AddressAppearanceMap, len(prefunds))
	for i, prefund := range prefunds {
		addr := prefund.Address.Hex()
		appMap[addr] = append(appMap[addr], index.AppearanceRecord{
			BlockNumber:   0,
			TransactionId: uint32(i),
		})
	}

	array := []tslib.TimestampRecord{}
	array = append(array, tslib.TimestampRecord{
		Bn: uint32(0),
		Ts: uint32(opts.Conn.GetBlockTimestamp(0)),
	})
	_ = tslib.Append(chain, array)

	logger.Info("Writing block zero allocations for", len(prefunds), "prefunds, nAddresses:", len(appMap))
	indexPath := index.ToIndexPath(bloomPath)
	publisher := base.ZeroAddr
	if report, err := index.WriteChunk(chain, indexPath, publisher, appMap, len(prefunds), opts.Pin, opts.Remote); err != nil {
		return false, err
	} else if report == nil {
		logger.Fatal("Should not happen, write chunk returned empty report")
	} else {
		report.Snapped = true // assumes block zero is a snap to grid (which it is in a sense)
		report.Report()
	}

	return true, nil
}
