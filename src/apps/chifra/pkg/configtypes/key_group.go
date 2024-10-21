package configtypes

type KeyGroup struct {
	License string `toml:"license,omitempty"`
	ApiKey  string `toml:"apiKey"`
	Secret  string `toml:"secret,omitempty"`
	Jwt     string `toml:"jwt,omitempty"`
}
