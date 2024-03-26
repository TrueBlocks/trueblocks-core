package types

import "strings"

// CmdLineEndpoint carries some interesting information about a command such
// capabilities and the usage string. Similar to the command's description option.
type CmdLineEndpoint struct {
	Num           string   `json:"num" csv:"num"`
	Group         string   `json:"group" csv:"group"`
	IsVisible     string   `json:"is_visible" csv:"is_visible"`
	IsVisibleDocs string   `json:"is_visible_docs" csv:"is_visible_docs"`
	ApiGroup      string   `json:"api_group" csv:"api_group"`
	ApiRoute      string   `json:"api_route" csv:"api_route"`
	Tool          string   `json:"tool" csv:"tool"`
	Summary       string   `json:"summary" csv:"summary"`
	Usage         string   `json:"usage" csv:"usage"`
	Capabilities  string   `json:"capabilities" csv:"capabilities"`
	Description   string   `json:"description" csv:"description"`
	cmd           *Command `json:"-" csv:"-"`
}

func (c CmdLineEndpoint) Validate() bool {
	return len(c.ApiRoute) > 0
}

func readCmdEndpoint(cmd *CmdLineEndpoint, data *any) (bool, error) {
	cmd.Description = strings.ReplaceAll(cmd.Description, "&#44;", ",")
	return true, nil
}
