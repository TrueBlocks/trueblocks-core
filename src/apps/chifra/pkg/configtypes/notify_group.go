package configtypes

type NotifyGroup struct {
	Url    string `toml:"url" json:"url,omitempty"`
	Author string `toml:"author" json:"author,omitempty"`
}
