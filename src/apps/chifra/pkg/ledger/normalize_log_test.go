package ledger

import (
	"math/big"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func TestNormalizedLog_Standard(t *testing.T) {
	// Standard event with 3 topics.
	logStandard := types.Log{
		Topics: []base.Hash{
			topics.TransferTopic, // event signature
			base.HexToHash("0x0000000000000000000000001111111111111111111111111111111111111111"),
			base.HexToHash("0x0000000000000000000000002222222222222222222222222222222222222222"),
		},
		Data: "0x00000000000000000000000000000000000000000000000000000000000003e8", // value = 1000
	}

	var l Ledger
	normLog, err := l.normalizeLog(&logStandard)
	if err != nil {
		t.Fatalf("unexpected error in standard case: %v", err)
	}

	// Expected normalized topics.
	expectedFrom := "0x0000000000000000000000001111111111111111111111111111111111111111"
	expectedTo := "0x0000000000000000000000002222222222222222222222222222222222222222"

	if !strings.EqualFold(normLog.Topics[1].Hex(), expectedFrom) {
		t.Errorf("expected normalized from topic %s, got %s", expectedFrom, normLog.Topics[1])
	}
	if !strings.EqualFold(normLog.Topics[2].Hex(), expectedTo) {
		t.Errorf("expected normalized to topic %s, got %s", expectedTo, normLog.Topics[2])
	}

	// Verify the data encodes the value 1000.
	expectedValue := big.NewInt(1000)
	val, err := decodeUint256(normLog.Data)
	if err != nil {
		t.Fatalf("failed to decode normalized data: %v", err)
	}
	if val.Cmp(expectedValue) != 0 {
		t.Errorf("expected value %s, got %s", expectedValue.String(), val.String())
	}
}

func TestNormalizedLog_TwoTopics(t *testing.T) {
	// Two-topic variant: topics[1] is the "from" address;
	// the data field holds two 32-byte values: first the "to" address and then the value.
	fromTopic := "0x000000000000000000000000aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

	// Create the "to" address.
	// Use an unpadded 40-character address, which our helper will normalize.
	toRaw := "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
	// Expected normalized to topic: padded to 32 bytes.
	expectedTo := "0x000000000000000000000000" + toRaw

	// Encode the value (5000) as a 32-byte hex string.
	value := big.NewInt(5000)
	valueHex := encodeUint256(value)

	// Build the data: concatenation of the padded "to" address and the value.
	data := "0x" + strings.TrimPrefix(expectedTo, "0x") + strings.TrimPrefix(valueHex, "0x")

	logTwoTopics := types.Log{
		Topics: []base.Hash{
			topics.TransferTopic, // event signature
			base.HexToHash(fromTopic),
		},
		Data: data,
	}

	var l Ledger
	normLog, err := l.normalizeLog(&logTwoTopics)
	if err != nil {
		t.Fatalf("unexpected error in two-topic case: %v", err)
	}

	if !strings.EqualFold(normLog.Topics[1].Hex(), fromTopic) {
		t.Errorf("expected normalized from topic %s, got %s", fromTopic, normLog.Topics[1])
	}
	if !strings.EqualFold(normLog.Topics[2].Hex(), expectedTo) {
		t.Errorf("expected normalized to topic %s, got %s", expectedTo, normLog.Topics[2])
	}

	expectedValue := value
	val, err := decodeUint256(normLog.Data)
	if err != nil {
		t.Fatalf("failed to decode normalized data: %v", err)
	}
	if val.Cmp(expectedValue) != 0 {
		t.Errorf("expected value %s, got %s", expectedValue.String(), val.String())
	}
}

func TestNormalizedLog_NonStandard(t *testing.T) {
	// Non-standard variant: No addresses are indexed; all parameters are in the data.
	// Data encodes three 32-byte values: from, to, and value.
	fromRaw := "cccccccccccccccccccccccccccccccccccccccc"
	toRaw := "dddddddddddddddddddddddddddddddddddddddd"
	expectedFrom := "0x000000000000000000000000" + fromRaw
	expectedTo := "0x000000000000000000000000" + toRaw
	value := big.NewInt(9999)
	valueHex := encodeUint256(value)

	data := "0x" +
		"000000000000000000000000" + fromRaw +
		"000000000000000000000000" + toRaw +
		strings.TrimPrefix(valueHex, "0x")

	logNonStandard := types.Log{
		Topics: []base.Hash{
			topics.TransferTopic,
		},
		Data: data,
	}

	var l Ledger
	normLog, err := l.normalizeLog(&logNonStandard)
	if err != nil {
		t.Fatalf("unexpected error in non-standard case: %v", err)
	}

	if !strings.EqualFold(normLog.Topics[1].Hex(), expectedFrom) {
		t.Errorf("expected normalized from topic %s, got %s", expectedFrom, normLog.Topics[1])
	}
	if !strings.EqualFold(normLog.Topics[2].Hex(), expectedTo) {
		t.Errorf("expected normalized to topic %s, got %s", expectedTo, normLog.Topics[2])
	}

	expectedValue := value
	val, err := decodeUint256(normLog.Data)
	if err != nil {
		t.Fatalf("failed to decode normalized data: %v", err)
	}
	if val.Cmp(expectedValue) != 0 {
		t.Errorf("expected value %s, got %s", expectedValue.String(), val.String())
	}
}

func TestNormalizedLog_UnrecognizedFormat(t *testing.T) {
	// Unrecognized format: extra topics.
	logInvalid := types.Log{
		Topics: []base.Hash{
			topics.TransferTopic,
			base.HexToHash("0x000000000000000000000000aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
			base.HexToHash("0x000000000000000000000000bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
			base.HexToHash("0x000000000000000000000000cccccccccccccccccccccccccccccccccccccccc"), // extra topic
		},
		Data: "0x00000000000000000000000000000000000000000000000000000000000003e8",
	}

	var l Ledger
	_, err := l.normalizeLog(&logInvalid)
	if err == nil {
		t.Fatal("expected error for unrecognized event format, got nil")
	}
}
