package configtypes

import (
	"bytes"
	"encoding/json"
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/version"
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

func (s *Config) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func NewConfig(cachePath, indexPath, defaultIpfs string) Config {
	ret := defaultConfig
	ret.Settings.CachePath = cachePath
	ret.Settings.IndexPath = indexPath
	ret.Pinning.GatewayUrl = defaultIpfs
	return ret
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

var defaultConfig = Config{
	Settings: SettingsGroup{
		// The location of the per chain caches
		CachePath: "",
		// The location of the per chain unchained indexes
		IndexPath: "",
		// The default chain to use if none is provided
		DefaultChain: "mainnet",
	},
	Pinning: PinningGroup{
		// The pinning gateway to query when downloading the unchained index
		GatewayUrl: "",
		// The local endpoint for the IPFS daemon
		LocalPinUrl: "http://localhost:5001",
		// The remote endpoint for pinning on Pinata
		RemotePinUrl: "https://api.pinata.cloud/pinning/pinFileToIPFS",
	},
	Unchained: UnchainedGroup{
		// The default publisher of the index if none other is provided
		PreferredPublisher: "publisher.unchainedindex.eth",
		// V2: The address of the current version of the Unchained Index
		SmartContract: "0x0c316b7042b419d07d343f2f4f5bd54ff731183d",
	},
}
