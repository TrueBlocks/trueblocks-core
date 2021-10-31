package pinlib

import (
	"os"
	"path"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

type ErrCustomizedPath struct {
	indexPath string
}

func (err *ErrCustomizedPath) Error() string {
	return "customized index path not present"
}

func (err *ErrCustomizedPath) GetIndexPath() string {
	return err.indexPath
}

func EstablishDirectories() error {
	indexPath := config.ReadGlobal().Settings.IndexPath
	subdirectories := []string{
		"blooms",
		"finalized",
		"ripe",
		"staging",
		"unripe",
	}

	_, err := os.Stat(indexPath)
	if err != nil && !os.IsNotExist(err) {
		return err
	}
	// directory doesn't exist and is custom: abort
	if os.IsNotExist(err) && indexPath != config.DefaultIndexPath {
		return &ErrCustomizedPath{
			indexPath: indexPath,
		}
	}
	// directory doesn't exists, but it is the default one: create it
	if os.IsNotExist(err) {
		err = os.Mkdir(indexPath, 0755)

		if err != nil {
			return err
		}
	}

	// make sure all subdirectories exist as well, or create the ones that
	// are missing
	for _, subdirectory := range subdirectories {
		subdirPath := path.Join(indexPath, subdirectory)
		_, err = os.Stat(subdirPath)
		if err != nil && !os.IsNotExist(err) {
			return err
		}

		if os.IsNotExist(err) {
			err = os.Mkdir(subdirPath, 0755)
			if err != nil {
				return err
			}
		}
	}

	return nil
}
