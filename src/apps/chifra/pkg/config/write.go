package config

import (
	"os"

	"github.com/BurntSushi/toml"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// writeFile writes the toml config file from the given struct
func (cfg *ConfigFile) writeFile(outFn string, vers version.Version) error {
	cfg.Version.Current = vers.String()
	if f, err := os.OpenFile(outFn, os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0666); err != nil {
		return err
	} else {
		defer f.Close()
		if err := toml.NewEncoder(f).Encode(cfg); err != nil {
			return err
		}
	}
	return nil
}

func ChangeSetting(group, item, value string, writeOut bool) error {
	// changed := false
	// cfg := GetRootConfig()

	// switch group {
	// case "unchained":
	// 	switch item {
	// 	case "specVersion":
	// 		changed = true
	// 		cfg.Unchained.SpecVersion = value
	// 	}
	// case "history":
	// 	switch item {
	// 	case "init":
	// 		changed = true
	// 		cfg.History.Init = value
	// 	}
	// }

	// if writeOut && changed {
	// 	minVersion := version.NewVersion(minVersionStr)
	// 	configFile := PathToConfigFile()
	// 	return cfg.writeFile(configFile, minVersion)
	// }

	return nil
}
