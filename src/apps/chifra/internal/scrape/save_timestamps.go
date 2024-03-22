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

	if blocks[len(blocks)-1] < nTimestamps {
		// We already have all of these timestamps, leave early
		return nil

	} else if blocks[0] > nTimestamps {
		// we need to catch up (for example, the user truncated the timestamps file while debugging)
		// don't get more than maxBlocks at a time
		cnt := 0
		maxBlocks := 1000
		for block := nTimestamps; block < blocks[0] && cnt < maxBlocks; block++ {
			ts := tslib.TimestampRecord{
				Bn: uint32(block),
				Ts: uint32(bm.opts.Conn.GetBlockTimestamp(block)),
			}
			msg := fmt.Sprintf("Backfilling timestamps (%d-%d) at ", cnt, maxBlocks)
			logProgressTs(msg, block, blocks[len(blocks)-1])
			if err := binary.Write(fp, binary.LittleEndian, &ts); err != nil {
				return err
			}
			cnt++
		}
		// we must return early here, otherwise there will be skipped records
		return nil
	}

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
		logProgressTs("Updating timestamps ", block, blocks[len(blocks)-1])
		if err := binary.Write(fp, binary.LittleEndian, &ts); err != nil {
			return err
		}

	}

	logger.Progress(true, fmt.Sprintf("Finished writing timestamps to block %-04d"+spaces,
		blocks[len(blocks)-1],
	))

	return nil
}

func logProgressTs(msgIn string, cur, total base.Blknum) {
	frequency := base.Blknum(13)
	left := total - cur
	msg := fmt.Sprintf("%s%-04d of %-04d (%-04d remaining)"+spaces,
		msgIn,
		cur,
		total,
		left,
	)
	logger.Progress((cur%frequency) == 0, msg)
}
