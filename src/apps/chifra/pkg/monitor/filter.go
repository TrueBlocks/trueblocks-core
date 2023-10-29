package monitor

import (
	"encoding/binary"
	"fmt"
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (mon *Monitor) ReadAndFilterAppearances(filt *filter.AppearanceFilter) (apps []types.SimpleAppearance, cnt int, err error) {
	readAppearances := func(apps *[]index.AppearanceRecord) (err error) {
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

	fromDisc := make([]index.AppearanceRecord, mon.Count())
	if err := readAppearances(&fromDisc); err != nil {
		mon.Close()
		return nil, 0, err
	} else if len(fromDisc) == 0 {
		mon.Close()
		return nil, 0, nil
	}
	filt.Sort(fromDisc)

	prev := fromDisc[0]
	apps = make([]types.SimpleAppearance, 0, len(fromDisc))
	for _, app := range fromDisc {
		app := app
		if app.TransactionId == uint32(types.Withdrawal) {
			continue
		}

		if passes, finished := filt.BlockRangeFilter(&app); finished {
			return apps, len(apps), nil
		} else if passes {
			if len(apps) == 0 {
				filt.OuterBounds.First = uint64(prev.BlockNumber)
			}
			filt.OuterBounds.Last = uint64(app.BlockNumber + 1)
			s := types.SimpleAppearance{
				Address:          mon.Address,
				BlockNumber:      uint32(app.BlockNumber),
				TransactionIndex: uint32(app.TransactionId),
				Timestamp:        utils.NOPOSI,
			}
			apps = append(apps, s)
		}
		prev = app
	}
	mon.Close()

	return apps, len(apps), nil
}
