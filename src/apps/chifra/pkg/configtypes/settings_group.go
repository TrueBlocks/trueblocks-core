package configtypes

type SettingsGroup struct {
	CachePath      string      `toml:"cachePath" comment:"The location of the per chain caches"`
	IndexPath      string      `toml:"indexPath" comment:"The location of the per chain unchained indexes"`
	DefaultChain   string      `toml:"defaultChain" comment:"The default chain to use if none is provided"`
	DefaultGateway string      `toml:"defaultGateway,omitempty"`
	Notify         NotifyGroup `toml:"notify"`
}
