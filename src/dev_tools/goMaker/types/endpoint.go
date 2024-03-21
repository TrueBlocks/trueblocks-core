package types

// CmdLineEndpoint carries some interesting information about a command such
// capabilities and the usage string. Similar to the command's description option.
type CmdLineEndpoint struct {
	Num           string   `json:"num"`
	Group         string   `json:"group"`
	IsVisible     string   `json:"is_visible"`
	IsVisibleDocs string   `json:"is_visible_docs"`
	ApiGroup      string   `json:"api_group"`
	ApiRoute      string   `json:"api_route"`
	Tool          string   `json:"tool"`
	Summary       string   `json:"summary"`
	Usage         string   `json:"usage"`
	Capabilities  string   `json:"capabilities"`
	Description   string   `json:"description"`
	cmd           *Command `json:"-"`
}

func (c CmdLineEndpoint) Validate() bool {
	return len(c.ApiRoute) > 0
}
