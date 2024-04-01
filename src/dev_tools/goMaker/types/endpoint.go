package types

import (
	"encoding/json"
	"strings"
)

// Endpoint is a struct that represents a single endpoint in the API. It is
// attached to the assoicated Command structure.
type Endpoint struct {
	Num           int      `json:"num" csv:"num"`
	Group         string   `json:"group" csv:"group"`
	IsVisible     bool     `json:"is_visible" csv:"is_visible"`
	IsVisibleDocs bool     `json:"is_visible_docs" csv:"is_visible_docs"`
	Folder        string   `json:"folder" csv:"folder"`
	ApiRoute      string   `json:"api_route" csv:"api_route"`
	Tool          string   `json:"tool" csv:"tool"`
	Summary       string   `json:"summary" csv:"summary"`
	Usage         string   `json:"usage" csv:"usage"`
	Capabilities  string   `json:"capabilities" csv:"capabilities"`
	Description   string   `json:"description" csv:"description"`
	cmdPtr        *Command `json:"-" csv:"-"`
}

func (c Endpoint) String() string {
	bytes, _ := json.MarshalIndent(c, "", "    ")
	return string(bytes)
}

func (c Endpoint) Validate() bool {
	return true
}

func readCmdEndpoint(ep *Endpoint, data *any) (bool, error) {
	ep.Description = strings.ReplaceAll(ep.Description, "&#44;", ",")
	ep.Group = strings.Trim(ep.Group, wss)
	ep.Folder = strings.Trim(ep.Folder, wss)
	ep.ApiRoute = strings.Trim(ep.ApiRoute, wss)
	ep.Tool = strings.Trim(ep.Tool, wss)
	ep.Summary = strings.Trim(ep.Summary, wss)
	ep.Usage = strings.Trim(ep.Usage, wss)
	ep.Capabilities = strings.Trim(ep.Capabilities, wss)
	ep.Description = strings.Trim(ep.Description, wss)
	return true, nil
}
