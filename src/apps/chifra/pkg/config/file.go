package config

import (
	"os"

	"github.com/BurntSushi/toml"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

func ReadConfigFile(inFile string, contents interface{}) error {
	_, err := toml.DecodeFile(inFile, contents)
	return err
}

func (cfg *ConfigFile) WriteConfigFile(outFn string) error {
	cfg.SetVersionStr(version.VersionString())
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
