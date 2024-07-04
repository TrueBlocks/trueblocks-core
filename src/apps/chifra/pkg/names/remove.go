package names

import (
	"fmt"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func RemoveName(dbType DatabaseType, chain string, address base.Address) (name *types.Name, err error) {
	switch dbType {
	case DatabaseCustom:
		return customRemoveName(chain, address)
	case DatabaseRegular:
		// return regularRemoveName(chain, address)
		logger.Fatal("should not happen ==> remove is not supported for regular database")
	default:
		logger.Fatal("should not happen ==> unknown database type")
	}
	return
}

func customRemoveName(chain string, address base.Address) (*types.Name, error) {
	name, exists := customNames[address]
	if !exists {
		return nil, fmt.Errorf("cannot remove non-existant custom name for address %s", address.Hex())
	}

	if !name.Deleted {
		return nil, fmt.Errorf("cannot remove non-deleted custom name for address %s", address.Hex())
	}

	namesPath := getDatabasePath(chain, DatabaseCustom)
	tmpPath := filepath.Join(config.PathToCache(chain), "tmp")

	// openDatabaseForEdit truncates the file when it opens. We make
	// a backup so we can restore it on an error if we need to.
	backup, err := file.MakeBackup(tmpPath, namesPath)
	if err != nil {
		return nil, err
	}

	db, err := openDatabaseForEdit(chain, DatabaseCustom)
	if err != nil {
		// The backup will replace the now truncated file.
		return nil, err
	}

	defer func() {
		_ = db.Close()
		// If the backup exists, it will replace the now truncated file.
		backup.Restore()
	}()

	customNamesMutex.Lock()
	defer customNamesMutex.Unlock()
	delete(customNames, address)
	err = writeCustomNames(db)
	if err == nil {
		// Everything went okay, so we can remove the backup.
		backup.Clear()
	}
	return &name, err
}
