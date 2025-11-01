// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package types

import (
	"bytes"
	"encoding/json"
	"os"
	"path/filepath"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/cache"
	"github.com/stretchr/testify/assert"
)

// Donate function ABI definition
const donateAbiJson = `{
	"encoding": "0x9120491c",
	"inputs": [
		{
			"internalType": "(address,uint256,address)[]",
			"name": "_donations",
			"type": "(address,uint256,address)[]"
		}
	],
	"name": "donate",
	"outputs": [],
	"signature": "donate((address,uint256,address)[])",
	"stateMutability": "payable",
	"type": "function"
}`

// Operate function ABI definition
const operateAbiJson = `{
	"encoding": "0xa67a6a45",
	"inputs": [
		{
			"internalType": "(address,uint256)[]",
			"name": "accounts",
			"type": "(address,uint256)[]"
		},
		{
			"internalType": "(uint8,uint256,(bool,uint8,uint8,uint256),uint256,uint256,address,uint256,bytes)[]",
			"name": "actions",
			"type": "(uint8,uint256,(bool,uint8,uint8,uint256),uint256,uint256,address,uint256,bytes)[]"
		}
	],
	"name": "operate",
	"outputs": [],
	"signature": "operate((address,uint256)[],(uint8,uint256,(bool,uint8,uint8,uint256),uint256,uint256,address,uint256,bytes)[])",
	"stateMutability": "nonpayable",
	"type": "function"
}`

// Test_Operate_Binary_Cache tests complex nested components in the operate function
func Test_Operate_Binary_Cache(t *testing.T) {
	// Create a temporary directory for our test files
	tempDir, err := os.MkdirTemp("", "operate_cache_test")
	if err != nil {
		t.Fatalf("Failed to create temp dir: %v", err)
	}
	defer os.RemoveAll(tempDir)

	// 1. Parse the operate function from JSON
	var operateFunc Function
	err = json.Unmarshal([]byte(operateAbiJson), &operateFunc)
	if err != nil {
		t.Fatalf("Failed to parse operate function JSON: %v", err)
	}

	// 2. Create an ABI with the operate function
	abi := &Abi{
		Address:   base.HexToAddress("0x1e0447b19bb6ecfdae1e4ae1694b0c3659614e4e"),
		Name:      "OperateAbi",
		Functions: []Function{operateFunc},
	}

	// 3. Set up an in-memory cache store
	store, err := cache.NewStore(&cache.StoreOptions{
		Location: cache.MemoryCache,
		RootDir:  tempDir,
	})
	if err != nil {
		t.Fatalf("Failed to create cache store: %v", err)
	}

	// 4. Write the ABI to the cache
	err = store.Write(abi)
	if err != nil {
		t.Fatalf("Failed to write ABI to cache: %v", err)
	}

	// 5. Create a new empty ABI to read into
	readAbi := &Abi{
		Address: abi.Address, // We need the address to locate the item in cache
	}

	// 6. Read the ABI from the cache
	err = store.ReadFromStore(readAbi)
	if err != nil {
		t.Fatalf("Failed to read ABI from cache: %v", err)
	}

	// 7. Verify that the read ABI matches the original
	assert.Equal(t, abi.Address, readAbi.Address, "Addresses should match")
	assert.Equal(t, abi.Name, readAbi.Name, "Names should match")
	assert.Equal(t, len(abi.Functions), len(readAbi.Functions), "Function count should match")
	assert.Equal(t, abi.Functions[0].Name, readAbi.Functions[0].Name, "Function names should match")
	assert.Equal(t, abi.Functions[0].Signature, readAbi.Functions[0].Signature, "Function signatures should match")

	// 8. Check the nested Parameters structure for the operate function
	// This is a very complex structure, so we need detailed verification
	assert.Equal(t, len(abi.Functions[0].Inputs), len(readAbi.Functions[0].Inputs), "Input count should match")

	// 9. Check the first parameter (accounts array)
	originalParam1 := abi.Functions[0].Inputs[0]
	readParam1 := readAbi.Functions[0].Inputs[0]
	assert.Equal(t, originalParam1.Name, readParam1.Name, "Parameter names should match")
	assert.Equal(t, originalParam1.ParameterType, readParam1.ParameterType, "Parameter types should match")
	assert.Equal(t, originalParam1.InternalType, readParam1.InternalType, "Parameter internal types should match")

	// 10. Check the second parameter (actions array with very complex nested structure)
	originalParam2 := abi.Functions[0].Inputs[1]
	readParam2 := readAbi.Functions[0].Inputs[1]
	assert.Equal(t, originalParam2.Name, readParam2.Name, "Parameter names should match")
	assert.Equal(t, originalParam2.ParameterType, readParam2.ParameterType, "Parameter types should match")
	assert.Equal(t, originalParam2.InternalType, readParam2.InternalType, "Parameter internal types should match")

	// 11. Specifically test marshaling/unmarshaling the second parameter directly
	// as it has the most complex structure
	buffer := new(bytes.Buffer)
	err = originalParam2.MarshalCache(buffer)
	if err != nil {
		t.Fatalf("Failed to marshal complex parameter: %v", err)
	}

	var readComplexParam Parameter
	err = readComplexParam.UnmarshalCache(0, buffer) // 0 means latest version
	if err != nil {
		t.Fatalf("Failed to unmarshal complex parameter: %v", err)
	}

	assert.Equal(t, originalParam2.Name, readComplexParam.Name, "Complex parameter names should match after direct marshal/unmarshal")
	assert.Equal(t, originalParam2.ParameterType, readComplexParam.ParameterType, "Complex parameter types should match after direct marshal/unmarshal")
	assert.Equal(t, originalParam2.InternalType, readComplexParam.InternalType, "Complex parameter internal types should match after direct marshal/unmarshal")
}

