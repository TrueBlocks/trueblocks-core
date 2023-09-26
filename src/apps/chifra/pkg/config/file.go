package config

import (
	"os"

	"github.com/BurntSushi/toml"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

func ReadConfigFile(inFile string, config interface{}) error {
	_, err := toml.DecodeFile(inFile, config)
	return err
}

func WriteConfigFile(outFn string, config *ConfigFile) error {
	config.Version.Current = version.VersionString()
	if f, err := os.Create(outFn); err != nil {
		return err
	} else {
		defer f.Close()
		if err := toml.NewEncoder(f).Encode(config); err != nil {
			return err
		}
	}
	return nil
}
