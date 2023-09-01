package scrapePkg

import (
	"encoding/json"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// BlazeManager manages the scraper by keeping track of the progress of the scrape and
// maintaining the wait groups, timestamp array, and processed map. The processed map
// helps us know if every block was visited or not.
type BlazeManager struct {
	Chain        string                  `json:"chain"`
	NChannels    uint64                  `json:"nChannels"`
	StartBlock   uint64                  `json:"startBlock"`
	BlockCount   uint64                  `json:"blockCnt"`
	UnripeDist   uint64                  `json:"unripe"`
	RpcProvider  string                  `json:"rpcProvider"`
	AppsPerChunk uint64                  `json:"-"`
	RipeBlock    uint64                  `json:"ripeBlock"`
	NProcessed   uint64                  `json:"nProcessed"`
	Timestamps   []tslib.TimestampRecord `json:"-"`
	ProcessedMap map[base.Blknum]bool    `json:"-"`
	BlockWg      sync.WaitGroup          `json:"-"`
	AppearanceWg sync.WaitGroup          `json:"-"`
	TimestampsWg sync.WaitGroup          `json:"-"`
}

func (blazeMan *BlazeManager) String() string {
	copy := BlazeManager{
		Chain:      blazeMan.Chain,
		NChannels:  blazeMan.NChannels,
		StartBlock: blazeMan.StartBlock,
		BlockCount: blazeMan.BlockCount,
		UnripeDist: blazeMan.UnripeDist,
		RipeBlock:  blazeMan.RipeBlock,
		NProcessed: blazeMan.NProcessed,
	}
	b, _ := json.MarshalIndent(&copy, "", "  ")
	return string(b)
}

// scrapedData combines the extracted block data, trace data, and log data into a
// structure that is passed through to the AddressChannel for further processing.
type scrapedData struct {
	blockNumber base.Blknum
	traces      []types.SimpleTrace
	logs        []types.SimpleLog
}