// Test_Abi_JSON_ReadWrite tests reading and writing the ABI JSON
func Test_Abi_JSON_ReadWrite(t *testing.T) {
	// Create a temporary directory for our test files
	tempDir, err := os.MkdirTemp("", "abi_test")
	if err != nil {
		t.Fatalf("Failed to create temp dir: %v", err)
	}
	defer os.RemoveAll(tempDir)

	// Parse the donate function from JSON
	var donateFunc Function
	err = json.Unmarshal([]byte(donateAbiJson), &donateFunc)
	if err != nil {
		t.Fatalf("Failed to parse donate function JSON: %v", err)
	}

	// Create an ABI with the donate function
	abi := &Abi{
		Address:   base.HexToAddress("0x7d655c57f71464b6f83811c55d84009cd9f5221c"),
		Name:      "TestAbi",
		Functions: []Function{donateFunc},
	}

	// 1. Test writing the ABI to a JSON file
	jsonFilePath := filepath.Join(tempDir, "test_abi.json")
	abiBytes, err := json.MarshalIndent(abi, "", "  ")
	if err != nil {
		t.Fatalf("Failed to marshal ABI to JSON: %v", err)
	}

	err = os.WriteFile(jsonFilePath, abiBytes, 0644)
	if err != nil {
		t.Fatalf("Failed to write ABI to file: %v", err)
	}

	// 2. Test reading the ABI from the JSON file
	jsonData, err := os.ReadFile(jsonFilePath)
	if err != nil {
		t.Fatalf("Failed to read ABI file: %v", err)
	}

	var readAbi Abi
	err = json.Unmarshal(jsonData, &readAbi)
	if err != nil {
		t.Fatalf("Failed to unmarshal ABI from JSON: %v", err)
	}

	// 3. Verify that the read ABI matches the original
	assert.Equal(t, abi.Address, readAbi.Address, "Addresses should match")
	assert.Equal(t, abi.Name, readAbi.Name, "Names should match")
	assert.Equal(t, len(abi.Functions), len(readAbi.Functions), "Function count should match")
	assert.Equal(t, abi.Functions[0].Name, readAbi.Functions[0].Name, "Function names should match")
	assert.Equal(t, abi.Functions[0].Signature, readAbi.Functions[0].Signature, "Function signatures should match")

	// 4. Check the nested Parameters structure
	assert.Equal(t, len(abi.Functions[0].Inputs), len(readAbi.Functions[0].Inputs), "Input count should match")

	// Check the complex parameter type
	originalParam := abi.Functions[0].Inputs[0]
	readParam := readAbi.Functions[0].Inputs[0]
	assert.Equal(t, originalParam.Name, readParam.Name, "Parameter names should match")
	assert.Equal(t, originalParam.ParameterType, readParam.ParameterType, "Parameter types should match")
	assert.Equal(t, originalParam.InternalType, readParam.InternalType, "Parameter internal types should match")
}

