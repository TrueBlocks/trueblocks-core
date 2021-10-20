package config

import (
	"errors"
	"log"
	"os"
	"os/user"
	"path"
	"runtime"

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

// Returns the path to the directory where the configuration files are
func getConfigPath(fileName string) string {
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

// Opens the file and returns new TOML Decoder
func read(fileName string) *toml.Decoder {
	filePath := getConfigPath(fileName + ".toml")

	file, err := os.Open(filePath)
	if err != nil {
		log.Fatalln("Could not read configuration file:", filePath)
	}

	return toml.NewDecoder(file)
}

// Reads {configName}.toml from the config directory, parses it
// and loads the values into target. Warning: configName should NOT
// have .toml extension (it will be added automatically)
func ReadTo(target interface{}, configName string) {
	reader := read(configName)
	_, err := reader.Decode(&target)
	if err != nil {
		log.Fatalf(`Error while reading configuration for "%s": %s`, configName, err)
	}
}
