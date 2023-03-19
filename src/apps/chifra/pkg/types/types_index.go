package types

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/ethereum/go-ethereum/common"
)

type RawIndex interface{}

type SimpleIndex struct {
	Range           paths.FileRange `json:"range"`
	Magic           uint32          `json:"magic"`
	Hash            common.Hash     `json:"hash"`
	AddressCount    uint32          `json:"nAddresses"`
	AppearanceCount uint32          `json:"nAppearances"`
	Size            int64           `json:"fileSize"`
}

func (s *SimpleIndex) Write(p []byte) (n int, err error) {
	return 0, nil
}

func (s *SimpleIndex) Read(p []byte) (n int, err error) {
	return 0, nil
}
