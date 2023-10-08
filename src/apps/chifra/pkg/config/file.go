package config

import (
	"os"

	"github.com/BurntSushi/toml"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// readFile reads the toml config file into the given struct
func readFile(inFile string, contents interface{}) error {
	_, err := toml.DecodeFile(inFile, contents)
	return err
}

// writeFile writes the toml config file from the given struct
func (cfg *ConfigFile) writeFile(outFn string, vers version.Version) error {
	cfg.SetVersionStr(vers.String())
	if f, err := os.Create(outFn); err != nil {
		return err
	} else {
		defer f.Close()
		if err := toml.NewEncoder(f).Encode(cfg); err != nil {
			return err
		}
	}
	return nil
}
