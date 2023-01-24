package types

import (
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/bykof/gostradamus"
	"github.com/ethereum/go-ethereum/common"
)

type Modeler[Raw RawData] interface {
	Model(showHidden bool, format string, extraOptions map[string]any) Model
	Raw() *Raw
}

// TODO: BOGUS - The auto code generation should check that all auto generated fields are included here
type RawData interface {
	RawReceipt | RawWhenCount | RawNamedBlock | RawBlock | RawBlockCount | RawTraceAction | RawTraceResult | RawTrace | RawTraceCount | RawFunction | RawParameter
}

type Model struct {
	Data  map[string]any
	Order []string
}

type IpfsHash string

func (h IpfsHash) String() string {
	return string(h)
}

type SimpleTimestamp struct {
	BlockNumber uint64 `json:"blockNumber"`
	Timestamp   uint64 `json:"timestamp"`
	Diff        uint64 `json:"diff"`
}

type SimpleAppearance struct {
	Address          string `json:"address"`
	BlockNumber      uint32 `json:"blockNumber"`
	TransactionIndex uint32 `json:"transactionIndex"`
}

type VerboseAppearance struct {
	Address          string               `json:"address"`
	BlockNumber      uint32               `json:"blockNumber"`
	TransactionIndex uint32               `json:"transactionIndex"`
	Timestamp        uint64               `json:"timestamp"`
	Date             gostradamus.DateTime `json:"date"`
}

type SimpleMonitor struct {
	Address     string `json:"address"`
	NRecords    int    `json:"nRecords"`
	FileSize    int64  `json:"fileSize"`
	LastScanned uint32 `json:"lastScanned"`
}

type SimpleManifest struct {
	Version string              `json:"version"`
	Chain   string              `json:"chain"`
	Schemas IpfsHash            `json:"schemas"`
	Chunks  []SimpleChunkRecord `json:"chunks"`
}

type SimpleChunkRecord struct {
	Range     string   `json:"range,omitempty"`
	BloomHash IpfsHash `json:"bloomHash,omitempty"`
	BloomSize int64    `json:"bloomSize,omitempty"`
	IndexHash IpfsHash `json:"indexHash,omitempty"`
	IndexSize int64    `json:"indexSize,omitempty"`
}

type SimpleBloom struct {
	Range     paths.FileRange `json:"range"`
	Magic     uint16          `json:"magic"`
	Hash      common.Hash     `json:"hash"`
	Count     uint32          `json:"nBlooms"`
	NInserted uint64          `json:"nInserted"`
	Size      int64           `json:"size"`
	Width     uint64          `json:"byteWidth"`
}

type SimpleIndex struct {
	Range           paths.FileRange `json:"range"`
	Magic           uint32          `json:"magic"`
	Hash            common.Hash     `json:"hash"`
	AddressCount    uint32          `json:"nAddresses"`
	AppearanceCount uint32          `json:"nAppearances"`
	Size            int64           `json:"fileSize"`
}

type SimpleIndexAppearance struct {
	BlockNumber      uint32 `json:"blockNumber"`
	TransactionIndex uint32 `json:"transactionIndex"`
}

type SimpleIndexAddress struct {
	Address string `json:"address"`
	Range   string `json:"range"`
	Offset  uint32 `json:"offset"`
	Count   uint32 `json:"count"`
}

type SimpleIndexAddressBelongs struct {
	Address string                  `json:"address"`
	Offset  uint32                  `json:"offset"`
	Count   uint32                  `json:"count"`
	Apps    []SimpleIndexAppearance `json:"apps"`
}

type SimpleName struct {
	Tags     string `json:"tags"`
	Address  string `json:"address"`
	Name     string `json:"name"`
	Symbol   string `json:"symbol,omitempty"`
	Source   string `json:"source,omitempty"`
	Decimals string `json:"decimals,omitempty"`
	Petname  string `json:"petname,omitempty"`
}

type Wei = big.Int
type Gas = uint64
type Blknum = uint64
type Topic = string
