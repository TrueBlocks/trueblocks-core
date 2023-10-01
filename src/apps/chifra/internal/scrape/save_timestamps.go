package scrapePkg

import (
	"encoding/binary"
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

// TODO: Protect against overwriting files on disc

func (bm *BlazeManager) WriteTimestamps(blocks []base.Blknum) error {
	chain := bm.chain

	// At all times, the timestamp file is complete (that is, there are no missing pieces
	// and the last record is at block nTimestamps. We can append as we go (which is fast).
	tsPath := config.PathToTimestamps(chain)
	fp, err := os.OpenFile(tsPath, os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
	if err != nil {
		return err
	}
	defer func() {
		tslib.ClearCache(chain)
		fp.Close()
	}()

	nTimestamps, err := tslib.NTimestamps(chain)
	if err != nil {
		return err
	}

	// usage.QueryUser(fmt.Sprintf("%d-%d-%d", blocks[0], blocks[len(blocks)-1], nTimestamps), "")

	if blocks[len(blocks)-1] < nTimestamps {
		// We already have all of these timestamps, leave early
		// usage.QueryUser("Leaving early", "")
		return nil

	} else if blocks[0] > nTimestamps {
		// we need to catch up (for example, the user truncated the timestamps file while debugging)
		for block := nTimestamps; block < blocks[0]; block++ {
			// usage.QueryUser(fmt.Sprintf("backfill: %d %d %d %d", block, blocks[0], nTimestamps, blocks[len(blocks)-1]), "")
			ts := tslib.TimestampRecord{
				Bn: uint32(block),
				Ts: uint32(bm.opts.Conn.GetBlockTimestamp(block)),
			}
			if err := writeOne(fp, &ts, block, blocks); err != nil {
				return err
			}
		}
	}

	// usage.QueryUser(fmt.Sprintf("done backfilling: %d-%d-%d", blocks[0], blocks[len(blocks)-1], nTimestamps), "")

	// Append to the timestamps file all the new timestamps but as we do that make sure we're
	// not skipping anything at the front, in the middle, or at the end of the list
	for _, block := range blocks {
		if block < nTimestamps {
			// We already have this timestampe, skip out
			continue
		}

		if base.Blknum(bm.timestamps[block].Bn) != block {
			return fmt.Errorf("timestamp missing at block %d", block)
		}

		ts := bm.timestamps[block]
		// usage.QueryUser(fmt.Sprintf("writing: %d-%d-%d", block, ts.Bn, ts.Ts), "")
		if err := writeOne(fp, &ts, block, blocks); err != nil {
			return err
		}

	}

	logger.Progress(true, fmt.Sprintf("Finished writing timestamps to block %-04d"+spaces,
		blocks[len(blocks)-1],
	))

	return nil
}

func writeOne(fp *os.File, ts *tslib.TimestampRecord, block base.Blknum, blocks []base.Blknum) error {
	// logger.Progress((block%13) == 0, fmt.Sprintf("Updating timestamps %-04d of %-04d (%-04d remaining)"+spaces,
	logger.Info(fmt.Sprintf("Updating timestamps %-04d of %-04d (%-04d remaining)"+spaces,
		block,
		blocks[len(blocks)-1],
		blocks[len(blocks)-1]-block,
	))
	return binary.Write(fp, binary.LittleEndian, ts)
}
