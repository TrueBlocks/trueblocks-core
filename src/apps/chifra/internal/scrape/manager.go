package scrapePkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// BlazeManager manages the scraper by keeping track of the progress of the scrape and
// maintaining the wait groups, timestamp array, and processed map. The processed map
// helps us know if every block was visited or not.
type BlazeManager struct {
	Chain        string                  `json:"chain"`
	opts         *ScrapeOptions          `json:"-"` // pointer back to the command line options
	RipeBlock    uint64                  `json:"ripeBlock"`
	NProcessed   uint64                  `json:"nProcessed"`
	Timestamps   []tslib.TimestampRecord `json:"-"`
	ProcessedMap map[base.Blknum]bool    `json:"-"`
}

// StartBlock returns the start block for the current pass of the scraper.
func (bm *BlazeManager) StartBlock() base.Blknum {
	return base.Blknum(bm.opts.StartBlock)
}

// StartBlock returns the number of blocks to process for this pass of the scraper.
func (bm *BlazeManager) BlockCount() base.Blknum {
	return base.Blknum(bm.opts.BlockCnt)
}

// scrapedData combines the extracted block data, trace data, and log data into a
// structure that is passed through to the AddressChannel for further processing.
type scrapedData struct {
	bn       base.Blknum
	traces   []types.SimpleTrace
	receipts []types.SimpleReceipt
}
