package types

import (
	"encoding/hex"
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/ethereum/go-ethereum/common"
)

type Modeler[Raw RawData] interface {
	Model(showHidden bool, format string, extraOptions map[string]any) Model
	Raw() *Raw
}

// TODO: BOGUS - The auto code generation should check that all auto generated fields are included here
type RawData interface {
	RawReceipt | RawWhenCount | RawNamedBlock | RawBlock | RawBlockCount | RawTraceAction |
		RawTraceResult | RawTrace | RawTraceCount | RawFunction | RawParameter | RawAppearance
}

type Model struct {
	Data  map[string]any
	Order []string
}

type IpfsHash string

func (h IpfsHash) String() string {
	return string(h)
}

// Address is a wrapper for go-ethereum's Address type that always
// return lower case hex.
type Address struct {
	common.Address
}

// Hex returns string representation of an address
func (a *Address) Hex() string {
	if a.IsZero() {
		return "0x0"
	}
	// This is 1000 ns/op faster than strings.ToLower
	return "0x" + hex.EncodeToString(a.Bytes())
}

func (a *Address) String() string {
	return a.Hex()
}

func (a Address) Format(s fmt.State, c rune) {
	s.Write([]byte(a.Hex()))
}

func (a Address) MarshalText() ([]byte, error) {
	hex := a.Hex()
	return []byte(hex), nil
}

// SetHex sets the address based on the provided string
func (a *Address) SetHex(hex string) {
	a.Address = common.HexToAddress(hex)
}

// IsZero returns true if an addres is a zero value or 0x0.
func (a *Address) IsZero() bool {
	// go-ethereum initializes Address.Bytes() with
	// length, so the slice if filled with 0s. Comparing
	// strings seem to be the simplest and most efficient
	// way.
	zero := common.HexToAddress("0x0")
	return a.Address.Hex() == zero.Hex()
}

// HexToAddress returns new address with the given string
// as value.
func HexToAddress(hex string) (addr Address) {
	addr.SetHex(hex)
	return
}

func BytesToAddress(b []byte) (addr Address) {
	addr.SetBytes(b)
	return
}

type SimpleTimestamp struct {
	BlockNumber uint64 `json:"blockNumber"`
	Timestamp   int64  `json:"timestamp"`
	Diff        int64  `json:"diff"`
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

type Wei = big.Int
type Gas = uint64
type Blknum = uint64
type Topic = string
