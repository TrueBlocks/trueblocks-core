package types

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/bykof/gostradamus"
	"github.com/ethereum/go-ethereum/common"
)

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
	Range     cache.FileRange `json:"range"`
	Magic     uint16          `json:"magic"`
	Hash      common.Hash     `json:"hash"`
	Count     uint32          `json:"nBlooms"`
	NInserted uint64          `json:"nInserted"`
	Size      int64           `json:"size"`
	Width     uint64          `json:"byteWidth"`
}

type SimpleIndex struct {
	Range           cache.FileRange `json:"range"`
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
	Address          string   `json:"address"`
	LogIndex         uint32   `json:"logIndex"`
	BlockNumber      uint32   `json:"blockNumber,omitempty"`
	TransactionIndex uint32   `json:"transactionIndex,omitempty"`
	Timestamp        uint64   `json:"timestamp,omitempty"`
	Topics           []string `json:"topics"`
	Data             string   `json:"data,omitempty"`
	CompressedLog    string   `json:"compressedLog,omitempty"`
}

type SimpleReceipt struct {
	BlockHash         common.Hash    `json:"blockHash"`
	BlockNumber       uint64         `json:"blockNumber"`
	ContractAddress   string         `json:"contractAddress,omitempty"`
	CumulativeGasUsed string         `json:"cumulativeGasUsed"`
	From              common.Address `json:"from"`
	GasUsed           uint64         `json:"gasUsed"`
	EffectiveGasPrice uint64         `json:"effectiveGasPrice"`
	Logs              []SimpleLog    `json:"logs,omitempty"`
	// LogsBloom         string         `json:"-"`
	// Root              string         `json:"-"`
	Status           *uint32     `json:"status"`
	IsError          bool        `json:"isError,omitempty"`
	To               string      `json:"to,omitempty"`
	TransactionHash  common.Hash `json:"hash"`
	TransactionIndex uint64      `json:"transactionIndex"`
}

type SimpleName struct {
	Tags        string `json:"tags"`
	Address     string `json:"address"`
	Name        string `json:"name"`
	Symbol      string `json:"symbol,omitempty"`
	Source      string `json:"source,omitempty"`
	Decimals    string `json:"decimals,omitempty"`
	Description string `json:"description,omitempty"`
}
