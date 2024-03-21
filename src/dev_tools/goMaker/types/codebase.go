package types

import (
	"encoding/json"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

type CodeBase struct {
	Commands  []Command   `json:"commands"`
	templates TemplateMap `json:"-"`
}

func (c *CodeBase) String() string {
	bytes, _ := json.MarshalIndent(c, "", "    ")
	return string(bytes)
}

func (cb *CodeBase) summary(filter string) string {
	ret := []string{}
	sort.Slice(cb.Commands, func(i, j int) bool {
		return cb.Commands[i].Endpoint.Num < cb.Commands[j].Endpoint.Num
	})
	for _, c := range cb.Commands {
		pad := func(s string, width int) string {
			return s + strings.Repeat(" ", width-len(s))
		}
		if c.Endpoint.Group == filter {
			descr := strings.ToLower(c.Endpoint.Description[0:1])
			descr += strings.TrimSuffix(c.Endpoint.Description[1:], ".")
			ret = append(ret, "    "+pad(c.Endpoint.ApiRoute, 14)+descr)
		}
	}
	return strings.Join(ret, "\n")
}

func (cb *CodeBase) AccountsSummary() string {
	return cb.summary("Accounts")
}

func (cb *CodeBase) ChainDataSummary() string {
	return cb.summary("Chain Data")
}

func (cb *CodeBase) ChainStateSummary() string {
	return cb.summary("Chain State")
}

func (cb *CodeBase) AdminSummary() string {
	return cb.summary("Admin")
}

func (cb *CodeBase) OtherSummary() string {
	return cb.summary("Other")
}

func (cb *CodeBase) Version() string {
	vers := strings.Trim(file.AsciiFileToString("VERSION"), "\n\r")
	return "GHC-TrueBlocks//" + vers + "-release"
}
