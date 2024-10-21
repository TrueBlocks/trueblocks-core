package configtypes

type KeyGroup struct {
	License string `json:"license" toml:"license,omitempty"`
	ApiKey  string `json:"apiKey" toml:"apiKey"`
	Secret  string `json:"secret" toml:"secret,omitempty"`
	Jwt     string `json:"jwt" toml:"jwt,omitempty"`
}
