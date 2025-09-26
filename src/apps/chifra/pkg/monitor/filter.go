package monitor

import (
	"encoding/binary"
	"fmt"
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (mon *Monitor) ReadAndFilterAppearances(filt *types.AppearanceFilter, withCount bool) (apps []types.Appearance, cnt int, err error) {
	readAppearances := func(apps *[]types.AppRecord) (err error) {
		if int64(len(*apps)) > mon.Count() {
			err = fmt.Errorf("array is larger than the size of the file (%d,%d)", len(*apps), mon.Count())
			return
		}

		if mon.ReadFp == nil {
			path := mon.Path()
			mon.ReadFp, err = os.OpenFile(path, os.O_RDONLY, 0644)
			if err != nil {
				return
			}
		}

		// Seek past the header to get to the first record
		_, err = mon.ReadFp.Seek(index.AppRecordWidth, io.SeekStart)
		if err != nil {
			return
		}

		err = binary.Read(mon.ReadFp, binary.LittleEndian, apps)
		if err != nil {
			return
		}

		return
	}

	filt.Reset()

	if mon.Count() == 0 {
		mon.Close()
		return nil, 0, nil
	}

	fromDisc := make([]types.AppRecord, mon.Count())
	if err := readAppearances(&fromDisc); err != nil {
		mon.Close()
		return nil, 0, err
	} else if len(fromDisc) == 0 {
		mon.Close()
		return nil, 0, nil
	}
	filt.Sort(fromDisc)

	prev := fromDisc[0]
	apps = make([]types.Appearance, 0, len(fromDisc))
	for _, app := range fromDisc {
		var passes bool
		var finished bool
		if withCount {
			passes, finished = filt.PassesFilter(&app)
		} else {
			passes, finished = filt.PassesRangeFilter(&app)
		}

		if finished {
			return apps, len(apps), nil
		} else if passes {
			if len(apps) == 0 {
				filt.OuterBounds.First = base.Blknum(prev.BlockNumber)
			}
			filt.OuterBounds.Last = base.Blknum(app.BlockNumber + 1)
			s := types.Appearance{
				Address:          mon.Address,
				BlockNumber:      app.BlockNumber,
				TransactionIndex: app.TransactionIndex,
				Timestamp:        base.NOPOSI,
			}
			apps = append(apps, s)
		}
		prev = app
	}
	mon.Close()

	return apps, len(apps), nil
}

// AsSliceOfTransactionMaps reads appearances from the monitor and returns them as batched maps
// ready for transaction processing
func (mon *Monitor) AsSliceOfTransactionMaps(filter *types.AppearanceFilter, reversed bool) ([]map[types.Appearance]*types.Transaction, int, error) {
	apps, cnt, err := mon.ReadAndFilterAppearances(filter, false)
	if err != nil {
		return nil, 0, err
	}

	if cnt == 0 {
		return nil, 0, nil
	}

	sliceOfMaps, _, err := types.AsSliceOfMaps[types.Transaction](apps, reversed)
	if err != nil {
		return nil, 0, err
	}

	return sliceOfMaps, cnt, nil
}
