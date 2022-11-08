package types

import (
	"math/big"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/bykof/gostradamus"
	"github.com/ethereum/go-ethereum/common"
)

type Modeler[Raw RawData] interface {
	Model(showHidden bool, format string) Model
	Raw() *Raw
}

type RawData interface {
	RawReceipt | RawWhenCount | RawNamedBlock
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
	Encoding        string            `json:"encoding,omitempty"`
	Signature       string            `json:"signature,omitempty"`
	Name            string            `json:"name"`
	FunctionType    string            `json:"functionType"`
	AbiSource       string            `json:"abi_source"`
	Anonymous       bool              `json:"anonymous"`
	Constant        bool              `json:"constant"`
	StateMutability string            `json:"stateMutability"`
	Inputs          []SimpleParameter `json:"inputs"`
	Outputs         []SimpleParameter `json:"outputs"`
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
	Address          common.Address  `json:"address"`
	LogIndex         uint64          `json:"logIndex"`
	BlockNumber      uint64          `json:"blockNumber"`
	TransactionIndex uint32          `json:"transactionIndex"`
	Timestamp        uint64          `json:"timestamp,omitempty"`
	Topics           []common.Hash   `json:"topics,omitempty"`
	Data             string          `json:"data,omitempty"`
	CompressedLog    string          `json:"compressedLog,omitempty"`
	ArticulatedLog   *SimpleFunction `json:"-"`
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

type SimpleBlock struct {
	GasLimit      uint64
	GasUsed       uint64
	Hash          common.Hash
	BlockNumber   Blknum
	ParentHash    common.Hash
	Miner         common.Address
	Difficulty    uint64
	Finalized     bool
	Timestamp     time.Time
	BaseFeePerGas Wei
	Transactions  []SimpleTransaction
}

type SimpleTransaction struct {
	Hash                 common.Hash     `json:"hash"`
	BlockHash            common.Hash     `json:"blockHash"`
	BlockNumber          Blknum          `json:"blockNumber"`
	TransactionIndex     uint64          `json:"transactionIndex"`
	Nonce                uint64          `json:"nonce"`
	Timestamp            time.Time       `json:"timestamp"`
	From                 common.Address  `json:"from"`
	To                   common.Address  `json:"to"`
	Value                Wei             `json:"value"`
	ExtraValue1          Wei             `json:"extraValue1"`
	ExtraValue2          Wei             `json:"extraValue2"`
	Gas                  Gas             `json:"gas"`
	GasPrice             Gas             `json:"gasPrice"`
	MaxFeePerGas         Gas             `json:"maxFeePerGas"`
	MaxPriorityFeePerGas Gas             `json:"maxPriorityFeePerGas"`
	Input                string          `json:"input"`
	IsError              bool            `json:"isError"`
	HasToken             bool            `json:"hasToken"`
	Cachebits            uint8           `json:"cachebits"`
	Reserved2            uint8           `json:"reserved2"`
	Receipt              *SimpleReceipt  `json:"receipt"`
	Traces               []SimpleTrace   `json:"traces"`
	ArticulatedTx        *SimpleFunction `json:"articulatedTx"`
}

type SimpleTrace struct {
	BlockHash        common.Hash        `json:"blockHash"`
	BlockNumber      Blknum             `json:"blockNumber"`
	Subtraces        uint64             `json:"subtraces"`
	TraceAddress     []string           `json:"traceAddress"`
	TransactionHash  common.Hash        `json:"transactionHash"`
	TransactionIndex Blknum             `json:"transactionIndex"`
	TraceType        string             `json:"traceType"`
	Error            string             `json:"error"`
	Action           *SimpleTraceAction `json:"action"`
	Result           *SimpleTraceResult `json:"result"`
	ArticulatedTrace *SimpleFunction    `json:"articulatedTrace"`
}

type SimpleTraceAction struct {
	SelfDestructed common.Address `json:"selfDestructed"`
	Balance        Wei            `json:"balance"`
	CallType       string         `json:"callType"`
	From           common.Address `json:"from"`
	Gas            Gas            `json:"gas"`
	Init           string         `json:"init"`
	Input          string         `json:"input"`
	RefundAddress  common.Address `json:"refundAddress"`
	To             common.Address `json:"to"`
	Value          Wei            `json:"value"`
}
type SimpleTraceResult struct {
	NewContract common.Address `json:"newContract"`
	Code        string         `json:"code"`
	GasUsed     Gas            `json:"gasUsed"`
	Output      string         `json:"output"`
}

type SimpleParameter struct {
	ParameterType string            `json:"parameterType"`
	Name          string            `json:"name"`
	StrDefault    string            `json:"strDefault"`
	Value         string            `json:"value"`
	Indexed       bool              `json:"indexed"`
	InternalType  string            `json:"internalType"`
	Components    []SimpleParameter `json:"components"`
	Unused        bool              `json:"unused"`
	IsFlags       uint64            `json:"is_flags"`
}
