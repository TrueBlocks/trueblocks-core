package pinlib

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/blockRange"

type ipfsHash = string

type PinDescriptor struct {
	FileName  string   `json:"fileName"`
	BloomHash ipfsHash `json:"bloomHash"`
	IndexHash ipfsHash `json:"indexHash"`
}

type Manifest struct {
	FileName           string           `json:"fileName"`
	IndexFormat        ipfsHash         `json:"indexFormat"`
	BloomFormat        ipfsHash         `json:"bloomFormat"`
	CommitHash         string           `json:"commitHash"`
	PreviousHash       ipfsHash         `json:"prevHash"`
	NewBlockRange      blockRange.Range `json:"newBlockRange"`
	PreviousBlockRange blockRange.Range `json:"prevBlockRange"`
	NewPins            []PinDescriptor  `json:"newPins"`
	PreviousPins       []PinDescriptor  `json:"prevPins"`
}
