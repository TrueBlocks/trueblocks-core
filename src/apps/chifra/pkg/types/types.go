package types

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/ethereum/go-ethereum/common"
)

type NamedBlock struct {
	BlockNumber uint64 `json:"blockNumber"`
	TimeStamp   uint64 `json:"timestamp"`
	Date        string `json:"date"`
	Name        string `json:"name,omitempty"`
}

type CleanReport struct {
	Addr     string `json:"addr"`
	SizeThen uint32 `json:"sizeThen"`
	SizeNow  uint32 `json:"sizeNow"`
	Dups     uint32 `json:"dupsRemoved"`
}

type CheckReport struct {
	Reason     string   `json:"reason"`
	VisitedCnt uint32   `json:"nVisited"`
	CheckedCnt uint32   `json:"nChecked,omitempty"`
	SkippedCnt uint32   `json:"nSkipped,omitempty"`
	PassedCnt  uint32   `json:"nPassed,omitempty"`
	FailedCnt  uint32   `json:"nFailed,omitempty"`
	ErrorStrs  []string `json:"errorStrs,omitempty"`
}

type SimpleAppearance struct {
	Address          string `json:"address"`
	BlockNumber      uint32 `json:"blockNumber"`
	TransactionIndex uint32 `json:"transactionIndex"`
}

type Function struct {
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
	Version   string        `json:"version"`
	Chain     string        `json:"chain"`
	Schemas   string        `json:"schemas"`
	Databases string        `json:"databases"`
	Chunks    []SimpleChunk `json:"chunks"`
}

type SimpleChunk struct {
	FileName  string `json:"fileName"`
	BloomHash string `json:"bloomHash"`
	IndexHash string `json:"indexHash"`
}

type SimpleBloom struct {
	Range     cache.FileRange `json:"range"`
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

type SimpleIndexAddress struct {
	Address string `json:"address"`
	Offset  uint32 `json:"offset"`
	Count   uint32 `json:"count"`
}

type SimpleIndexAppearance struct {
	BlockNumber      uint32 `json:"blockNumber"`
	TransactionIndex uint32 `json:"transactionIndex"`
}

type SimpleIndexAddressBelongs struct {
	Address string                  `json:"address"`
	Offset  uint32                  `json:"offset"`
	Count   uint32                  `json:"count"`
	Apps    []SimpleIndexAppearance `json:"apps"`
}
