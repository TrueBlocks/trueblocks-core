package base

import (
	"encoding/hex"
	"errors"
	"fmt"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/ethereum/go-ethereum/common"
)

// Hash is a wrapper for go-ethereum's Hash type that always
// return lower case hex.
type Hash struct {
	common.Hash
}

// Hex returns string representation of an hash. Unlike addresses, we do not shorten zero hashes.
func (h *Hash) Hex() string {
	// if h.IsZero() {
	// 	return "0x0"
	// }
	return bytesToHashString(h.Hash.Bytes())
}

func (h *Hash) String() string {
	return h.Hex()
}

// Format is used by Stringer don't remove
func (h Hash) Format(s fmt.State, c rune) {
	s.Write([]byte(h.Hex()))
}

// MarshalText is used by Stringer don't remove
func (h Hash) MarshalText() ([]byte, error) {
	return []byte(h.Hex()), nil
}

func (h *Hash) UnmarshalJSON(data []byte) error {
	if string(data) == "\"0x0\"" {
		return nil
	}
	return h.Hash.UnmarshalJSON(data)
}

// SetHex sets the hash based on the provided string
func (h *Hash) SetHex(hexStr string) {
	h.Hash = common.HexToHash(hexStr)
}

// IsZero returns true if an hash is a zero value or 0x0.
func (h *Hash) IsZero() bool {
	v := bytesToHashString(h.Hash.Bytes())
	return v == "0x0000000000000000000000000000000000000000000000000000000000000000"
}

func (h *Hash) UnmarshalCache(version uint64, reader io.Reader) error {
	var value [32]byte
	if err := cache.ReadValue(reader, &value, version); err != nil {
		return err
	}
	h.SetBytes(value[:])
	return nil
}

func (h *Hash) Common() common.Hash {
	return common.BytesToHash(h.Bytes())
}

func (h *Hash) SetCommon(c *common.Hash) Hash {
	return BytesToHash(c.Bytes())
}

// HexToHash returns new hash with the given string
// as value.
func HexToHash(hex string) (hash Hash) {
	hash.SetHex(hex)
	return
}

func BytesToHash(b []byte) (hash Hash) {
	hash.SetBytes(b)
	return
}

func bytesToHashString(hashBytes []byte) string {
	return "0x" + hex.EncodeToString(hashBytes)
}

func IsValidHex(typ string, val string, nBytes int) (bool, error) {
	if _, err := ValidHex(val, nBytes); err != nil {
		if errors.Is(err, ErrInvalidLength) {
			//lint:ignore ST1005 sorry
			return false, fmt.Errorf("The %s option (%s) must be %d bytes long.", typ, val, nBytes)
		} else if errors.Is(err, ErrInvalidHex) {
			//lint:ignore ST1005 sorry
			return false, fmt.Errorf("The %s option (%s) must be hex.", typ, val)
		} else if errors.Is(err, ErrNoLeading0x) {
			//lint:ignore ST1005 sorry
			return false, fmt.Errorf("The %s option (%s) must start with '0x'.", typ, val)
		}
		return false, err
	}
	return true, nil
}
