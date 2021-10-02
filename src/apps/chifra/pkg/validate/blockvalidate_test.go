package validate

import (
	"testing"
)

func TestIsBlockHash(t *testing.T) {
	if IsBlockHash("fe611cbee814a8e84c3339186b6dc973947c2dc058cd41a5e3669f3b7f4c980d") {
		t.Error("Passes even if 0x is missing")
	}

	if IsBlockHash("0xfe611cbee814a8e84c3339186b6dc973947c2dc058") {
		t.Error("Passes for too short strings")
	}

	if IsBlockHash("0xgg611cbee814a8e84c3339186b6dc973947c2dc058cd41a5e3669f3b7f4c980d") {
		t.Error("Passes for non-hex strings")
	}

	if !IsBlockHash("0xfe611cbee814a8e84c3339186b6dc973947c2dc058cd41a5e3669f3b7f4c980d") {
		t.Error("Fails for correct hash")
	}
}

func TestIsBlockNumber(t *testing.T) {
	if IsBlockNumber("notanumber") {
		t.Error("Passes for strings")
	}

	if IsBlockNumber("") {
		t.Error("Passes for empty string")
	}

	// We only support decimal numbers for block numbers
	if IsBlockNumber("0xff") {
		t.Error("Passes for hex")
	}

	if !IsBlockNumber("0") {
		t.Error("Fails for 0")
	}

	if !IsBlockNumber("1000") {
		t.Error("Fails for numbers")
	}
}

func TestIsSpecialBlock(t *testing.T) {
	if IsSpecialBlock("london byzantium") {
		t.Error("Passes for invalid string (space)")
	}

	if IsSpecialBlock("lądon") {
		t.Error("Passes for string with invalid characters")
	}

	if IsSpecialBlock("123") {
		t.Error("Passes for number")
	}

	if IsSpecialBlock("ab") {
		t.Error("Passes for too short strings")
	}

	if !IsSpecialBlock("london") {
		t.Error("Fails for valid block name")
	}

	if !IsSpecialBlock("devcon1") {
		t.Error("Fails for valid block name with a number")
	}
}

func TestIsDateTimeString(t *testing.T) {
	if !IsDateTimeString("2021-09-28") {
		t.Error("Fails for date without time")
	}

	if !IsDateTimeString("2021-09-28T16") {
		t.Error("Fails for date without minutes")
	}

	if !IsDateTimeString("2021-09-28T16:42") {
		t.Error("Fails for date without seconds")
	}

	if !IsDateTimeString("2021-09-28T16:42:15") {
		t.Error("Fails for date with time")
	}

	if !IsDateTimeString("2021-09-28T16:42:15UTC") {
		t.Error("Fails for date with time and timezone")
	}

	if IsDateTimeString("hello") {
		t.Error("Passes for invalid date #1")
	}

	if IsDateTimeString("2020-08") {
		t.Error("Passes for invalid date #2")
	}
}

func TestIsRange(t *testing.T) {
	if IsRange("-0100") {
		t.Error("Passes for malformed string (1) ")
	}

	if IsRange("100-") {
		t.Error("Passes for malformed string (2) ")
	}

	if IsRange("0-100:") {
		t.Error("Passes for malformed string (3) ")
	}

	if !IsRange("0-100") {
		t.Error("Fails for range without step")
	}

	if !IsRange("100-100000:20") {
		t.Error("Fails for range with step")
	}

	if !IsRange("london") {
		t.Error("Fails for special")
	}
}
