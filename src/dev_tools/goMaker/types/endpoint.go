package types

import (
	"encoding/json"
	"strings"
)

type Endpoint struct {
	Num           string   `json:"num" csv:"num"`
	Group         string   `json:"group" csv:"group"`
	IsVisible     string   `json:"is_visible" csv:"is_visible"`
	IsVisibleDocs string   `json:"is_visible_docs" csv:"is_visible_docs"`
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
	return true, nil
}
