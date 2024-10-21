package configtypes

type NotifyGroup struct {
	Url    string `json:"url,omitempty" toml:"url"`
	Author string `json:"author,omitempty" toml:"author"`
}
