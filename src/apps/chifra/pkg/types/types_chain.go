// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import "encoding/json"

// EXISTING_CODE

type Chain struct {
	Chain          string `json:"chain"`
	ChainId        uint64 `json:"chainId"`
	IpfsGateway    string `json:"ipfsGateway"`
	LocalExplorer  string `json:"localExplorer"`
	RemoteExplorer string `json:"remoteExplorer"`
	RpcProvider    string `json:"rpcProvider"`
	Symbol         string `json:"symbol"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Chain) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Chain) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"chain",
		"chainId",
		"ipfsGateway",
		"localExplorer",
		"remoteExplorer",
		"rpcProvider",
		"symbol",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this Chain.
// This excludes any calculated or derived fields.
func (s *Chain) RawMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{
		"chain":          s.Chain,
		"chainId":        s.ChainId,
		"ipfsGateway":    s.IpfsGateway,
		"localExplorer":  s.LocalExplorer,
		"remoteExplorer": s.RemoteExplorer,
		"rpcProvider":    s.RpcProvider,
		"symbol":         s.Symbol,
	}

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this Chain.
// This type has no calculated fields currently.
func (s *Chain) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{}

	return model
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Chain) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
