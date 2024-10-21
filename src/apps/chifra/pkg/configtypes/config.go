package configtypes

import (
	"bytes"
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/pelletier/go-toml/v2"
)

type Config struct {
	Version   VersionGroup          `json:"version" toml:"version"`
	Settings  SettingsGroup         `json:"settings" toml:"settings"`
	Keys      map[string]KeyGroup   `json:"keys" toml:"keys"`
	Pinning   PinningGroup          `json:"pinning" toml:"pinning"`
	Unchained UnchainedGroup        `json:"unchained" toml:"unchained,omitempty" comment:"Do not edit these values unless instructed to do so."`
	Chains    map[string]ChainGroup `json:"chains" toml:"chains"`
}

// WriteFile writes the toml config file from the given struct
func (cfg *Config) WriteFile(outFn string, vers version.Version) error {
	cfg.Version.Current = vers.String()
	f, err := os.OpenFile(outFn, os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0666)
	if err != nil {
		return err
	}
	defer f.Close()

	return write(f, cfg)
}

func write(w io.Writer, cfg *Config) (err error) {
	// koanf doesn't keep key order nor comments when unmarshalling,
	// so we will use TOML package directly. We use the same TOML
	// package as koanf.
	var buf bytes.Buffer
	enc := toml.NewEncoder(&buf)
	enc.SetIndentTables(true)
	if err = enc.Encode(cfg); err != nil {
		return
	}
	_, err = w.Write(buf.Bytes())
	return
}
