// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"encoding/json"
	"os"
	"path"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

// EXISTING_CODE

type Destination struct {
	Source   string   `json:"source"`
	Term     string   `json:"term"`
	TermType DestType `json:"termType"`
	Url      string   `json:"url"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Destination) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Destination) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"term":     s.Term,
		"termType": s.TermType,
		"url":      s.Url,
		"source":   s.Source,
	}
	order = []string{
		"term",
		"termType",
		"url",
		"source",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Destination) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
type DestType string

const (
	DestinationNone     DestType = "none"
	DestinationAddress  DestType = "address"
	DestinationName     DestType = "name"
	DestinationEnsName  DestType = "ensName"
	DestinationTx       DestType = "tx"
	DestinationBlock    DestType = "block"
	DestinationFourByte DestType = "4byte"
)

func (t DestType) String() string {
	switch t {
	case DestinationNone:
		return "DestinationNone"
	case DestinationAddress:
		return "DestinationAddress"
	case DestinationName:
		return "DestinationName"
	case DestinationEnsName:
		return "DestinationEnsName"
	case DestinationTx:
		return "DestinationTx"
	case DestinationBlock:
		return "DestinationBlock"
	case DestinationFourByte:
		return "DestinationFourByte"
	default:
		return "Invalid DestType"
	}
}

func NewDestination(term string, termType DestType) Destination {
	return Destination{
		Term:     term,
		TermType: termType,
	}
}

func NewDestinationEx(exp Destination, url, source string) Destination {
	ret := exp
	ret.Url = url
	ret.Source = source
	return ret
}

func (s *Destination) Resolve(chain string, google, dalle, local bool) []Destination {
	ret := []Destination{}
	if google {
		var query = "https://www.google.com/search?q=[{TERM}]"
		query = strings.Replace(query, "[{TERM}]", s.Term, -1)
		var exclusions = []string{
			"etherscan", "etherchain", "bloxy", "bitquery", "ethplorer", "tokenview", "anyblocks", "explorer",
		}
		for _, ex := range exclusions {
			query += ("+-" + ex)
		}
		ret = append(ret, NewDestinationEx(*s, query, "google"))
	}

	if dalle {
		var query = "https://dalledress.io/dalle/[{SERIES}]/[{TERM}]"
		query = strings.ReplaceAll(query, "[{TERM}]", s.Term)
		if series, ok := os.LookupEnv("TB_DALLE_SERIES"); ok {
			query = strings.ReplaceAll(query, "[{SERIES}]", series)
		} else {
			query = strings.ReplaceAll(query, "[{SERIES}]", "simple")
		}
		ret = append(ret, NewDestinationEx(*s, query, "dalle"))
	}

	if len(ret) > 0 {
		return ret
	}

	if s.TermType == DestinationFourByte {
		var query = "https://www.4byte.directory/signatures/?bytes4_signature=[{TERM}]"
		query = strings.Replace(query, "[{TERM}]", s.Term, -1)
		ret = append(ret, NewDestinationEx(*s, query, "url"))
	}

	if s.TermType == DestinationEnsName {
		var query = "https://app.ens.domains/name/[{TERM}]/details"
		query = strings.Replace(query, "[{TERM}]", s.Term, -1)
		ret = append(ret, NewDestinationEx(*s, query, "url"))
	}

	url := config.GetChain(chain).RemoteExplorer
	query := ""
	switch s.TermType {
	case DestinationNone:
		// do nothing
	case DestinationTx:
		query = path.Join("tx", s.Term)
	case DestinationBlock:
		query = path.Join("block", s.Term)
	case DestinationAddress:
		fallthrough
	default:
		query = path.Join("address", s.Term)
	}

	if local {
		url = config.GetChain(chain).LocalExplorer
		query = strings.Replace(query, "tx/", "explorer/transactions/", -1)
		query = strings.Replace(query, "block/", "explorer/blocks/", -1)
	}

	ret = append(ret, NewDestinationEx(*s, url+query, "url"))
	return ret
}

// EXISTING_CODE
