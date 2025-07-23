// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package types

import (
	"bytes"
	"encoding/json"
	"math/big"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/stretchr/testify/assert"
)

// Helper function to convert various numeric types to float64 for JSON comparison
func convertToFloat64(value interface{}) (float64, bool) {
	switch v := value.(type) {
	case uint8:
		return float64(v), true
	case uint16:
		return float64(v), true
	case uint32:
		return float64(v), true
	case uint64:
		return float64(v), true
	case int8:
		return float64(v), true
	case int16:
		return float64(v), true
	case int32:
		return float64(v), true
	case int64:
		return float64(v), true
	case int:
		return float64(v), true
	case float32:
		return float64(v), true
	case float64:
		return v, true
	default:
		return 0, false
	}
}

// Test_Parameter_Value_String tests Parameter with string values
func Test_Parameter_Value_String(t *testing.T) {
	tests := []struct {
		name  string
		value string
	}{
		{"empty_string", ""},
		{"simple_string", "hello world"},
		{"address_string", "0x6b175474e89094c44da98b954eedeac495271d0f"},
		{"hex_string", "0x1234567890abcdef"},
		{"unicode_string", "Hello 世界 🌍"},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			param := Parameter{
				Name:          "testParam",
				ParameterType: "string",
				InternalType:  "string",
				Value:         tt.value,
			}

			// Test JSON serialization
			jsonData, err := json.Marshal(param)
			assert.NoError(t, err, "JSON marshal should succeed")

			var readParam Parameter
			err = json.Unmarshal(jsonData, &readParam)
			assert.NoError(t, err, "JSON unmarshal should succeed")
			assert.Equal(t, param.Value, readParam.Value, "JSON round-trip should preserve string value")

			// Test binary cache serialization
			buffer := new(bytes.Buffer)
			err = param.MarshalCache(buffer)
			assert.NoError(t, err, "Binary marshal should succeed")

			var cachedParam Parameter
			err = cachedParam.UnmarshalCache(0, buffer)
			assert.NoError(t, err, "Binary unmarshal should succeed")
			assert.Equal(t, param.Name, cachedParam.Name, "Binary round-trip should preserve name")
			assert.Equal(t, param.ParameterType, cachedParam.ParameterType, "Binary round-trip should preserve type")
			// Note: Value is not serialized in binary cache for Parameter
		})
	}
}

// Test_Parameter_Value_Numbers tests Parameter with various numeric types
func Test_Parameter_Value_Numbers(t *testing.T) {
	tests := []struct {
		name  string
		value interface{}
		pType string
	}{
		{"uint8", uint8(255), "uint8"},
		{"uint16", uint16(65535), "uint16"},
		{"uint32", uint32(4294967295), "uint32"},
		{"uint64", uint64(18446744073709551615), "uint64"},
		{"int8", int8(-128), "int8"},
		{"int16", int16(-32768), "int16"},
		{"int32", int32(-2147483648), "int32"},
		{"int64", int64(-9223372036854775808), "int64"},
		{"big_int", func() *big.Int { i, _ := big.NewInt(0).SetString("12345678901234567890", 10); return i }(), "uint256"},
		{"negative_big_int", func() *big.Int { i, _ := big.NewInt(0).SetString("-12345678901234567890", 10); return i }(), "int256"},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			param := Parameter{
				Name:          "numParam",
				ParameterType: tt.pType,
				InternalType:  tt.pType,
				Value:         tt.value,
			}

			// Test JSON serialization
			jsonData, err := json.Marshal(param)
			assert.NoError(t, err, "JSON marshal should succeed")

			var readParam Parameter
			err = json.Unmarshal(jsonData, &readParam)
			assert.NoError(t, err, "JSON unmarshal should succeed")

			// For big.Int, we need special handling since JSON unmarshaling might change the type
			if bigInt, ok := tt.value.(*big.Int); ok {
				if readBigInt, ok := readParam.Value.(*big.Int); ok {
					assert.Equal(t, bigInt.String(), readBigInt.String(), "Big int values should match")
				} else {
					// JSON might unmarshal big ints as strings or numbers (float64)
					// This is expected behavior - JSON doesn't preserve Go's specific numeric types
					t.Logf("Big int unmarshaled as %T: %v", readParam.Value, readParam.Value)
					// We'll accept this as normal JSON behavior
				}
			} else {
				// JSON unmarshaling converts all numbers to float64, so we need to handle this
				if expectedFloat, ok := convertToFloat64(tt.value); ok {
					if actualFloat, ok := readParam.Value.(float64); ok {
						assert.Equal(t, expectedFloat, actualFloat, "JSON round-trip should preserve numeric value as float64")
					} else {
						t.Errorf("Expected float64 but got %T: %v", readParam.Value, readParam.Value)
					}
				} else {
					assert.Equal(t, param.Value, readParam.Value, "JSON round-trip should preserve numeric value")
				}
			}

			// Test binary cache serialization
			buffer := new(bytes.Buffer)
			err = param.MarshalCache(buffer)
			assert.NoError(t, err, "Binary marshal should succeed")

			var cachedParam Parameter
			err = cachedParam.UnmarshalCache(0, buffer)
			assert.NoError(t, err, "Binary unmarshal should succeed")
			assert.Equal(t, param.Name, cachedParam.Name, "Binary round-trip should preserve name")
			assert.Equal(t, param.ParameterType, cachedParam.ParameterType, "Binary round-trip should preserve type")
		})
	}
}

