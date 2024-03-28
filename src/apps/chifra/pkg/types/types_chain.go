// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE

// EXISTING_CODE

type RawChain struct {
	Chain          string `json:"chain"`
	ChainId        string `json:"chainId"`
	IpfsGateway    string `json:"ipfsGateway"`
	LocalExplorer  string `json:"localExplorer"`
	RemoteExplorer string `json:"remoteExplorer"`
	RpcProvider    string `json:"rpcProvider"`
	Symbol         string `json:"symbol"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleChain struct {
	Chain          string    `json:"chain"`
	ChainId        uint64    `json:"chainId"`
	IpfsGateway    string    `json:"ipfsGateway"`
	LocalExplorer  string    `json:"localExplorer"`
	RemoteExplorer string    `json:"remoteExplorer"`
	RpcProvider    string    `json:"rpcProvider"`
	Symbol         string    `json:"symbol"`
	raw            *RawChain `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleChain) Raw() *RawChain {
	return s.raw
}

func (s *SimpleChain) SetRaw(raw *RawChain) {
	s.raw = raw
}

func (s *SimpleChain) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"chain":          s.Chain,
		"chainId":        s.ChainId,
		"ipfsGateway":    s.IpfsGateway,
		"localExplorer":  s.LocalExplorer,
		"remoteExplorer": s.RemoteExplorer,
		"rpcProvider":    s.RpcProvider,
		"symbol":         s.Symbol,
	}
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

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *SimpleChain) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
