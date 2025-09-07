package base

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"testing"

	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

func TestAddress_Hex(t *testing.T) {
	a := hexutil.MustDecode("0xAb")
	b := []byte{0xAb}
	c := bytes.Compare(a, b)
	if c != 0 {
		t.Error("incorrect result from MustDecode")
	}

	addr := HexToAddress("0x00000000000000000000000000000000deadbeef")

	hex := "0xF1AA581F353005BA3765B81BF52D6B1C488C2101"
	addr.SetHex(hex)

	expectedHex := "0xf1aa581f353005ba3765b81bf52d6b1c488c2101"
	if result := addr.Hex(); result != expectedHex {
		t.Fatal("wrong Hex() return value:", result)
	}

	zero := "0x0000000000000000000000000000000000000000"
	expectedHex = "0x0"
	addr = HexToAddress(zero)
	if result := addr.Hex(); result != expectedHex {
		t.Fatal("wrong Hex() return value for 0x0:", result)
	}

	var zero2 Address
	addrStr := fmt.Sprint(zero2)
	if addrStr != expectedHex {
		t.Fatal("wrong Hex() return value for 0x0:", addrStr)
	}
}

func TestAddress_Stringer(t *testing.T) {
	hex := "0xF1AA581F353005BA3765B81BF52D6B1C488C2101"
	var addr Address
	addr.SetHex(hex)

	expected := "0xf1aa581f353005ba3765b81bf52d6b1c488c2101"
	if result := fmt.Sprint(addr); result != expected {
		t.Fatal("wrong Hex() return value:", result)
	}
}

func TestAddress_IsZero(t *testing.T) {
	var zeroValue Address
	if result := zeroValue.IsZero(); result != true {
		t.Fatal("wrong result for zero value")
	}

	if result := ZeroAddr.IsZero(); result != true {
		t.Fatal("wrong result for zero address")
	}
}

func TestHexToAddress(t *testing.T) {
	hex := "0xF1AA581F353005BA3765B81BF52D6B1C488C2101"
	addr := HexToAddress(hex)

	expected := "0xf1aa581f353005ba3765b81bf52d6b1c488c2101"
	if result := addr.Hex(); result != expected {
		t.Fatal("HexToAddress: wrong Hex() return value:", result)
	}
}

func TestAddressCompareToCommon(t *testing.T) {
	c := common.HexToAddress("0x00000123456789abcde")
	b := HexToAddress("0x00000123456789abcde")
	if c != b.Common() {
		t.Fatal("base.Hash.toCommon() does not match")
	}
	if b != new(Address).SetCommon(&c) {
		t.Fatal("fromCommon(c) does not match Hash")
	}
}

// TestAddress_JSONMarshalling tests JSON unmarshalling (including the special cases)
// and marshalling (via MarshalText) of the Address type.
func TestAddress_JSONMarshalling(t *testing.T) {
	// --- Unmarshal valid hex address ---
	var addr Address
	jsonStr := "\"0xf1aa581f353005ba3765b81bf52d6b1c488c2101\""
	if err := json.Unmarshal([]byte(jsonStr), &addr); err != nil {
		t.Fatal("failed to unmarshal valid address:", err)
	}
	expected := "0xf1aa581f353005ba3765b81bf52d6b1c488c2101"
	if addr.Hex() != expected {
		t.Fatalf("expected %s, got %s", expected, addr.Hex())
	}

	// --- Unmarshal special values that should yield a zero address ---
	var addrZero Address
	cases := []string{"\"0x0\"", "\"\"", "null"}
	for _, tc := range cases {
		if err := json.Unmarshal([]byte(tc), &addrZero); err != nil {
			t.Fatalf("failed to unmarshal %s: %v", tc, err)
		}
		if !addrZero.IsZero() {
			t.Fatalf("expected zero address for %s", tc)
		}
	}

	// --- Test MarshalText (which should yield the same as Hex()) ---
	text, err := addr.MarshalText()
	if err != nil {
		t.Fatal("MarshalText failed:", err)
	}
	if string(text) != expected {
		t.Fatalf("MarshalText expected %s, got %s", expected, string(text))
	}
}

// TestAddress_CheckSum tests that the CheckSum method produces the correct EIP‑55 checksum.
func TestAddress_CheckSum(t *testing.T) {
	// Using a known test vector from EIP‑55:
	// Input: "0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359"
	// Expected checksum: "0xfB6916095ca1df60bB79Ce92cE3Ea74c37c5d359"
	addr := HexToAddress("0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359")
	expected := "0xfB6916095ca1df60bB79Ce92cE3Ea74c37c5d359"
	if cs := addr.CheckSum(); cs != expected {
		t.Fatalf("CheckSum expected %s, got %s", expected, cs)
	}

	// Check that a zero address returns "0x0"
	var zero Address
	if cs := zero.CheckSum(); cs != "0x0" {
		t.Fatalf("CheckSum for zero address expected %s, got %s", "0x0", cs)
	}
}