// Test_Abi_Binary_Cache tests writing and reading the ABI to/from the binary cache
func Test_Abi_Binary_Cache(t *testing.T) {
	// Create a temporary directory for our test files
	tempDir, err := os.MkdirTemp("", "abi_cache_test")
	if err != nil {
		t.Fatalf("Failed to create temp dir: %v", err)
	}
	defer os.RemoveAll(tempDir)

	// 1. Parse the donate function from JSON
	var donateFunc Function
	err = json.Unmarshal([]byte(donateAbiJson), &donateFunc)
	if err != nil {
		t.Fatalf("Failed to parse donate function JSON: %v", err)
	}

	// 2. Create an ABI with the donate function
	abi := &Abi{
		Address:   base.HexToAddress("0x7d655c57f71464b6f83811c55d84009cd9f5221c"),
		Name:      "TestAbi",
		Functions: []Function{donateFunc},
	}

	// 3. Set up an in-memory cache store
	store, err := cache.NewStore(&cache.StoreOptions{
		Location: cache.MemoryCache,
		RootDir:  tempDir,
	})
	if err != nil {
		t.Fatalf("Failed to create cache store: %v", err)
	}

	// 4. Write the ABI to the cache
	err = store.Write(abi)
	if err != nil {
		t.Fatalf("Failed to write ABI to cache: %v", err)
	}

	// 5. Create a new empty ABI to read into
	readAbi := &Abi{
		Address: abi.Address, // We need the address to locate the item in cache
	}

	// 6. Read the ABI from the cache
	err = store.ReadFromStore(readAbi)
	if err != nil {
		t.Fatalf("Failed to read ABI from cache: %v", err)
	}

	// 7. Verify that the read ABI matches the original
	assert.Equal(t, abi.Address, readAbi.Address, "Addresses should match")
	assert.Equal(t, abi.Name, readAbi.Name, "Names should match")
	assert.Equal(t, len(abi.Functions), len(readAbi.Functions), "Function count should match")
	assert.Equal(t, abi.Functions[0].Name, readAbi.Functions[0].Name, "Function names should match")
	assert.Equal(t, abi.Functions[0].Signature, readAbi.Functions[0].Signature, "Function signatures should match")

	// 8. Check the nested Parameters structure
	assert.Equal(t, len(abi.Functions[0].Inputs), len(readAbi.Functions[0].Inputs), "Input count should match")

	// 9. Check the complex parameter type
	originalParam := abi.Functions[0].Inputs[0]
	readParam := readAbi.Functions[0].Inputs[0]
	assert.Equal(t, originalParam.Name, readParam.Name, "Parameter names should match")
	assert.Equal(t, originalParam.ParameterType, readParam.ParameterType, "Parameter types should match")
	assert.Equal(t, originalParam.InternalType, readParam.InternalType, "Parameter internal types should match")
}

// Test_Binary_Cache_Components tests handling of nested Components in the binary cache
func Test_Binary_Cache_Components(t *testing.T) {
	// Create a temporary directory for our test files
	tempDir, err := os.MkdirTemp("", "component_cache_test")
	if err != nil {
		t.Fatalf("Failed to create temp dir: %v", err)
	}
	defer os.RemoveAll(tempDir)

	// 1. Create a parameter with nested components
	// This is similar to the structure in the donate function
	param := Parameter{
		Name:          "_donations",
		ParameterType: "(address,uint256,address)[]",
		InternalType:  "(address,uint256,address)[]",
		Components: []Parameter{
			{
				Name:          "token",
				ParameterType: "address",
				InternalType:  "address",
			},
			{
				Name:          "amount",
				ParameterType: "uint256",
				InternalType:  "uint256",
			},
			{
				Name:          "dest",
				ParameterType: "address",
				InternalType:  "address",
			},
		},
	}

	// 2. Test binary serialization and deserialization directly
	// Serialize to a buffer
	buffer := new(bytes.Buffer)
	err = param.MarshalCache(buffer)
	if err != nil {
		t.Fatalf("Failed to marshal parameter: %v", err)
	}

	// Deserialize from the buffer
	var readParam Parameter
	err = readParam.UnmarshalCache(0, buffer) // 0 means latest version
	if err != nil {
		t.Fatalf("Failed to unmarshal parameter: %v", err)
	}

	// 3. Verify the nested components were properly serialized/deserialized
	assert.Equal(t, param.Name, readParam.Name, "Parameter names should match")
	assert.Equal(t, param.ParameterType, readParam.ParameterType, "Parameter types should match")
	assert.Equal(t, param.InternalType, readParam.InternalType, "Parameter internal types should match")
	assert.Equal(t, len(param.Components), len(readParam.Components), "Component count should match")

	// Check each component
	for i, comp := range param.Components {
		readComp := readParam.Components[i]
		assert.Equal(t, comp.Name, readComp.Name, "Component names should match")
		assert.Equal(t, comp.ParameterType, readComp.ParameterType, "Component types should match")
		assert.Equal(t, comp.InternalType, readComp.InternalType, "Component internal types should match")
	}
}
