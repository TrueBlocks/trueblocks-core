package scrapePkg

import (
	"context"
	"encoding/binary"
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/tslib"
)

// TODO: Protect against overwriting files on disc

func (bm *BlazeManager) WriteTimestamps(ctx context.Context, blocks []base.Blknum) error {
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
		maxBlocks := 2000
		// if an environment variable called TB_BACKFILL_BLOCKS is set, use it
		if val := os.Getenv("TB_BACKFILL_BLOCKS"); val != "" {
			if n := base.MustParseInt64(val); int(n) > maxBlocks {
				maxBlocks = int(n)
			}
		}
		for block := nTimestamps; block < blocks[0] && cnt < maxBlocks; block++ {
			if ctx.Err() != nil {
				// This means the context got cancelled, i.e. we got a SIGINT.
				return nil
			}
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

	if ctx.Err() != nil {
		// This means the context got cancelled, i.e. we got a SIGINT.
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
		if !bm.isHeadless {
			logProgressTs("Updating timestamps ", block, blocks[len(blocks)-1])
		}
		if err := binary.Write(fp, binary.LittleEndian, &ts); err != nil {
			return err
		}

	}

	if !bm.isHeadless {
		logger.Progress(true, fmt.Sprintf("Finished writing timestamps to block %-04d"+spaces,
			blocks[len(blocks)-1],
		))
	}

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
