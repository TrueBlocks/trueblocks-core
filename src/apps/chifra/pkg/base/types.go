package base

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
)

type Gas = uint64
type Blknum = uint64
type Txnum = uint64
type Topic = string
type Timestamp = int64

func IsFinal(latestTs, blockTs Timestamp) bool {
	// TODO: This is not consistent with they way we determine unripe in the scraper, for example.
	var min = Timestamp(5)
	var s = Timestamp(60)
	var pendingPeriod = min * s
	var diff = latestTs - blockTs
	return diff >= pendingPeriod
}

func init() {
	if os.Getenv("NO_COLOR") == "true" {
		colors.ColorsOff()
	}
}
