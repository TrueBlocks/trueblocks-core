// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"github.com/ethereum/go-ethereum/crypto"
)

type unchainedGroup struct {
	Manifest           string `toml:"manifest"`
	PreferredPublisher string `toml:"preferredPublisher"`
	UnchainedIndex     string `toml:"unchainedIndex"`
	HeaderMagic        string `toml:"headerMagic"`
	Specification      string `toml:"specification"`
}

func GetUnchained() unchainedGroup {
	return GetRootConfig().Unchained
}

// HeaderTag is inserted into each chunk and each bloom filter as it is produced (or with
// chifra chunks --tag. It is the keccak256 of the Manifest string.
func HeaderTag() []byte {
	return crypto.Keccak256([]byte(GetUnchained().Manifest))
}
