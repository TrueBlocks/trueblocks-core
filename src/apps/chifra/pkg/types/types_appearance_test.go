package types

import (
	"encoding/json"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/stretchr/testify/assert"
)

func TestAppearanceInitialization(t *testing.T) {
	app := Appearance{
		Address:          base.HexToAddress("0x1234567890abcdef"),
		BlockNumber:      1000000,
		Reason:           "Test Appearance",
		TraceIndex:       0,
		TransactionIndex: 1,
	}

	assert.Equal(t, base.HexToAddress("0x1234567890abcdef"), app.Address)
	assert.Equal(t, uint32(1000000), app.BlockNumber)
	assert.Equal(t, "Test Appearance", app.Reason)
	assert.Equal(t, uint32(0), app.TraceIndex)
	assert.Equal(t, uint32(1), app.TransactionIndex)
}

func TestAppearanceString(t *testing.T) {
	app := Appearance{
		Address:          base.HexToAddress("0xabcdef"),
		BlockNumber:      123456,
		Reason:           "Unit Test",
		Timestamp:        base.Timestamp(1710000000), // Mock timestamp
		TraceIndex:       2,
		TransactionIndex: 3,
	}

	expectedJSON, _ := json.Marshal(app)
	assert.JSONEq(t, string(expectedJSON), app.String(), "Appearance String() should return JSON")
}

func TestAppearanceDate(t *testing.T) {
	app := Appearance{
		Timestamp: base.Timestamp(1710000000), // Mock timestamp
	}

	expectedDate := base.FormattedDate(app.Timestamp)
	assert.Equal(t, expectedDate, app.Date(), "Appearance Date() should format correctly")
}

func TestAppearanceGetKey(t *testing.T) {
	app := Appearance{
		Address:          base.HexToAddress("0xabcdef"),
		BlockNumber:      123456,
		TransactionIndex: 3,
	}

	expectedKey := "0x0000000000000000000000000000000000abcdef\t000123456\t00003"
	got := app.GetKey()
	assert.Equal(t, expectedKey, got, "Appearance GetKey() should return the correct key format")
}
