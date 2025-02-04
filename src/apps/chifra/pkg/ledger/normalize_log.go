package ledger

import (
	"encoding/hex"
	"errors"
	"math/big"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// standardSignature is the known ERC-20 Transfer event signature hash.
// (Typically, it's the Keccak256 hash of "Transfer(address,address,uint256)")
const standardSignature = "0x0000000ddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a21f"

// normalizeLog inspects an input log and returns a new log that is normalized.
// The normalized log always has three topics (signature, from, to) and a data field that only contains the value.
func (l *Ledger) normalizeLog(log *types.Log) (types.Log, error) {
	var from, to *string
	var value *big.Int
	var err error
	var lenTopics = len(log.Topics)
	var topic = log.Topics[0].Hex()

	// Case 1: Standard event with 3 topics and matching signature.
	if lenTopics == 3 && strings.EqualFold(topic, standardSignature) {
		from, err = normalizeTopicAddress(log.Topics[1].Hex())
		if err != nil {
			return types.Log{}, err
		}
		to, err = normalizeTopicAddress(log.Topics[2].Hex())
		if err != nil {
			return types.Log{}, err
		}
		value, err = decodeUint256(log.Data)
		if err != nil {
			return types.Log{}, err
		}
	} else if lenTopics == 2 {
		// Case 2: Two-topic variant:
		// Assume topics[1] is the indexed 'from'.
		from, err = normalizeTopicAddress(log.Topics[1].Hex())
		if err != nil {
			return types.Log{}, err
		}
		// In this case, the data should contain two 32-byte values:
		// first 32 bytes: padded 'to' address,
		// next 32 bytes: uint256 value.
		toDecoded, valueDecoded, err := decodeTwoValues(log.Data)
		if err != nil {
			return types.Log{}, err
		}
		to, err = normalizeTopicAddress(toDecoded)
		if err != nil {
			return types.Log{}, err
		}
		value = valueDecoded
	} else if lenTopics == 1 || lenTopics == 0 {
		// Case 3: Non-standard variant:
		// No indexed addresses; all parameters are in data.
		fromDecoded, toDecoded, valueDecoded, err := decodeThreeValues(log.Data)
		if err != nil {
			return types.Log{}, err
		}
		from, err = normalizeTopicAddress(fromDecoded)
		if err != nil {
			return types.Log{}, err
		}
		to, err = normalizeTopicAddress(toDecoded)
		if err != nil {
			return types.Log{}, err
		}
		value = valueDecoded
	} else {
		return types.Log{}, errors.New("unrecognized Transfer event format")
	}

	// Build a new normalized log: three topics and a data field containing only the value.
	newLog := types.Log{
		Topics: []base.Hash{
			base.HexToHash(standardSignature), // normalized event signature
			base.HexToHash(*from),             // normalized "from" address
			base.HexToHash(*to),               // normalized "to" address
		},
		Data: encodeUint256(value),
	}
	return newLog, nil
}

// normalizeTopicAddress converts a topic (or decoded address string) into a normalized 32-byte padded address.
// It assumes the input is a hex string that represents a 20-byte address (with or without the "0x" prefix).
func normalizeTopicAddress(input string) (*string, error) {
	// Remove any "0x" prefix.
	s := strings.TrimPrefix(input, "0x")
	// For a padded address, we expect 64 hex characters.
	if len(s) == 64 {
		// We want to verify that the last 40 characters represent the actual address.
		addr := s[24:]
		// Rebuild the normalized topic value.
		normalized := "0x" + "000000000000000000000000" + addr
		return &normalized, nil
	}
	// If the input is 40 hex characters, assume it's an unpadded address.
	if len(s) == 40 {
		normalized := "0x" + "000000000000000000000000" + s
		return &normalized, nil
	}
	return nil, errors.New("invalid address length in topic")
}

// decodeUint256 decodes a 32-byte (64 hex characters) uint256 value from the beginning of the data.
func decodeUint256(data string) (*big.Int, error) {
	data = strings.TrimPrefix(data, "0x")
	if len(data) < 64 {
		return nil, errors.New("data length too short for uint256")
	}
	bytes, err := hex.DecodeString(data[:64])
	if err != nil {
		return nil, err
	}
	value := new(big.Int).SetBytes(bytes)
	return value, nil
}

// encodeUint256 encodes a *big.Int into a 32-byte hex string with "0x" prefix.
func encodeUint256(value *big.Int) string {
	bytes := value.Bytes()
	padded := make([]byte, 32)
	copy(padded[32-len(bytes):], bytes)
	return "0x" + hex.EncodeToString(padded)
}

// decodeTwoValues decodes a data payload assumed to contain two 32-byte values:
// first for a padded address and second for a uint256 value.
func decodeTwoValues(data string) (string, *big.Int, error) {
	data = strings.TrimPrefix(data, "0x")
	if len(data) < 128 {
		return "", nil, errors.New("data length too short for two values")
	}
	// Extract the first 32 bytes for the 'to' address.
	toPart := "0x" + data[:64]
	// Next 32 bytes for the value.
	valueBytes, err := hex.DecodeString(data[64:128])
	if err != nil {
		return "", nil, err
	}
	value := new(big.Int).SetBytes(valueBytes)
	return toPart, value, nil
}

// decodeThreeValues decodes a data payload assumed to contain three 32-byte values:
// from, to, and value.
func decodeThreeValues(data string) (string, string, *big.Int, error) {
	data = strings.TrimPrefix(data, "0x")
	if len(data) < 192 {
		return "", "", nil, errors.New("data length too short for three values")
	}
	fromPart := "0x" + data[:64]
	toPart := "0x" + data[64:128]
	valueBytes, err := hex.DecodeString(data[128:192])
	if err != nil {
		return "", "", nil, err
	}
	value := new(big.Int).SetBytes(valueBytes)
	return fromPart, toPart, value, nil
}
