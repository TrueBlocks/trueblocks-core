package config

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/knadh/koanf/parsers/toml"
	"github.com/knadh/koanf/providers/structs"
	"github.com/knadh/koanf/v2"
)

// writeFile writes the toml config file from the given struct
func (cfg *ConfigFile) writeFile(outFn string, vers version.Version) error {
	cfg.Version.Current = vers.String()
	f, err := os.OpenFile(outFn, os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0666)
	if err != nil {
		return err
	}
	defer f.Close()

	config := koanf.New(".")
	if err := config.Load(structs.Provider(cfg, ""), nil); err != nil {
		return fmt.Errorf("write: loading config: %w", err)
	}
	content, err := config.Marshal(toml.Parser())
	if err != nil {
		return err
	}
	_, err = f.Write(content)
	return err
}
