package base

import (
	"encoding/json"
	"fmt"
	"math/big"
	"testing"
)

// TestUnmarshalFloat tests the UnmarshalJSON method of Float. Note this is the whole
// point of the Float type: to be able to unmarshal JSON strings into big.Floats.
func TestUnmarshalFloat(t *testing.T) {
	type withFloat struct {
		Name  string  `json:"name"`
		Value *Float  `json:"value"`
		Float float64 `json:"float"`
	}
	type withBigFloat struct {
		Name  string     `json:"name"`
		Value *big.Float `json:"value"`
		Float float64    `json:"float"`
	}

	type testCase struct {
		input    string
		expected string
	}

	var tests = []testCase{
		{
			input:    "1",
			expected: "1",
		},
		{
			input:    `0.000000000000000001`,
			expected: "0.000000000000000001",
		},
		{
			input:    `0.123456789012345678`,
			expected: "0.123456789012345678",
		},
		{
			input:    `1.234567890123456789`,
			expected: "1.234567890123456789",
		},
	}

	quoted := []string{}
	unquoted := []string{}
	for _, test := range tests {
		unquoted = append(unquoted, test.input)
		quoted = append(quoted, fmt.Sprintf("\"%s\"", test.input))
	}

	expected := [][]bool{
		{true, true, true, true, true, true, true, true},     // unquoted both Float and Float work
		{true, false, true, false, true, false, true, false}, // quoted only Float works (that's the point)
	}

	for i, inputArray := range [][]string{unquoted, quoted} { /* four quoted and four unquoted */
		for j, inputStr := range inputArray {
			tag := fmt.Sprintf("%s%d", []string{"unquoted", "quoted"}[i], j)
			ethName := fmt.Sprintf("ether-%s", tag)
			bigName := fmt.Sprintf("big-%s", tag)
			passEther := expected[i][j*2]
			passFloat := expected[i][j*2+1]
			// fmt.Printf("Running test %s [%d][%d] (expects: %t, %t)\n", tag, i, j, passEther, passFloat)

			if e, ok := new(Float).SetString(inputStr); !ok {
				if passEther {
					t.Errorf("error: %s %s %s", tag, ethName, inputStr)
					continue
				}
			} else {
				fe := e.Float64()
				se := withFloat{
					Name:  ethName,
					Value: e,
					Float: fe,
				}
				if eJson, err := json.MarshalIndent(&se, "", "  "); err != nil {
					if passEther {
						t.Errorf("error: %s %s %s", tag, ethName, inputStr)
						continue
					}
				} else {
					t.Log(eJson)
					ve := se.Value
					if err := ve.UnmarshalJSON([]byte(inputStr)); err != nil {
						if passEther {
							t.Errorf("error: %s %s %s", tag, ethName, inputStr)
							continue
						}
					}
				}
			}

			if f, ok := new(big.Float).SetString(inputStr); !ok {
				if passFloat {
					t.Errorf("error: %s %s", ethName, inputStr)
					continue
				}

			} else {
				ff, _ := f.Float64()
				sf := withBigFloat{
					Name:  bigName,
					Value: f,
					Float: ff,
				}
				if fJson, err := json.MarshalIndent(&sf, "", "  "); err != nil {
					if passFloat {
						t.Errorf("error: %s %s %s", tag, ethName, inputStr)
						continue
					}
				} else {
					t.Log(fJson)
					ve := sf.Value
					if err := ve.UnmarshalText([]byte(inputStr)); err != nil {
						if passFloat {
							t.Errorf("error: %s %s %s", tag, ethName, inputStr)
							continue
						}
					}
				}
			}

			if !passEther && !passFloat {
				t.Errorf("error: %s %s", ethName, inputStr)
			}
		}
	}
}

func TestFloatBasicOperations(t *testing.T) {
	// Test NewFloat and its String method.
	e1 := NewFloat(1.5)
	// The String method calls Text('f', -18) which removes trailing zeros.
	if e1.String() != "1.5" {
		t.Errorf("NewFloat(1.5).String() = %s; want 1.5", e1.String())
	}

	// Test SetWei: 1 ether is 1e18 wei.
	weiVal := NewWeiStr("1000000000000000000")
	e2 := new(Float).SetWei(weiVal)
	if e2.String() != "1" {
		t.Errorf("SetWei(1e18) = %s; want 1", e2.String())
	}

	// Test SetString.
	e3, ok := new(Float).SetString("2.5")
	if !ok {
		t.Errorf("SetString('2.5') failed")
	}
	if e3.String() != "2.5" {
		t.Errorf("SetString('2.5') = %s; want 2.5", e3.String())
	}

	// Test arithmetic: Quo. (e2/e1 = 1/1.5 ≈ 0.666666666666666667)
	e4 := new(Float).Quo(e2, e1)
	diff := e4.Float64() - 0.6666666666666667
	if diff < -1e-10 || diff > 1e-10 {
		t.Errorf("Quo: 1 / 1.5 = %f; want approximately 0.6666666666666667", e4.Float64())
	}

	// Test JSON marshalling and unmarshalling.
	jsonData, err := e3.MarshalJSON()
	if err != nil {
		t.Errorf("Float MarshalJSON error: %v", err)
	}
	e5 := new(Float)
	err = e5.UnmarshalJSON(jsonData)
	if err != nil {
		t.Errorf("Float UnmarshalJSON error: %v", err)
	}
	if e5.String() != e3.String() {
		t.Errorf("Float JSON round-trip: got %s; want %s", e5.String(), e3.String())
	}
}