// TestAddressFromPath tests AddressFromPath for both the happy path and an error case.
func TestAddressFromPath(t *testing.T) {
	// --- Happy path: valid file path ---
	path := "/some/dir/0xf1aa581f353005ba3765b81bf52d6b1c488c2101.monitor"
	addr, err := AddressFromPath(path, ".monitor")
	if err != nil {
		t.Fatal("unexpected error in AddressFromPath:", err)
	}
	expected := "0xf1aa581f353005ba3765b81bf52d6b1c488c2101"
	if addr.Hex() != expected {
		t.Fatalf("expected %s, got %s", expected, addr.Hex())
	}

	// --- Error path: filename does not contain a valid address ---
	badPath := "/some/dir/invalidfilename.monitor"
	addr, err = AddressFromPath(badPath, ".monitor")
	if err == nil {
		t.Fatal("expected error for invalid address in path, got nil")
	}
	if addr.Hex() != "0x0" {
		t.Fatalf("expected zero address for error case, got %s", addr.Hex())
	}
}

// TestValidHexAndAddressValidation tests the functions ValidHex, IsValidAddress, and IsValidAddressE.
func TestValidHexAndAddressValidation(t *testing.T) {
	// --- Valid hex string ---
	valid := "0xf1aa581f353005ba3765b81bf52d6b1c488c2101"
	ok, err := ValidHex(valid, 20)
	if !ok || err != nil {
		t.Fatalf("ValidHex expected valid for %s, got error %v", valid, err)
	}

	// --- Missing "0x" prefix ---
	invalid := "f1aa581f353005ba3765b81bf52d6b1c488c2101"
	ok, err = ValidHex(invalid, 20)
	if ok || !errors.Is(err, ErrNoLeading0x) {
		t.Fatalf("ValidHex expected ErrNoLeading0x for %s, got %v", invalid, err)
	}

	// --- Incorrect length ---
	short := "0xf1aa581f353005ba3765b81bf52d6b1c488c21"
	ok, err = ValidHex(short, 20)
	if ok || !errors.Is(err, ErrInvalidLength) {
		t.Fatalf("ValidHex expected ErrInvalidLength for %s, got %v", short, err)
	}

	// --- Non-hex characters ---
	nonHex := "0xf1aa581f353005ba3765b81bf52d6b1c488c21zz"
	ok, err = ValidHex(nonHex, 20)
	if ok || !errors.Is(err, ErrInvalidHex) {
		t.Fatalf("ValidHex expected ErrInvalidHex for %s, got %v", nonHex, err)
	}

	// --- IsValidAddress for hex addresses ---
	if !IsValidAddress(valid) {
		t.Fatal("IsValidAddress expected true for a valid hex address")
	}

	// --- IsValidAddressE for .eth addresses ---
	validEth := "vitalik.eth"
	ok, err = IsValidAddressE(validEth)
	if !ok || err != nil {
		t.Fatal("IsValidAddressE expected true for valid .eth address")
	}

	// A .eth address containing whitespace should be invalid.
	invalidEth := "vitalik. eth"
	ok, _ = IsValidAddressE(invalidEth)
	if ok {
		t.Fatal("IsValidAddressE expected false for .eth address with whitespace")
	}
}

// TestUtilityMethods tests the helper methods: Prefix, Encoded32, Pad32, and IsPrecompile.
func TestUtilityMethods(t *testing.T) {
	addr := HexToAddress("0xf1aa581f353005ba3765b81bf52d6b1c488c2101")
	hexStr := addr.Hex() // Expected: "0xf1aa581f353005ba3765b81bf52d6b1c488c2101"

	// --- Test Prefix ---
	expectedSym := hexStr[:6]
	if sym := addr.DefaultSymbol(); sym != expectedSym {
		t.Fatalf("Prefix expected %s, got %s", expectedSym, sym)
	}

	// --- Test Pad32 ---
	expectedPad32 := "000000000000000000000000" + hexStr[2:]
	if pad := addr.Pad32(); pad != expectedPad32 {
		t.Fatalf("Pad32 expected %s, got %s", expectedPad32, pad)
	}

	// --- Test IsPrecompile ---
	// A known precompile address should return true.
	precompileAddr := HexToAddress("0x0000000000000000000000000000000000000001")
	if !precompileAddr.IsPrecompile() {
		t.Fatalf("IsPrecompile expected true for %s", precompileAddr)
	}

	// A non-precompile address (e.g. starting with 0x1...) should return false.
	nonPrecompileAddr := HexToAddress("0x1000000000000000000000000000000000000000")
	if nonPrecompileAddr.IsPrecompile() {
		t.Fatalf("IsPrecompile expected false for %s", nonPrecompileAddr)
	}
}

func TestAddress_Prefix_CustomParameter(t *testing.T) {
	addr := HexToAddress("0xf1aa581f353005ba3765b81bf52d6b1c488c2101")
	expectedSym := addr.Hex()[:6] // e.g. "0xf1aa"
	for _, n := range []int{3, 6, 10} {
		if sym := addr.DefaultSymbol(); sym != expectedSym {
			t.Errorf("Prefix(%d) expected %s, got %s", n, expectedSym, sym)
		}
	}
}

