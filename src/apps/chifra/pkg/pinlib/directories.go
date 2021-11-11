/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
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

// EstablishIndexFolders sets up directories required by UnchainedIndex.
// If the parent directory is the default one and missing, it will be created.
// If the parent directory is a custom one and missing, an error will be reported.
// If the parent directory exists, any missing subdirectory will be created.
func EstablishIndexFolders() error {
	indexPath := config.ReadTrueBlocks().Settings.IndexPath
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
