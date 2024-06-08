package names

import (
	"errors"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func CustomWriteNames(chain string, dryRun bool) (err error) {
	database := DatabaseCustom
	if dryRun {
		database = DatabaseDryRun
	}

	return writeDatabase(
		chain,
		Custom,
		database,
		loadedCustomNames,
	)
}

func RegularWriteNames(chain string, dryRun bool) (err error) {
	database := DatabaseRegular
	if dryRun {
		database = DatabaseDryRun
	}

	return writeDatabase(
		chain,
		Regular,
		database,
		loadedRegularNames,
	)
}

func writeDatabase(chain string, kind Parts, database DatabaseType, names map[base.Address]types.Name) (err error) {
	switch kind {
	case Regular:
		loadedRegularNamesMutex.Lock()
		defer loadedRegularNamesMutex.Unlock()
	case Custom:
		loadedCustomNamesMutex.Lock()
		defer loadedCustomNamesMutex.Unlock()
	default:
		return errors.New("kind should be Regular or Custom")
	}

	db, err := openDatabaseForEdit(chain, database)
	if err != nil {
		return
	}
	defer db.Close()

	if database != DatabaseDryRun {
		if err = file.Lock(db); err != nil {
			return err
		}
		defer func() {
			_ = file.Unlock(db)
		}()
	}

	sorted := make([]types.Name, 0, len(names))
	for _, name := range names {
		sorted = append(sorted, name)
	}
	sort.SliceStable(sorted, func(i, j int) bool {
		return sorted[i].Address.Hex() < sorted[j].Address.Hex()
	})

	writer := NewNameWriter(db)
	for _, name := range sorted {
		if err = writer.Write(&name); err != nil {
			return err
		}
	}
	writer.Flush()

	return writer.Error()
}