func TestAddress_UnmarshalJSON_Invalid(t *testing.T) {
	var addr Address
	invalidInputs := []string{
		"\"invalid\"", // not a valid hex string
		"\"0x12345\"", // too short for a full address
		"\"0xzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\"", // invalid hex characters
	}
	for _, input := range invalidInputs {
		err := addr.UnmarshalJSON([]byte(input))
		if err == nil {
			t.Errorf("Expected error unmarshalling %s, got nil", input)
		}
	}
}

func TestIsHex(t *testing.T) {
	// Valid hex strings.
	valids := []string{
		"0x",       // empty after prefix is considered valid by IsHex.
		"0x123abc", // lower-case only.
		"0xABCDEF", // upper-case only.
		"0x123aBC", // mixed-case.
	}
	for _, s := range valids {
		if !IsHex(s) {
			t.Errorf("IsHex(%s) expected true", s)
		}
	}

	// Invalid hex strings.
	invalids := []string{
		"0x123abz", // 'z' is not a hex character.
		"0x12#34",  // '#' is not allowed.
	}
	for _, s := range invalids {
		if IsHex(s) {
			t.Errorf("IsHex(%s) expected false", s)
		}
	}
}

func TestAddressComparison(t *testing.T) {
	tests := []struct {
		name                   string
		aStr, bStr             string
		expectedEqual          bool
		expectedLess           bool
		expectedLessOrEqual    bool
		expectedGreater        bool
		expectedGreaterOrEqual bool
	}{
		{
			name:                   "Equal addresses",
			aStr:                   "0x0000000000000000000000000000000000000001",
			bStr:                   "0x0000000000000000000000000000000000000001",
			expectedEqual:          true,
			expectedLess:           false,
			expectedLessOrEqual:    true,
			expectedGreater:        false,
			expectedGreaterOrEqual: true,
		},
		{
			name:                   "a less than b",
			aStr:                   "0x0000000000000000000000000000000000000001",
			bStr:                   "0x0000000000000000000000000000000000000002",
			expectedEqual:          false,
			expectedLess:           true,
			expectedLessOrEqual:    true,
			expectedGreater:        false,
			expectedGreaterOrEqual: false,
		},
		{
			name:                   "a greater than b",
			aStr:                   "0x0000000000000000000000000000000000000003",
			bStr:                   "0x0000000000000000000000000000000000000002",
			expectedEqual:          false,
			expectedLess:           false,
			expectedLessOrEqual:    false,
			expectedGreater:        true,
			expectedGreaterOrEqual: true,
		},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			a := HexToAddress(tc.aStr)
			b := HexToAddress(tc.bStr)

			if a.Equal(b) != tc.expectedEqual {
				t.Errorf("Equal: got %v, expected %v", a.Equal(b), tc.expectedEqual)
			}

			if a.LessThan(b) != tc.expectedLess {
				t.Errorf("LessThan: got %v, expected %v", a.LessThan(b), tc.expectedLess)
			}

			if a.LessThanOrEqual(b) != tc.expectedLessOrEqual {
				t.Errorf("LessThanOrEqual: got %v, expected %v", a.LessThanOrEqual(b), tc.expectedLessOrEqual)
			}

			if a.GreaterThan(b) != tc.expectedGreater {
				t.Errorf("GreaterThan: got %v, expected %v", a.GreaterThan(b), tc.expectedGreater)
			}

			if a.GreaterThanOrEqual(b) != tc.expectedGreaterOrEqual {
				t.Errorf("GreaterThanOrEqual: got %v, expected %v", a.GreaterThanOrEqual(b), tc.expectedGreaterOrEqual)
			}
		})
	}
}

func TestDisplay(t *testing.T) {
	addr := HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b")
	tests := []struct {
		name        string
		left, right int
		expected    string
	}{
		{
			name:     "Full address with left=0, right=0",
			left:     0,
			right:    0,
			expected: "0xf503017d7baf7fbc0fff7492b751025c6a78179b",
		},
		{
			name:     "Last 4 digits with left=0, right=4",
			left:     0,
			right:    4,
			expected: "0x179b",
		},
		{
			name:     "First 4 digits with left=4, right=0",
			left:     4,
			right:    0,
			expected: "0xf503",
		},
		{
			name:     "Both left and right non-zero with left=4, right=4",
			left:     4,
			right:    4,
			expected: "0xf503...179b",
		},
		{
			name:     "Left digits exceed available length (not recommended in production)",
			left:     100,
			right:    0,
			expected: "0xf503017d7baf7fbc0fff7492b751025c6a78179b",
		},
		{
			name:     "Right digits exceed available length (not recommended in production)",
			left:     0,
			right:    100,
			expected: "0xf503017d7baf7fbc0fff7492b751025c6a78179b",
		},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			result := addr.Display(tc.left, tc.right)
			if result != tc.expected {
				t.Errorf("Display(%d, %d) = %q; want %q", tc.left, tc.right, result, tc.expected)
			}
		})
	}
}
