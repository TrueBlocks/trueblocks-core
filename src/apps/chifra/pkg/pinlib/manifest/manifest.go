package manifest

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

type IpfsHash = string

type PinDescriptor struct {
	FileName  string   `json:"fileName"`
	BloomHash IpfsHash `json:"bloomHash"`
	IndexHash IpfsHash `json:"indexHash"`
}

type Manifest struct {
	FileName           string          `json:"fileName"`
	IndexFormat        IpfsHash        `json:"indexFormat"`
	BloomFormat        IpfsHash        `json:"bloomFormat"`
	CommitHash         string          `json:"commitHash"`
	PreviousHash       IpfsHash        `json:"prevHash"`
	NewBlockRange      ManifestRange   `json:"newBlockRange"`
	PreviousBlockRange ManifestRange   `json:"prevBlockRange"`
	NewPins            []PinDescriptor `json:"newPins"`
	PreviousPins       []PinDescriptor `json:"prevPins"`
}

// FromLocalFile loads the manifest saved in ConfigPath
func FromLocalFile() (*Manifest, error) {
	file, err := os.Open(config.GetConfigPath("manifest/manifest.txt"))
	if err != nil {
		return nil, err
	}

	return ReadTabManifest(file)
}
