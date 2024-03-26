// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/proto"
)

// EXISTING_CODE

type RawName struct {
	Address    string `json:"address"`
	Decimals   string `json:"decimals"`
	Deleted    string `json:"deleted"`
	IsContract string `json:"isContract"`
	IsCustom   string `json:"isCustom"`
	IsErc20    string `json:"isErc20"`
	IsErc721   string `json:"isErc721"`
	IsPrefund  string `json:"isPrefund"`
	Name       string `json:"name"`
	Petname    string `json:"petname"`
	Source     string `json:"source"`
	Symbol     string `json:"symbol"`
	Tags       string `json:"tags"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleName struct {
	Address    base.Address `json:"address"`
	Decimals   uint64       `json:"decimals"`
	Deleted    bool         `json:"deleted,omitempty"`
	IsContract bool         `json:"isContract,omitempty"`
	IsCustom   bool         `json:"isCustom,omitempty"`
	IsErc20    bool         `json:"isErc20,omitempty"`
	IsErc721   bool         `json:"isErc721,omitempty"`
	IsPrefund  bool         `json:"isPrefund,omitempty"`
	Name       string       `json:"name"`
	Petname    string       `json:"petname"`
	Source     string       `json:"source"`
	Symbol     string       `json:"symbol"`
	Tags       string       `json:"tags"`
	raw        *RawName     `json:"-"`
	// EXISTING_CODE
	Prefund base.Wei `json:"prefund,omitempty"`
	// EXISTING_CODE
}

func (s *SimpleName) Raw() *RawName {
	return s.raw
}

func (s *SimpleName) SetRaw(raw *RawName) {
	s.raw = raw
}

func (s *SimpleName) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	if extraOptions["single"] == "tags" || extraOptions["single"] == "address" {
		if extraOptions["single"] == "tags" {
			model["tags"] = s.Tags
		} else {
			model["address"] = s.Address.Hex()
		}
		order = append(order, extraOptions["single"].(string))
		return Model{
			Data:  model,
			Order: order,
		}
	}

	model = map[string]interface{}{
		"address":  s.Address,
		"decimals": s.Decimals,
		"name":     s.Name,
		"petname":  s.Petname,
		"source":   s.Source,
		"symbol":   s.Symbol,
		"tags":     s.Tags,
	}

	order = []string{
		"tags",
		"address",
		"name",
		"symbol",
		"source",
		"decimals",
		"petname",
	}

	if len(s.Address.Bytes()) > 0 && s.Address != base.ZeroAddr {
		model["address"] = strings.ToLower(s.Address.String())
	}

	isExpanded := extraOptions["expand"] == true
	isPrefund := extraOptions["prefund"] == true
	if !isExpanded && !isPrefund {
		x := []string{}
		for _, v := range order {
			if v != "source" {
				x = append(x, v)
			}
		}
		order = x
		delete(model, "source")
	}

	if isExpanded && isPrefund {
		model["prefund"] = s.Prefund.String()
		order = append(order, "prefund")
	}

	if format == "json" {
		if len(s.Symbol) == 0 {
			x := []string{}
			for _, v := range order {
				if v != "symbol" {
					x = append(x, v)
				}
			}
			order = x
			delete(model, "symbol")
		} else {
			model["symbol"] = s.Symbol
		}
		if s.Decimals == 0 {
			x := []string{}
			for _, v := range order {
				if v != "decimals" {
					x = append(x, v)
				}
			}
			order = x
			delete(model, "decimals")
		}
		if len(s.Source) > 0 {
			model["source"] = s.Source
			order = append(order, "source")
		}
		if s.Deleted {
			model["deleted"] = true
			order = append(order, "deleted")
		}
		if s.IsCustom {
			model["isCustom"] = true
			order = append(order, "isCustom")
		}
		if s.IsPrefund {
			model["isPrefund"] = true
			order = append(order, "isPrefund")
		}
		if s.IsContract {
			model["isContract"] = true
			order = append(order, "isContract")
		}
		if s.IsErc20 {
			model["isErc20"] = true
			order = append(order, "isErc20")
		}
		if s.IsErc721 {
			model["isErc721"] = true
			order = append(order, "isErc721")
		}
	} else {
		if len(s.Symbol) == 0 {
			model["symbol"] = ""
		}
		if s.Decimals == 0 {
			model["decimals"] = ""
		}

		if isExpanded {
			model["deleted"] = s.Deleted
			order = append(order, "deleted")
			model["isCustom"] = s.IsCustom
			order = append(order, "isCustom")
			model["isPrefund"] = s.IsPrefund
			order = append(order, "isPrefund")
			model["isContract"] = s.IsContract
			order = append(order, "isContract")
			model["isErc20"] = s.IsErc20
			order = append(order, "isErc20")
			model["isErc721"] = s.IsErc721
			order = append(order, "isErc721")
		}
	}

	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
//

func (s *SimpleName) ToMessage() *proto.Name {
	return &proto.Name{
		Address:    s.Address.Hex(),
		Decimals:   utils.PointerOf(s.Decimals),
		Deleted:    utils.PointerOf(s.Deleted),
		IsContract: utils.PointerOf(s.IsContract),
		IsCustom:   utils.PointerOf(s.IsCustom),
		IsErc20:    utils.PointerOf(s.IsErc20),
		IsErc721:   utils.PointerOf(s.IsErc721),
		IsPrefund:  utils.PointerOf(s.IsPrefund),
		Name:       s.Name,
		Petname:    utils.PointerOf(s.Petname),
		Source:     utils.PointerOf(s.Source),
		Symbol:     utils.PointerOf(s.Symbol),
		Tags:       utils.PointerOf(s.Tags),
	}
}

func (s *SimpleName) Send(stream proto.Names_SearchStreamServer) error {
	return stream.Send(s.ToMessage())
}

func NewNameFromGrpc(gRpcName *proto.Name) *SimpleName {
	return &SimpleName{
		Address:    base.HexToAddress(gRpcName.GetAddress()),
		Decimals:   gRpcName.GetDecimals(),
		Deleted:    gRpcName.GetDeleted(),
		IsContract: gRpcName.GetIsContract(),
		IsCustom:   gRpcName.GetIsCustom(),
		IsErc20:    gRpcName.GetIsErc20(),
		IsErc721:   gRpcName.GetIsErc721(),
		IsPrefund:  gRpcName.GetIsPrefund(),
		Name:       gRpcName.GetName(),
		Petname:    gRpcName.GetPetname(),
		Source:     gRpcName.GetSource(),
		Symbol:     gRpcName.GetSymbol(),
		Tags:       gRpcName.GetTags(),
	}
}

// EXISTING_CODE
