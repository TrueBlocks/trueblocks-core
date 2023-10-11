package names

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func RemoveName(dbType DatabaseType, chain string, address base.Address) (name *types.SimpleName, err error) {
	switch dbType {
	case DatabaseCustom:
		return customRemoveName(chain, address)
	case DatabaseRegular:
		// return regularRemoveName(chain, address)
		logger.Fatal("remove is not supported for regular database")
	default:
		logger.Fatal("unknown database type")
	}
	return
}

func customRemoveName(chain string, address base.Address) (name *types.SimpleName, err error) {
	db, err := openDatabaseFile(chain, DatabaseCustom, os.O_WRONLY|os.O_TRUNC)
	if err != nil {
		return
	}
	defer db.Close()

	return removeIfExists(db, address)
}
