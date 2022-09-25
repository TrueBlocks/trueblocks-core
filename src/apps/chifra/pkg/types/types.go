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
	RawReceipt
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
	BlockNumber      uint64         `json:"blockNumber,omitempty"`
	TransactionIndex uint32         `json:"transactionIndex,omitempty"`
	Timestamp        uint64         `json:"timestamp,omitempty"`
	Topics           []common.Hash  `json:"topics"`
	Data             string         `json:"data,omitempty"`
	CompressedLog    string         `json:"compressedLog,omitempty"`
}

type SimpleReceipt struct {
	BlockHash         common.Hash    `json:"blockHash"`
	BlockNumber       uint64         `json:"blockNumber"`
	ContractAddress   common.Address `json:"contractAddress,omitempty"`
	CumulativeGasUsed string         `json:"cumulativeGasUsed"`
	From              common.Address `json:"from"`
	GasUsed           uint64         `json:"gasUsed"`
	EffectiveGasPrice uint64         `json:"effectiveGasPrice"`
	Logs              []SimpleLog    `json:"logs,omitempty"`
	// LogsBloom         string         `json:"-"`
	// Root              string         `json:"-"`
	Status           uint64         `json:"status"`
	IsError          bool           `json:"isError,omitempty"`
	To               common.Address `json:"to,omitempty"`
	TransactionHash  common.Hash    `json:"hash"`
	TransactionIndex uint64         `json:"transactionIndex"`
	raw              *RawReceipt
}

func (r *SimpleReceipt) Raw() *RawReceipt {
	return r.raw
}

func (r *SimpleReceipt) SetRaw(rawReceipt RawReceipt) {
	r.raw = &rawReceipt
}

// To support custom format: just execute template on the output of Model
func (r *SimpleReceipt) Model(showHidden bool, format string) Model {
	model := map[string]interface{}{
		"blockNumber":      r.BlockNumber,
		"transactionIndex": r.TransactionIndex,
		"hash":             r.TransactionHash,
		"gasUsed":          r.GasUsed,
		"status":           r.Status,
		"isError":          r.IsError,
	}
	order := []string{
		"blockNumber",
		"transactionIndex",
		"hash",
		"gasUsed",
		"status",
		"isError",
	}

	if showHidden && format == "json" {
		model["blockHash"] = r.BlockHash
		model["contractAddress"] = r.ContractAddress
		model["cumulativeGasUsed"] = r.CumulativeGasUsed
		model["from"] = r.From
		model["effectiveGasPrice"] = r.EffectiveGasPrice
		model["logs"] = r.Logs
		model["to"] = r.To

		order = append(order, []string{
			"blockHash",
			"contractAddress",
			"cumulativeGasUsed",
			"from",
			"effectiveGasPrice",
			"logs",
			"to",
		}...)
	}

	if format == "api" {
		model["logs"] = r.Logs
		order = append(order, []string{"logs"}...)
	}

	return Model{
		Data:  model,
		Order: order,
	}
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
