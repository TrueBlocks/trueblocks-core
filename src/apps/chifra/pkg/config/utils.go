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
package config

import (
	"errors"
	"log"
	"os"
	"os/user"
	"path"
	"runtime"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/spf13/viper"
)

var ErrOldFolder = errors.New(`
      You must complete the migration process before proceeding:

          https://github.com/TrueBlocks/trueblocks-core/tree/master/src/other/migrations
`)

var osToPath = map[string]string{
	"linux":  ".local/share/trueblocks",
	"darwin": "Library/Application Support/TrueBlocks",
}

// GetConfigPath returns the path to the directory where the configuration files are
func GetConfigPath(fileName string) string {
	userOs := runtime.GOOS
	if userOs == "windows" {
		log.Fatalln("Windows is not supported")
	}

	user, err := user.Current()
	if err != nil {
		log.Fatalln("could not read user home directory")
	}

	homeDir := user.HomeDir

	// Check if user has migrated to a new config
	if _, err := os.Stat(path.Join(homeDir, ".quickBlocks")); err == nil {
		// Abort if they hasn't
		log.Fatalf(ErrOldFolder.Error())
	}

	confDir := osToPath[userOs]
	if len(confDir) == 0 {
		log.Fatalln("Unsupported operating system")
	}

	return path.Join(homeDir, confDir, fileName)
}

// MustReadConfig calls v's ReadInConfig and fills values in the
// given targetStruct. Any error will result in a call to logger.Fatal
func MustReadConfig(v *viper.Viper, targetStruct interface{}, fileRequired bool) {
	v.AddConfigPath(GetConfigPath(""))
	v.SetEnvPrefix("TB")
	v.AutomaticEnv()
	v.SetEnvKeyReplacer(strings.NewReplacer(".", "_"))

	err := v.ReadInConfig()
	if err != nil {
		_, ok := err.(viper.ConfigFileNotFoundError)
		// We only require some files to be present
		if !ok || (ok && fileRequired) {
			logger.Fatal(err)
		}
	}

	err = v.Unmarshal(targetStruct)
	if err != nil {
		logger.Fatal(err)
	}
}
