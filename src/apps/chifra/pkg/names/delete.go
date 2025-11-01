package names

import (
	"fmt"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

func SetDeleted(dbType DatabaseType, chain string, address base.Address, deleted bool) (name *types.Name, err error) {
	switch dbType {
	case DatabaseCustom:
		return customSetDeleted(chain, address, deleted)
	case DatabaseRegular:
		// return regularSetDeleted(chain, address, deleted)
		logger.ShouldNotHappen("SetDeleted is not supported for regular database")
	default:
		logger.ShouldNotHappen("unknown database type")
	}
	return
}

func customSetDeleted(chain string, address base.Address, deleted bool) (*types.Name, error) {
	existing, ok := customNames[address]
	if !ok {
		return nil, fmt.Errorf("no custom name for address %s", address.Hex())
	}

	if existing.Deleted && deleted {
		return nil, fmt.Errorf("name for address %s is already deleted, cannot delete", address.Hex())
	} else if !existing.Deleted && !deleted {
		return nil, fmt.Errorf("name for address %s is not deleted, cannot undelete", address.Hex())
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

	name, err := changeDeleted(db, address, deleted)
	if err == nil {
		// Everything went okay, so we can remove the backup.
		backup.Clear()
	}
	return name, err
}

func changeDeleted(output *os.File, address base.Address, deleted bool) (*types.Name, error) {
	name := customNames[address]
	name.Deleted = deleted
	name.IsCustom = true
	customNamesMutex.Lock()
	defer customNamesMutex.Unlock()
	customNames[name.Address] = name
	return &name, writeCustomNames(output)
}