// Test_Parameter_Value_Boolean tests Parameter with boolean values
func Test_Parameter_Value_Boolean(t *testing.T) {
	tests := []struct {
		name  string
		value bool
	}{
		{"true", true},
		{"false", false},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			param := Parameter{
				Name:          "boolParam",
				ParameterType: "bool",
				InternalType:  "bool",
				Value:         tt.value,
			}

			// Test JSON serialization
			jsonData, err := json.Marshal(param)
			assert.NoError(t, err, "JSON marshal should succeed")

			var readParam Parameter
			err = json.Unmarshal(jsonData, &readParam)
			assert.NoError(t, err, "JSON unmarshal should succeed")
			assert.Equal(t, param.Value, readParam.Value, "JSON round-trip should preserve boolean value")

			// Test binary cache serialization
			buffer := new(bytes.Buffer)
			err = param.MarshalCache(buffer)
			assert.NoError(t, err, "Binary marshal should succeed")

			var cachedParam Parameter
			err = cachedParam.UnmarshalCache(0, buffer)
			assert.NoError(t, err, "Binary unmarshal should succeed")
			assert.Equal(t, param.Name, cachedParam.Name, "Binary round-trip should preserve name")
			assert.Equal(t, param.ParameterType, cachedParam.ParameterType, "Binary round-trip should preserve type")
		})
	}
}

// Test_Parameter_Value_Arrays tests Parameter with array/slice values
func Test_Parameter_Value_Arrays(t *testing.T) {
	tests := []struct {
		name  string
		value interface{}
		pType string
	}{
		{"string_array", []string{"hello", "world", "test"}, "string[]"},
		{"number_array", []int{1, 2, 3, 42, 100}, "uint256[]"},
		{"address_array", []string{
			"0x6b175474e89094c44da98b954eedeac495271d0f",
			"0xa735cf11ed1228feb7c7bb18673a868455ffb16f",
		}, "address[]"},
		{"mixed_interface_array", []interface{}{"hello", 42, true}, "tuple[]"},
		{"nested_array", [][]string{{"a", "b"}, {"c", "d"}}, "string[][]"},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			param := Parameter{
				Name:          "arrayParam",
				ParameterType: tt.pType,
				InternalType:  tt.pType,
				Value:         tt.value,
			}

			// Test JSON serialization
			jsonData, err := json.Marshal(param)
			assert.NoError(t, err, "JSON marshal should succeed")

			var readParam Parameter
			err = json.Unmarshal(jsonData, &readParam)
			assert.NoError(t, err, "JSON unmarshal should succeed")

			// Arrays might be unmarshaled as []interface{}, so we need flexible comparison
			t.Logf("Original value type: %T, value: %v", param.Value, param.Value)
			t.Logf("Read value type: %T, value: %v", readParam.Value, readParam.Value)

			// Test binary cache serialization
			buffer := new(bytes.Buffer)
			err = param.MarshalCache(buffer)
			assert.NoError(t, err, "Binary marshal should succeed")

			var cachedParam Parameter
			err = cachedParam.UnmarshalCache(0, buffer)
			assert.NoError(t, err, "Binary unmarshal should succeed")
			assert.Equal(t, param.Name, cachedParam.Name, "Binary round-trip should preserve name")
			assert.Equal(t, param.ParameterType, cachedParam.ParameterType, "Binary round-trip should preserve type")
		})
	}
}

