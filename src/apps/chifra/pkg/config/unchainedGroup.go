// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
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
func HeaderTag(vers string) []byte {
	if vers == "" {
		logger.Fatal("Implementation error: HeaderTag called with empty version string")
	}

	if !version.IsValidVersion(vers) {
		logger.Fatal("Implementation error: HeaderTag called with invalid version string")
	}

	return crypto.Keccak256([]byte(vers))
}

// Specification   = "QmUou7zX2g2tY58LP1A2GyP5RF9nbJsoxKTp299ah3svgb"                     // IPFS hash of the specification for the Unchained Index
// HeaderMagicHash = "0x81ae14ba68e372bc9bd4a295b844abd8e72b1de10fcd706e624647701d911da1" // V2: Internal hash for the index chunks. The keccek256 of the manifest version
// ReadHashName_V2 = "manifestHashMap"                                                    // V2: The name of the function to read the hash
// address_V2         = "0x0c316b7042b419d07d343f2f4f5bd54ff731183d" // V2: The address of the current version of the Unchained Index
// preferredPublisher = "0xf503017d7baf7fbc0fff7492b751025c6a78179b" // V2: Us
