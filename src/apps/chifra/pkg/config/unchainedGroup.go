package config

import (
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/history"
	"github.com/ethereum/go-ethereum/crypto"
)

type unchainedGroup struct {
	PreferredPublisher string `toml:"preferredPublisher,omitempty" comment:"The default publisher of the index if none other is provided"`
	SmartContract      string `toml:"smartContract,omitempty" comment:"The address of the current version of the Unchained Index"`
}

func GetUnchained() unchainedGroup {
	return GetRootConfig().Unchained
}

func HeaderHash(version string) []byte {
	return crypto.Keccak256([]byte(version))
}

func GetPublisher(value string) string {
	if value == "" {
		value = GetUnchained().PreferredPublisher
		if value == "" {
			value = "publisher.unchainedindex.eth"
		}
	}
	return value
}

// VersionTags allows us to go from the version bytes found in the chunks to a version string
var VersionTags = map[string]string{
	"0x81ae14ba68e372bc9bd4a295b844abd8e72b1de10fcd706e624647701d911da1": "trueblocks-core@v0.40.0",
	"0x6fc0c6dd027719f456c1e50a329f6157767325aa937411fa6e7be9359d9e0046": "trueblocks-core@v2.0.0-release",
}

// SpecTags allows us to go from a version string to an IPFS hash pointing to the spec
var SpecTags = map[string]string{
	"trueblocks-core@v0.40.0":        "QmUou7zX2g2tY58LP1A2GyP5RF9nbJsoxKTp299ah3svgb",
	"trueblocks-core@v2.0.0-release": "QmUyyU8wKW57c3CuwphhMdZb2QA5bsjt9vVfTE6LcBKmE9",
}

func KnownVersionTag(tag string) bool {
	for _, v := range VersionTags {
		vShort := strings.Replace(v, "trueblocks-core@", "", -1)
		if v == tag || vShort == tag {
			return true
		}
	}
	return false
}

var m sync.Mutex

func ExpectedVersion() string {
	if headerVersion == "" {
		m.Lock()
		historyFile := PathToRootConfig() + "unchained.txt"
		headerVersion = history.FromHistory(historyFile, "headerVersion")
		if headerVersion == "" {
			headerVersion = "trueblocks-core@v2.0.0-release"
		}
		m.Unlock()
	}
	return headerVersion
}

func SetExpectedVersion(version string) {
	m.Lock()
	historyFile := PathToRootConfig() + "unchained.txt"
	_ = history.ToHistory(historyFile, "headerVersion", version)
	headerVersion = version
	m.Unlock()
}

var headerVersion = ""
