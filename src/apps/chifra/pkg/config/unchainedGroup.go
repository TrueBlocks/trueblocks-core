package config

import (
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

func SpecVersionHex() string {
	return hexutil.Encode(SpecVersionKeccak())
}

func SpecVersionText() string {
	return GetUnchained().SpecVersion
}

func SpecVersionKeccak() []byte {
	return crypto.Keccak256([]byte(GetUnchained().SpecVersion))
}

// Specification      = "QmUou7zX2g2tY58LP1A2GyP5RF9nbJsoxKTp299ah3svgb"                     // IPFS hash of the specification for the Unchained Index
// ReadHashName_V2    = "manifestHashMap"                                                    // V2: The name of the function to read the hash
// address_V2         = "0x0c316b7042b419d07d343f2f4f5bd54ff731183d"                         // V2: The address of the current version of the Unchained Index
// preferredPublisher = "0xf503017d7baf7fbc0fff7492b751025c6a78179b"                         // V2: Us
