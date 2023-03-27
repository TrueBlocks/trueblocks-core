// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"io"
)

// EXISTING_CODE

type RawChain struct {
	ApiProvider    string `json:"apiProvider"`
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
	ApiProvider    string    `json:"apiProvider"`
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

func (s *SimpleChain) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleChain) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleChain) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
