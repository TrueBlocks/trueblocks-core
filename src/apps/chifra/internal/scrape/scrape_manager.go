package scrapePkg

import (
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

// BlazeManager manages the scraper by keeping track of the progress of the scrape and
// maintaining the timestamp array and processed map. The processed map helps us know
// if every block was visited or not.
type BlazeManager struct {
	chain        string
	timestamps   []tslib.TimestampRecord
	processedMap map[base.Blknum]bool
	opts         *ScrapeOptions
	meta         *rpc.MetaData
	startBlock   base.Blknum
	blockCount   base.Blknum
	ripeBlock    base.Blknum
	nRipe        int
	nUnripe      int
	nChannels    int
}

// StartBlock returns the start block for the current pass of the scraper.
func (bm *BlazeManager) StartBlock() base.Blknum {
	return bm.startBlock
}

// BlockCount returns the number of blocks to process for this pass of the scraper.
func (bm *BlazeManager) BlockCount() base.Blknum {
	return bm.blockCount
}

// EndBlock returns the last block to process for this pass of the scraper.
func (bm *BlazeManager) EndBlock() base.Blknum {
	return bm.startBlock + bm.blockCount
}

// nProcessed returns the number of blocks processed so far (i.e., ripe + unripe).
func (bm *BlazeManager) nProcessed() int {
	return bm.nRipe + bm.nUnripe
}

// IsTestMode returns true if the scraper is running in test mode.
func (bm *BlazeManager) IsTestMode() bool {
	return bm.opts.Globals.TestMode
}

// AllowMissing returns true for all chains but mainnet and the value
// of the config item on mainnet (false by default). The scraper will
// halt if AllowMissing is false and a block with zero appearances is
// encountered.
func (bm *BlazeManager) AllowMissing() bool {
	if bm.chain != "mainnet" {
		return true
	}
	return bm.opts.Settings.Allow_missing
}

// PerChunk returns the number of blocks to process per chunk.
func (bm *BlazeManager) PerChunk() base.Blknum {
	return bm.opts.Settings.Apps_per_chunk
}

// FirstSnap returns the first block to process.
func (bm *BlazeManager) FirstSnap() base.Blknum {
	return bm.opts.Settings.First_snap
}

// SnapTo returns the number of blocks to process per chunk.
func (bm *BlazeManager) SnapTo() base.Blknum {
	return bm.opts.Settings.Snap_to_grid
}

// IsSnap returns true if the block is a snap point.
func (bm *BlazeManager) IsSnap(block base.Blknum) bool {
	return block >= bm.FirstSnap() && (block%bm.SnapTo()) == 0
}

// StageFolder returns the folder where the stage file is stored.
func (bm *BlazeManager) StageFolder() string {
	return filepath.Join(config.GetPathToIndex(bm.chain), "staging")
}

// RipeFolder returns the folder where the stage file is stored.
func (bm *BlazeManager) RipeFolder() string {
	return filepath.Join(config.GetPathToIndex(bm.chain), "ripe")
}

// UnripeFolder returns the folder where the stage file is stored.
func (bm *BlazeManager) UnripeFolder() string {
	return filepath.Join(config.GetPathToIndex(bm.chain), "unripe")
}
