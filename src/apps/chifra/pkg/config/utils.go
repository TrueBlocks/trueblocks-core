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

	"github.com/BurntSushi/toml"
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

// readToml opens the file and returns new TOML Decoder
func readToml(fileName string) *toml.Decoder {
	fileNameWithExt := fileName
	if !strings.Contains(fileNameWithExt, ".toml") {
		fileNameWithExt += ".toml"
	}

	filePath := GetConfigPath(fileNameWithExt)

	file, err := os.Open(filePath)
	if err != nil {
		log.Fatalln("Could not read configuration file:", filePath)
	}

	return toml.NewDecoder(file)
}

// ReadTo reads {configName}.toml from the config directory, parses it
// and loads the values into target.
func ReadTo(target interface{}, configName string) {
	reader := readToml(configName)
	_, err := reader.Decode(target)
	if err != nil {
		log.Fatalf(`Error while reading configuration for "%s": %s`, configName, err)
	}
}