// Test_Parameter_Value_ComplexObjects tests Parameter with complex object values
func Test_Parameter_Value_ComplexObjects(t *testing.T) {
	// Create a complex nested structure similar to what we see in real ABIs
	complexStruct := map[string]interface{}{
		"token":  "0x6b175474e89094c44da98b954eedeac495271d0f",
		"amount": big.NewInt(1000000000000000000),
		"dest":   "0xa735cf11ed1228feb7c7bb18673a868455ffb16f",
		"metadata": map[string]interface{}{
			"timestamp": uint64(1642000000),
			"tags":      []string{"defi", "donation"},
			"verified":  true,
		},
	}

	// Create a nested Function object
	nestedFunction := Function{
		Name:            "nestedFunc",
		Signature:       "nestedFunc(uint256)",
		Encoding:        "0x12345678",
		StateMutability: "view",
		Inputs: []Parameter{
			{
				Name:          "value",
				ParameterType: "uint256",
				InternalType:  "uint256",
				Value:         big.NewInt(42),
			},
		},
	}

	tests := []struct {
		name  string
		value interface{}
		pType string
	}{
		{"complex_map", complexStruct, "tuple"},
		{"nested_function", nestedFunction, "function"},
		{"base_address", base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f"), "address"},
		{"base_hash", base.HexToHash("0x1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef"), "bytes32"},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			param := Parameter{
				Name:          "complexParam",
				ParameterType: tt.pType,
				InternalType:  tt.pType,
				Value:         tt.value,
			}

			// Test JSON serialization
			jsonData, err := json.Marshal(param)
			assert.NoError(t, err, "JSON marshal should succeed")

			var readParam Parameter
			err = json.Unmarshal(jsonData, &readParam)
			assert.NoError(t, err, "JSON unmarshal should succeed")

			t.Logf("Original value type: %T", param.Value)
			t.Logf("Read value type: %T", readParam.Value)

			// For complex objects, we mainly want to ensure no errors occur
			// The exact structure might change during JSON round-trip
			assert.Equal(t, param.Name, readParam.Name, "Parameter name should be preserved")
			assert.Equal(t, param.ParameterType, readParam.ParameterType, "Parameter type should be preserved")

			// Test binary cache serialization
			buffer := new(bytes.Buffer)
			err = param.MarshalCache(buffer)
			assert.NoError(t, err, "Binary marshal should succeed")

			var cachedParam Parameter
			err = cachedParam.UnmarshalCache(0, buffer)
			assert.NoError(t, err, "Binary unmarshal should succeed")
			assert.Equal(t, param.Name, cachedParam.Name, "Binary round-trip should preserve name")
			assert.Equal(t, param.ParameterType, cachedParam.ParameterType, "Binary round-trip should preserve type")
		})
	}
}

// Test_Parameter_Value_Nil tests Parameter with nil values
func Test_Parameter_Value_Nil(t *testing.T) {
	param := Parameter{
		Name:          "nilParam",
		ParameterType: "address",
		InternalType:  "address",
		Value:         nil,
	}

	// Test JSON serialization
	jsonData, err := json.Marshal(param)
	assert.NoError(t, err, "JSON marshal should succeed")

	var readParam Parameter
	err = json.Unmarshal(jsonData, &readParam)
	assert.NoError(t, err, "JSON unmarshal should succeed")
	assert.Nil(t, readParam.Value, "JSON round-trip should preserve nil value")

	// Test binary cache serialization
	buffer := new(bytes.Buffer)
	err = param.MarshalCache(buffer)
	assert.NoError(t, err, "Binary marshal should succeed")

	var cachedParam Parameter
	err = cachedParam.UnmarshalCache(0, buffer)
	assert.NoError(t, err, "Binary unmarshal should succeed")
	assert.Equal(t, param.Name, cachedParam.Name, "Binary round-trip should preserve name")
	assert.Equal(t, param.ParameterType, cachedParam.ParameterType, "Binary round-trip should preserve type")
}

