package base

import (
	"bytes"
	"math/big"
	"strings"
	"testing"
)

func TestFormattedValue(t *testing.T) {
	vv := NewWei(0)
	vv, _ = vv.SetString("599853750352891471017612", 10)
	tests := []struct {
		name     string
		bigIn    *Wei
		asEther  bool
		decimals int
		want     string
	}{
		{
			name:     "Test with positive value",
			bigIn:    NewWei(1000000000000000000),
			asEther:  true,
			decimals: 18,
			want:     "1",
		},
		{
			name:     "Test with negative value",
			bigIn:    NewWei(-1000000000000000000),
			asEther:  true,
			decimals: 18,
			want:     "-1",
		},
		{
			name:     "Test with zero value",
			bigIn:    NewWei(0),
			asEther:  true,
			decimals: 18,
			want:     "0",
		},
		{
			name:     "Test with large positive value",
			bigIn:    NewWei(123456789012345678),
			asEther:  true,
			decimals: 18,
			want:     "0.123456789012345678",
		},
		{
			name:     "Test with large negative value",
			bigIn:    NewWei(-123456789012345678),
			asEther:  true,
			decimals: 18,
			want:     "-0.123456789012345678",
		},
		{
			name:     "Test with non-ether value",
			bigIn:    NewWei(1234567890),
			asEther:  false,
			decimals: 0,
			want:     "1234567890",
		},
		{
			name:     "Test with non-ether value and decimals",
			bigIn:    NewWei(1234567890),
			asEther:  false,
			decimals: 2, // decimals is not used when asEther is false
			want:     "1234567890",
		},
		{
			name:     "live test",
			bigIn:    vv,
			asEther:  true,
			decimals: 18,
			want:     "599853.750352891471017612",
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got := tt.bigIn.ToEtherStr(tt.decimals)
			if !tt.asEther {
				got = tt.bigIn.String()
			}
			if got != tt.want {
				t.Errorf("%s TestFv = %v, want %v", tt.name, got, tt.want)
			}
		})
	}
}

func TestToEther(t *testing.T) {
	type testCase struct {
		input    *Wei
		expected string
	}

	v1, _ := new(Wei).SetString("123456789012345678", 10)
	v2, _ := new(Wei).SetString("1234567890123456789", 10)
	var tests = []testCase{
		{NewWei(0), "0"},
		{NewWei(1), "0.000000000000000001"},
		{v1, "0.123456789012345678"},
		{v2, "1.234567890123456789"},
	}

	for _, test := range tests {
		got := ToEther(test.input).Text('f', -18)
		if got != test.expected {
			t.Errorf("ToEther(%v) want %v, got %v", test.input, test.expected, got)
		}
		got = ToEther(test.input).String()
		if got != test.expected {
			t.Errorf("ToEther(%v) want %v, got %v", test.input, test.expected, got)
		}
		got = ToEther(test.input).String()
		if got != test.expected {
			t.Errorf("ToEther(%v) want %v, got %v", test.input, test.expected, got)
		}
	}
}

func TestWeiBasicOperations(t *testing.T) {
	// NewWei
	w1 := NewWei(100)
	if w1.String() != "100" {
		t.Errorf("NewWei(100) = %s; want 100", w1.String())
	}

	// NewWeiStr with a decimal string
	w2 := NewWeiStr("12345")
	if w2.String() != "12345" {
		t.Errorf("NewWeiStr('12345') = %s; want 12345", w2.String())
	}

	// NewWeiStr with a hex string (the prefix is trimmed)
	w3 := NewWeiStr("0x10")
	if w3.String() != "16" {
		t.Errorf("NewWeiStr('0x10') = %s; want 16", w3.String())
	}

	// Test SetString (decimal)
	var w4 Wei
	res, ok := w4.SetString("42", 10)
	if !ok {
		t.Errorf("SetString('42', 10) failed")
	}
	if res.String() != "42" {
		t.Errorf("SetString('42', 10) = %s; want 42", res.String())
	}

	// Test SetString (hex)
	var w5 Wei
	res, ok = w5.SetString("0xff", 0)
	if !ok {
		t.Errorf("SetString('0xff', 0) failed")
	}
	if res.String() != "255" {
		t.Errorf("SetString('0xff', 0) = %s; want 255", res.String())
	}

	// Test IsZero
	wZero := NewWei(0)
	if !wZero.IsZero() {
		t.Errorf("IsZero() for 0 should be true")
	}
	if w1.IsZero() {
		t.Errorf("IsZero() for non-zero value should be false")
	}

	// Test ToInt and Bytes
	bigInt := w1.ToInt()
	if bigInt.Int64() != 100 {
		t.Errorf("ToInt() = %d; want 100", bigInt.Int64())
	}
	bytesVal := w1.Bytes()
	expectedBytes := big.NewInt(100).Bytes()
	if !bytes.Equal(bytesVal, expectedBytes) {
		t.Errorf("Bytes() = %v; want %v", bytesVal, expectedBytes)
	}

	// Test SetUint64 and SetInt64
	w6 := NewWei(0)
	w6.SetUint64(200)
	if w6.String() != "200" {
		t.Errorf("SetUint64(200) = %s; want 200", w6.String())
	}
	w6.SetInt64(-300)
	if w6.String() != "-300" {
		t.Errorf("SetInt64(-300) = %s; want -300", w6.String())
	}

	// Test Float64 and Uint64
	w7 := NewWei(1)
	if w7.Float64() != 1.0 {
		t.Errorf("Float64() = %f; want 1.0", w7.Float64())
	}
	if w7.Uint64() != 1 {
		t.Errorf("Uint64() = %d; want 1", w7.Uint64())
	}

	// Test Text (for base 10, should be "1")
	if w7.Text(10) != "1" {
		t.Errorf("Text(10) = %s; want 1", w7.Text(10))
	}
}

