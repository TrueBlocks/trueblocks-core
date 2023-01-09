package articulation

import (
	"encoding/json"
	"fmt"
	"os"
	"path"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var knownAbiSubdirectories = []string{
	"known-000", "known-005", "known-010", "known-015",
}

// !!!
// https://pkg.go.dev/github.com/ethereum/go-ethereum/accounts/abi#pkg-overview

type AbiInterfaceMap = map[string]*types.SimpleFunction

// TODO: make sure we really need slice of pointers here
func LoadAbiFromJsonFile(filePath string) (result AbiInterfaceMap, err error) {
	// result = make([]*types.SimpleFunction, 0, )

	file, err := os.OpenFile(filePath, os.O_RDONLY, 0)
	if err != nil {
		return
	}

	var functions []types.SimpleFunction
	decoder := json.NewDecoder(file)
	if err = decoder.Decode(&functions); err != nil {
		return
	}
	result = make(AbiInterfaceMap, len(functions))
	for _, function := range functions {
		function := function
		result[strings.ToLower(function.Encoding)] = &function
	}

	return
}

func LoadAbiFromKnownFile(name string) (result AbiInterfaceMap, err error) {
	var filePath string
	if filePath = findKnownAbi(name); filePath == "" {
		err = fmt.Errorf("known abi file not found: %s", name)
		return
	}
	return LoadAbiFromJsonFile(filePath)
}

func mergeMaps(destination, source AbiInterfaceMap) {
	for key, value := range source {
		destination[key] = value
	}
}

func PreloadTokensAbi() (result AbiInterfaceMap, err error) {
	result, err = LoadAbiFromJsonFile(path.Join(config.GetPathToRootConfig(), "abis", "known-000/erc_00020.json"))
	if err != nil {
		return
	}
	erc721, err := LoadAbiFromJsonFile(path.Join(config.GetPathToRootConfig(), "abis", "known-000/erc_00721.json"))
	if err != nil {
		return
	}
	mergeMaps(result, erc721)
	return
}

func PreloadKnownAbis(tokensOnly bool) (result AbiInterfaceMap, err error) {
	if tokensOnly {
		return PreloadTokensAbi()
	}

	// Check if cache file is fresh
	// Use cache file

	return
}

func findKnownAbi(name string) (filePath string) {
	for _, subdirName := range knownAbiSubdirectories {
		testPath := path.Join(config.GetPathToRootConfig(), "abis", subdirName, name+".json")
		if file.FileExists(testPath) {
			filePath = testPath
			return
		}
	}
	return
}
