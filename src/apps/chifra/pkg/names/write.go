package names

import (
	"errors"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func WriteNames(dbType DatabaseType, chain string, overrideDest DatabaseType) (err error) {
	switch dbType {
	case DatabaseCustom:
		return customWriteNames(chain, overrideDest)
	case DatabaseRegular:
		return regularWriteNames(chain, overrideDest)
	default:
		logger.Fatal("unknown database type")
	}
	return
}

func customWriteNames(chain string, overrideDest DatabaseType) (err error) {
	database := DatabaseCustom
	if overrideDest != "" {
		database = overrideDest
	}

	return writeDatabase(
		chain,
		Custom,
		database,
		loadedCustomNames,
	)
}

func regularWriteNames(chain string, overrideDest DatabaseType) (err error) {
	database := DatabaseRegular
	if overrideDest != "" {
		database = overrideDest
	}

	return writeDatabase(
		chain,
		Regular,
		database,
		loadedRegularNames,
	)
}

func writeDatabase(chain string, kind Parts, database DatabaseType, names map[base.Address]types.SimpleName) (err error) {
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

	db, err := openDatabaseFile(chain, database, os.O_RDWR|os.O_TRUNC)
	if err != nil {
		return
	}
	defer db.Close()

	sorted := make([]types.SimpleName, 0, len(names))
	for _, name := range names {
		sorted = append(sorted, name)
	}
	sort.SliceStable(sorted, func(i, j int) bool {
		return sorted[i].Address.Hex() < sorted[j].Address.Hex()
	})

	if database != DatabaseDryRun {
		if err = file.Lock(db); err != nil {
			return err
		}
		defer func() {
			_ = file.Unlock(db)
		}()
	}

	writer := NewNameWriter(db)
	for _, name := range sorted {
		if err = writer.Write(&name); err != nil {
			return err
		}
	}
	writer.Flush()

	return writer.Error()
}