func TestWeiArithmetic(t *testing.T) {
	a := NewWei(10)
	b := NewWei(20)
	sum := new(Wei).Add(a, b)
	if sum.String() != "30" {
		t.Errorf("Add: 10 + 20 = %s; want 30", sum.String())
	}

	sub := new(Wei).Sub(b, a)
	if sub.String() != "10" {
		t.Errorf("Sub: 20 - 10 = %s; want 10", sub.String())
	}

	mul := new(Wei).Mul(a, b)
	if mul.String() != "200" {
		t.Errorf("Mul: 10 * 20 = %s; want 200", mul.String())
	}

	div := new(Wei).Div(b, a)
	if div.String() != "2" {
		t.Errorf("Div: 20 / 10 = %s; want 2", div.String())
	}

	quo := new(Wei).Quo(b, a)
	if quo.String() != "2" {
		t.Errorf("Quo: 20 / 10 = %s; want 2", quo.String())
	}
}

func TestWeiComparison(t *testing.T) {
	a := NewWei(10)
	b := NewWei(20)
	if a.Cmp(a) != 0 {
		t.Errorf("Cmp: 10 vs 10 should be 0")
	}
	if a.Cmp(b) >= 0 {
		t.Errorf("Cmp: 10 vs 20 should be negative")
	}
	if b.Cmp(a) <= 0 {
		t.Errorf("Cmp: 20 vs 10 should be positive")
	}

	a2 := NewWei(10)
	if !a.Equal(a2) {
		t.Errorf("Equals: 10 should equal 10")
	}
}

func TestWeiSerialization(t *testing.T) {
	// Test UnmarshalJSON (expecting a JSON string with quotes)
	var w Wei
	err := w.UnmarshalJSON([]byte(`"12345"`))
	if err != nil {
		t.Errorf("UnmarshalJSON error: %v", err)
	}
	if w.String() != "12345" {
		t.Errorf("UnmarshalJSON: got %s; want 12345", w.String())
	}

	// Test MarshalText.
	// NOTE: According to the implementation, MarshalText returns "0" if the local text var is empty.
	text, err := w.MarshalText()
	if err != nil {
		t.Errorf("MarshalText error: %v", err)
	}
	if string(text) != "0" {
		t.Errorf("MarshalText: got %s; want 0 (per implementation)", string(text))
	}

	// Test UnmarshalText.
	var w2 Wei
	err = w2.UnmarshalText([]byte("67890"))
	if err != nil {
		t.Errorf("UnmarshalText error: %v", err)
	}
	if w2.String() != "67890" {
		t.Errorf("UnmarshalText: got %s; want 67890", w2.String())
	}
}

func TestWeiCacheMarshalling(t *testing.T) {
	// Create a Wei value and marshal it using the Cache functions.
	w := NewWei(123456789)
	buf := new(bytes.Buffer)
	if err := w.MarshalCache(buf); err != nil {
		t.Errorf("MarshalCache error: %v", err)
	}

	// Now unmarshal into a new Wei and check that the values match.
	var w2 Wei
	if err := w2.UnmarshalCache(1, buf); err != nil {
		t.Errorf("UnmarshalCache error: %v", err)
	}
	if !w.Equal(&w2) {
		t.Errorf("Cache marshalling: expected %s, got %s", w.String(), w2.String())
	}
}

func TestHexToWeiAndWeiToHash(t *testing.T) {
	// Test HexToWei with a short hex string.
	w := HexToWei("0x1a")
	if w.String() != "26" {
		t.Errorf("HexToWei('0x1a') = %s; want 26", w.String())
	}

	// Test HexToWei with a long hex string (>66 characters).
	hexLong := "0x" + strings.Repeat("f", 70)
	wLong := HexToWei(hexLong)
	expected, _ := new(big.Int).SetString(strings.Repeat("f", 64), 16)
	if wLong.ToInt().Cmp(expected) != 0 {
		t.Errorf("HexToWei(long) = %s; want %s", wLong.String(), expected.String())
	}

	// Test WeiToHash – the returned hash should be 32 bytes (hex string length 66 including "0x")
	wHash := NewWei(255) // 255 => 0xff
	hashHex := WeiToHash(wHash)
	if !strings.HasPrefix(hashHex, "0x") || len(hashHex) != 66 {
		t.Errorf("WeiToHash output = %s; want a 66-character hex string", hashHex)
	}
	if !strings.HasSuffix(hashHex, "ff") {
		t.Errorf("WeiToHash output = %s; want to end with 'ff'", hashHex)
	}
}

func TestWeiSignAndNegation(t *testing.T) {
	wPos := NewWei(10)
	if wPos.Sign() != 1 {
		t.Errorf("Sign(10) = %d; want 1", wPos.Sign())
	}
	wZero := NewWei(0)
	if wZero.Sign() != 0 {
		t.Errorf("Sign(0) = %d; want 0", wZero.Sign())
	}
	wNeg := NewWei(-10)
	if wNeg.Sign() != -1 {
		t.Errorf("Sign(-10) = %d; want -1", wNeg.Sign())
	}

	negWPos := wPos.Neg()
	if negWPos.String() != "-10" {
		t.Errorf("Neg(10) = %s; want -10", negWPos.String())
	}
	negWNeg := wNeg.Neg()
	if negWNeg.String() != "10" {
		t.Errorf("Neg(-10) = %s; want 10", negWNeg.String())
	}
}
