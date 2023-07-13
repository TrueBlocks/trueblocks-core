package base

import (
	"encoding/hex"
	"fmt"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/binary"
	"github.com/ethereum/go-ethereum/common"
)

type IpfsHash string

func (h IpfsHash) String() string {
	return string(h)
}

type Hash struct {
	common.Hash
}

func (h *Hash) Hex() string {
	return "0x" + hex.EncodeToString(h.Bytes())
}

func (h *Hash) String() string {
	return h.Hex()
}

func (h Hash) Format(s fmt.State, c rune) {
	s.Write([]byte(h.Hex()))
}

func (h Hash) MarshalText() ([]byte, error) {
	hex := h.Hex()
	return []byte(hex), nil
}

func (h *Hash) SetHex(hex string) {
	h.Hash = common.HexToHash(hex)
}

func (h *Hash) IsZero() bool {
	zero := common.HexToHash("0x0")
	return h.Hash.Hex() == zero.Hex()
}

func (h *Hash) UnmarshalCache(version uint64, reader io.Reader) error {
	var value [32]byte
	if err := binary.ReadValue(reader, &value, version); err != nil {
		return err
	}
	h.SetBytes(value[:])
	return nil
}

func HexToHash(hex string) (hash Hash) {
	hash.SetHex(hex)
	return
}

func BytesToHash(b []byte) (hash Hash) {
	hash.SetBytes(b)
	return
}
