package types

import (
	"strings"
)

type CmdLineEndpoint struct {
	cmd           *Command
	Num           string `json:"num"`
	Group         string `json:"group"`
	IsVisible     string `json:"is_visible"`
	IsVisibleDocs string `json:"is_visible_docs"`
	ApiGroup      string `json:"api_group"`
	ApiRoute      string `json:"api_route"`
	Tool          string `json:"tool"`
	Summary       string `json:"summary"`
	Usage         string `json:"usage"`
	Capabilities  string `json:"capabilities"`
	Description   string `json:"description"`
}

func (c CmdLineEndpoint) Validate() bool { return len(c.ApiRoute) > 0 }

func ReadCmdEndpoint(cmd *CmdLineEndpoint, data *any) (bool, error) {
	cmd.Description = strings.ReplaceAll(cmd.Description, "&#44;", ",")
	return true, nil
}