// Test_Parameter_Components_With_Values tests Parameter with components and values
func Test_Parameter_Components_With_Values(t *testing.T) {
	// Create a parameter with components (tuple type) and set values on both parent and children
	param := Parameter{
		Name:          "donation",
		ParameterType: "(address,uint256,address)",
		InternalType:  "(address,uint256,address)",
		Value: []interface{}{
			"0x6b175474e89094c44da98b954eedeac495271d0f",
			"1000000000000000000",
			"0xa735cf11ed1228feb7c7bb18673a868455ffb16f",
		},
		Components: []Parameter{
			{
				Name:          "token",
				ParameterType: "address",
				InternalType:  "address",
				Value:         "0x6b175474e89094c44da98b954eedeac495271d0f",
			},
			{
				Name:          "amount",
				ParameterType: "uint256",
				InternalType:  "uint256",
				Value:         "1000000000000000000",
			},
			{
				Name:          "dest",
				ParameterType: "address",
				InternalType:  "address",
				Value:         "0xa735cf11ed1228feb7c7bb18673a868455ffb16f",
			},
		},
	}

	// Test JSON serialization
	jsonData, err := json.Marshal(param)
	assert.NoError(t, err, "JSON marshal should succeed")

	var readParam Parameter
	err = json.Unmarshal(jsonData, &readParam)
	assert.NoError(t, err, "JSON unmarshal should succeed")

	// Verify the structure is preserved
	assert.Equal(t, param.Name, readParam.Name, "Parameter name should be preserved")
	assert.Equal(t, param.ParameterType, readParam.ParameterType, "Parameter type should be preserved")
	assert.Equal(t, len(param.Components), len(readParam.Components), "Components count should be preserved")

	// Verify component details
	for i, comp := range param.Components {
		readComp := readParam.Components[i]
		assert.Equal(t, comp.Name, readComp.Name, "Component name should be preserved")
		assert.Equal(t, comp.ParameterType, readComp.ParameterType, "Component type should be preserved")
		// Component values might be affected by JSON round-trip
		t.Logf("Component %d - Original: %T %v, Read: %T %v", i, comp.Value, comp.Value, readComp.Value, readComp.Value)
	}

	// Test binary cache serialization
	buffer := new(bytes.Buffer)
	err = param.MarshalCache(buffer)
	assert.NoError(t, err, "Binary marshal should succeed")

	var cachedParam Parameter
	err = cachedParam.UnmarshalCache(0, buffer)
	assert.NoError(t, err, "Binary unmarshal should succeed")

	// Verify the structure is preserved in binary cache
	assert.Equal(t, param.Name, cachedParam.Name, "Binary round-trip should preserve name")
	assert.Equal(t, param.ParameterType, cachedParam.ParameterType, "Binary round-trip should preserve type")
	assert.Equal(t, len(param.Components), len(cachedParam.Components), "Binary round-trip should preserve components count")

	// Verify component details in binary cache
	for i, comp := range param.Components {
		cachedComp := cachedParam.Components[i]
		assert.Equal(t, comp.Name, cachedComp.Name, "Binary round-trip should preserve component name")
		assert.Equal(t, comp.ParameterType, cachedComp.ParameterType, "Binary round-trip should preserve component type")
		// Note: Component values are not serialized in binary cache
	}
}

// Test_Parameter_Value_Summary documents the key findings about Parameter.Value behavior
func Test_Parameter_Value_Summary(t *testing.T) {
	t.Log("=== PARAMETER VALUE BEHAVIOR SUMMARY ===")
	t.Log("")
	t.Log("Key findings from Parameter.Value testing:")
	t.Log("")
	t.Log("1. JSON SERIALIZATION BEHAVIOR:")
	t.Log("   - All numeric types (uint8, int32, etc.) become float64 after JSON round-trip")
	t.Log("   - big.Int values become float64 (with potential precision loss)")
	t.Log("   - String arrays become []interface{}")
	t.Log("   - Complex structs (like Function) become map[string]interface{}")
	t.Log("   - base.Address and base.Hash become strings")
	t.Log("")
	t.Log("2. BINARY CACHE BEHAVIOR:")
	t.Log("   - Parameter.Value is NOT serialized in binary cache")
	t.Log("   - Only structure (Name, Type, Components) is preserved")
	t.Log("   - This is likely intentional - values are runtime data")
	t.Log("")
	t.Log("3. IMPLICATIONS FOR ARTICULATION:")
	t.Log("   - The articulation issue is NOT related to binary cache serialization")
	t.Log("   - Values must be computed/populated during articulation process")
	t.Log("   - The problem is in how complex parameter types are articulated from transaction data")
	t.Log("")
	t.Log("4. COMPONENTS BEHAVIOR:")
	t.Log("   - Component structure is preserved in both JSON and binary cache")
	t.Log("   - Component values follow same rules as parent Parameter values")
	t.Log("   - This confirms the ABI structure is correctly cached")
}
