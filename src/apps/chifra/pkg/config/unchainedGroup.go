// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/ethereum/go-ethereum/common/hexutil"
	"github.com/ethereum/go-ethereum/crypto"
)

type unchainedGroup struct {
	Comment            string `toml:"comment"`
	Specification      string `toml:"specification,omitempty"`
	PreferredPublisher string `toml:"preferredPublisher,omitempty"`
	SmartContract      string `toml:"smartContract,omitempty"`
	SpecVersion        string `toml:"specVersion,omitempty"`
}

func GetUnchained() unchainedGroup {
	return GetRootConfig().Unchained
}

var headerTag = ""

// HeaderTag is inserted into each chunk and each bloom filter as it is produced (or with
// chifra chunks --tag. It is the keccak256 of the SpecVersion string.
func HeaderTag(a ...interface{}) string {
	vers := GetUnchained().SpecVersion
	if len(a) > 0 {
		vers = a[0].(string)
		if version.IsValidVersion(vers) {
			cfg := GetRootConfig()
			cfg.Unchained.SpecVersion = vers
			minVersion := version.NewVersion(DesiredConfigVersion)
			configFile := PathToConfigFile()
			_ = cfg.writeFile(configFile, minVersion)
			headerTag = hexutil.Encode(crypto.Keccak256([]byte(vers)))
		} else {
			logger.Fatal("Implementation error: HeaderTag was called with an invalid version")
			return ""
		}
	}

	if version.IsValidVersion(vers) {
		if headerTag == "" {
			headerTag = hexutil.Encode(crypto.Keccak256([]byte(vers)))
		}
		return headerTag
	}
	logger.Fatal("Implementation error: HeaderTag was called with an invalid version")
	return ""
}

// Specification      = "QmUou7zX2g2tY58LP1A2GyP5RF9nbJsoxKTp299ah3svgb"                     // IPFS hash of the specification for the Unchained Index
// ReadHashName_V2    = "manifestHashMap"                                                    // V2: The name of the function to read the hash
// address_V2         = "0x0c316b7042b419d07d343f2f4f5bd54ff731183d"                         // V2: The address of the current version of the Unchained Index
// preferredPublisher = "0xf503017d7baf7fbc0fff7492b751025c6a78179b"                         // V2: Us
