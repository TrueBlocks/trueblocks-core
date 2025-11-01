package names

import (
	"path/filepath"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

func CustomWriteNames(chain string, dryRun bool) (err error) {
	database := DatabaseCustom
	if dryRun {
		database = DatabaseDryRun
	}

	customNamesMutex.Lock()
	defer customNamesMutex.Unlock()

	return writeDatabase(
		chain,
		database,
		customNames,
	)
}

func RegularWriteNames(chain string, dryRun bool) (err error) {
	database := DatabaseRegular
	if dryRun {
		database = DatabaseDryRun
	}

	regularNamesMutex.Lock()
	defer regularNamesMutex.Unlock()

	return writeDatabase(
		chain,
		database,
		regularNames,
	)
}

func writeDatabase(chain string, database DatabaseType, names map[base.Address]types.Name) error {
	namesPath := getDatabasePath(chain, database)
	tmpPath := filepath.Join(config.PathToCache(chain), "tmp")

	// openDatabaseForEdit truncates the file when it opens. We make
	// a backup so we can restore it on an error if we need to.
	backup, err := file.MakeBackup(tmpPath, namesPath)
	if err != nil {
		return err
	}

	db, err := openDatabaseForEdit(chain, database)
	if err != nil {
		// The backup will replace the now truncated file.
		return err
	}

	if database != DatabaseDryRun {
		if err = file.Lock(db); err != nil {
			return err
		}
		defer func() {
			_ = file.Unlock(db)
		}()
	}

	defer func() {
		_ = db.Close()
		// If the backup exists, it will replace the now truncated file.
		backup.Restore()
	}()

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

	err = writer.Error()
	if err == nil {
		// Everything went okay, so we can remove the backup.
		backup.Clear()
	}
	return err
}
