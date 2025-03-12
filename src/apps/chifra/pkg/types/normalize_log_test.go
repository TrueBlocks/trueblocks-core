package types

import (
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
)

func TestNormalizedLog_Standard(t *testing.T) {
	log := Log{
		Topics: []base.Hash{
			topics.TransferTopic,
			base.HexToHash("0x0000000000000000000000001111111111111111111111111111111111111111"),
			base.HexToHash("0x0000000000000000000000002222222222222222222222222222222222222222"),
		},
		Data: "0x00000000000000000000000000000000000000000000000000000000000003e8",
	}

	normLog, err := NormalizeKnownLogs(&log)
	if err != nil {
		t.Fatalf("unexpected error in standard case: %v", err)
	}

	fromTopic := "0x0000000000000000000000001111111111111111111111111111111111111111"
	if !strings.EqualFold(normLog.Topics[1].Hex(), fromTopic) {
		t.Errorf("expected normalized from topic %s, got %s", fromTopic, normLog.Topics[1])
	}

	toTopic := "0x0000000000000000000000002222222222222222222222222222222222222222"
	if !strings.EqualFold(normLog.Topics[2].Hex(), toTopic) {
		t.Errorf("expected normalized to topic %s, got %s", toTopic, normLog.Topics[2])
	}

	expectedVal := base.NewWei(1000)
	gotVal := base.HexToWei(normLog.Data)
	if gotVal.Cmp(expectedVal) != 0 {
		t.Errorf("expected value %s, got %s", expectedVal.Text(10), gotVal.Text(10))
	}
}

func TestNormalizedLog_Approval(t *testing.T) {
	log := Log{
		Topics: []base.Hash{
			topics.ApprovalTopic,
			base.HexToHash("0x0000000000000000000000001111111111111111111111111111111111111111"),
			base.HexToHash("0x0000000000000000000000002222222222222222222222222222222222222222"),
		},
		Data: "0x00000000000000000000000000000000000000000000000000000000000003e8",
	}

	normLog, err := NormalizeKnownLogs(&log)
	if err != nil {
		t.Fatalf("unexpected error in standard case: %v", err)
	}

	fromTopic := "0x0000000000000000000000001111111111111111111111111111111111111111"
	if !strings.EqualFold(normLog.Topics[1].Hex(), fromTopic) {
		t.Errorf("expected normalized from topic %s, got %s", fromTopic, normLog.Topics[1])
	}

	toTopic := "0x0000000000000000000000002222222222222222222222222222222222222222"
	if !strings.EqualFold(normLog.Topics[2].Hex(), toTopic) {
		t.Errorf("expected normalized to topic %s, got %s", toTopic, normLog.Topics[2])
	}

	expectedVal := base.NewWei(1000)
	gotVal := base.HexToWei(normLog.Data)
	if gotVal.Cmp(expectedVal) != 0 {
		t.Errorf("expected value %s, got %s", expectedVal.Text(10), gotVal.Text(10))
	}
}

func TestNormalizedLog_TwoTopics(t *testing.T) {
	fromTopic := "0x000000000000000000000000aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
	toTopic := "0x000000000000000000000000bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"

	value := base.NewWei(5000)
	valueHex, _ := base.WeiToHash(value)
	data := "0x" + strings.TrimPrefix(toTopic, "0x") + strings.TrimPrefix(valueHex, "0x")

	log := Log{
		Topics: []base.Hash{
			topics.TransferTopic,
			base.HexToHash(fromTopic),
		},
		Data: data,
	}

	normLog, err := NormalizeKnownLogs(&log)
	if err != nil {
		t.Fatalf("unexpected error in two-topic case: %v", err)
	}

	if !strings.EqualFold(normLog.Topics[1].Hex(), fromTopic) {
		t.Errorf("expected normalized from topic %s, got %s", fromTopic, normLog.Topics[1])
	}
	if !strings.EqualFold(normLog.Topics[2].Hex(), toTopic) {
		t.Errorf("expected normalized to topic %s, got %s", toTopic, normLog.Topics[2])
	}

	expectedVal := value
	gotVal := base.HexToWei(normLog.Data)
	if gotVal.Cmp(expectedVal) != 0 {
		t.Errorf("expected value %s, got %s", expectedVal.Text(10), gotVal.Text(10))
	}
}

func TestNormalizedLog_NonStandard(t *testing.T) {
	value := base.NewWei(9999)
	data := "0x" +
		"000000000000000000000000cccccccccccccccccccccccccccccccccccccccc" +
		"000000000000000000000000dddddddddddddddddddddddddddddddddddddddd" +
		"000000000000000000000000000000000000000000000000000000000000270f"

	log := Log{
		Topics: []base.Hash{
			topics.TransferTopic,
		},
		Data: data,
	}

	normLog, err := NormalizeKnownLogs(&log)
	if err != nil {
		t.Fatalf("unexpected error in non-standard case: %v", err)
	}

	fromTopic := "0x000000000000000000000000cccccccccccccccccccccccccccccccccccccccc"
	if !strings.EqualFold(normLog.Topics[1].Hex(), fromTopic) {
		t.Errorf("expected normalized from topic %s, got %s", fromTopic, normLog.Topics[1])
	}
	toTopic := "0x000000000000000000000000dddddddddddddddddddddddddddddddddddddddd"
	if !strings.EqualFold(normLog.Topics[2].Hex(), toTopic) {
		t.Errorf("expected normalized to topic %s, got %s", toTopic, normLog.Topics[2])
	}

	expectedVal := value
	gotVal := base.HexToWei(normLog.Data)
	if err != nil {
		t.Fatalf("failed to decode normalized data: %v", err)
	}
	if gotVal.Cmp(expectedVal) != 0 {
		t.Errorf("expected value %s, got %s", expectedVal.Text(10), gotVal.Text(10))
	}
}

func TestNormalizedLog_UnrecognizedFormat(t *testing.T) {
	log := Log{
		Topics: []base.Hash{
			topics.TransferTopic,
			base.HexToHash("0x000000000000000000000000aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
			base.HexToHash("0x000000000000000000000000bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
			base.HexToHash("0x000000000000000000000000cccccccccccccccccccccccccccccccccccccccc"),
		},
		Data: "0x00000000000000000000000000000000000000000000000000000000000003e8",
	}

	_, err := NormalizeKnownLogs(&log)
	if err != nil {
		t.Fatal("expected no error for unrecognized event format, got err", err)
	}
}
