package types

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/bykof/gostradamus"
	"github.com/ethereum/go-ethereum/common"
)

type Modeler[Raw RawData] interface {
	Model(showHidden bool, format string) Model
	Raw() *Raw
}

type RawData interface {
	RawReceipt | RawTimestampCount
}

type Model struct {
	Data  map[string]interface{}
	Order []string
}

type IpfsHash string

func (h IpfsHash) String() string {
	return string(h)
}

type SimpleTimestamp struct {
	BlockNumber uint64 `json:"blockNumber"`
	TimeStamp   uint64 `json:"timestamp"`
	Diff        uint64 `json:"diff"`
}

type SimpleNamedBlock struct {
	BlockNumber uint64 `json:"blockNumber"`
	TimeStamp   uint64 `json:"timestamp"`
	Date        string `json:"date"`
	Name        string `json:"name,omitempty"`
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

type SimpleFunction struct {
	Encoding  string `json:"encoding,omitempty"`
	Signature string `json:"signature,omitempty"`
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

type SimpleLog struct {
	Address          common.Address `json:"address"`
	LogIndex         uint32         `json:"logIndex"`
	BlockNumber      uint64         `json:"blockNumber"`
	TransactionIndex uint32         `json:"transactionIndex"`
	Timestamp        uint64         `json:"timestamp,omitempty"`
	Topics           []common.Hash  `json:"topics,omitempty"`
	Data             string         `json:"data,omitempty"`
	CompressedLog    string         `json:"compressedLog,omitempty"`
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
