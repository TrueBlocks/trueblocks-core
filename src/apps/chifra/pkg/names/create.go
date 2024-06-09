package names

import (
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func CreateName(dbType DatabaseType, chain string, name *types.Name) (err error) {
	switch dbType {
	case DatabaseCustom:
		return customCreateName(chain, name)
	case DatabaseRegular:
		return regularCreateName(name)
	default:
		logger.Fatal("should not happen ==> unknown database type")
	}
	return
}

func customCreateName(chain string, name *types.Name) error {
	namesPath := getDatabasePath(chain, DatabaseCustom)
	tmpPath := filepath.Join(config.PathToCache(chain), "tmp")

	// openDatabaseForEdit truncates the file when it opens. We make
	// a backup so we can restore it on an error if we need to.
	backup, err := file.MakeBackup(tmpPath, namesPath)
	if err != nil {
		return err
	}

	db, err := openDatabaseForEdit(chain, DatabaseCustom)
	if err != nil {
		// The backup will replace the now truncated file.
		return err
	}

	defer func() {
		_ = db.Close()
		// If the backup exists, it will replace the now truncated file.
		backup.Restore()
	}()

	name.IsCustom = true
	loadedCustomNamesMutex.Lock()
	defer loadedCustomNamesMutex.Unlock()
	loadedCustomNames[name.Address] = *name
	err = writeCustomNames(db)
	if err == nil {
		// Everything went okay, so we can remove the backup.
		backup.Clear()
	}
	return err
}

func regularCreateName(name *types.Name) (err error) {
	loadedRegularNamesMutex.Lock()
	defer loadedRegularNamesMutex.Unlock()

	name.IsCustom = false
	loadedRegularNames[name.Address] = *name
	return
}
