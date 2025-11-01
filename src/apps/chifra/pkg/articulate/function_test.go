package articulate

import (
	"encoding/json"
	"fmt"
	"math/big"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
	"github.com/ethereum/go-ethereum/accounts/abi"
)

var ensRegistrar = `[{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"releaseDeed","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"getAllowedTime","outputs":[{"name":"timestamp","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"unhashedName","type":"string"}],"name":"invalidateName","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"hash","type":"bytes32"},{"name":"owner","type":"address"},{"name":"value","type":"uint256"},{"name":"salt","type":"bytes32"}],"name":"shaBid","outputs":[{"name":"sealedBid","type":"bytes32"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"bidder","type":"address"},{"name":"seal","type":"bytes32"}],"name":"cancelBid","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"entries","outputs":[{"name":"","type":"uint8"},{"name":"","type":"address"},{"name":"","type":"uint256"},{"name":"","type":"uint256"},{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"ens","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"},{"name":"_value","type":"uint256"},{"name":"_salt","type":"bytes32"}],"name":"unsealBid","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"transferRegistrars","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"bytes32"}],"name":"sealedBids","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"state","outputs":[{"name":"","type":"uint8"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"},{"name":"newOwner","type":"address"}],"name":"transfer","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_hash","type":"bytes32"},{"name":"_timestamp","type":"uint256"}],"name":"isAllowed","outputs":[{"name":"allowed","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"finalizeAuction","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"registryStarted","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"launchLength","outputs":[{"name":"","type":"uint32"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"sealedBid","type":"bytes32"}],"name":"newBid","outputs":[],"payable":true,"type":"function"},{"constant":false,"inputs":[{"name":"labels","type":"bytes32[]"}],"name":"eraseNode","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hashes","type":"bytes32[]"}],"name":"startAuctions","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"hash","type":"bytes32"},{"name":"deed","type":"address"},{"name":"registrationDate","type":"uint256"}],"name":"acceptRegistrarTransfer","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_hash","type":"bytes32"}],"name":"startAuction","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"rootNode","outputs":[{"name":"","type":"bytes32"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"hashes","type":"bytes32[]"},{"name":"sealedBid","type":"bytes32"}],"name":"startAuctionsAndBid","outputs":[],"payable":true,"type":"function"},{"inputs":[{"name":"_ens","type":"address"},{"name":"_rootNode","type":"bytes32"},{"name":"_startDate","type":"uint256"}],"payable":false,"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":false,"name":"registrationDate","type":"uint256"}],"name":"AuctionStarted","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":true,"name":"bidder","type":"address"},{"indexed":false,"name":"deposit","type":"uint256"}],"name":"NewBid","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":true,"name":"owner","type":"address"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"status","type":"uint8"}],"name":"BidRevealed","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":true,"name":"owner","type":"address"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"registrationDate","type":"uint256"}],"name":"HashRegistered","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":false,"name":"value","type":"uint256"}],"name":"HashReleased","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"hash","type":"bytes32"},{"indexed":true,"name":"name","type":"string"},{"indexed":false,"name":"value","type":"uint256"},{"indexed":false,"name":"registrationDate","type":"uint256"}],"name":"HashInvalidated","type":"event"}]`

func TestArticulateFunction(t *testing.T) {
	const abiJson = `[{"constant":false,"inputs":[{"name":"memo","type":"bytes"}],"name":"receive","outputs":[],"payable":true,"stateMutability":"payable","type":"function"},{"constant":false,"inputs":[],"name":"send","outputs":[{"name":"amount","type":"uint256"}],"payable":true,"stateMutability":"payable","type":"function"},{"constant":false,"inputs":[{"name":"addr","type":"address"}],"name":"get","outputs":[{"name":"hash","type":"bytes"}],"payable":true,"stateMutability":"payable","type":"function"}]`
	abi, err := abi.JSON(strings.NewReader(abiJson))
	if err != nil {
		t.Fatal(err)
	}
	const hexdata = `00000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000000000000000000015800000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000158000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000001580000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000000000000000000015800000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000158`

	am := abi.Methods["get"]
	f := types.FunctionFromAbiMethod(&am)
	if err = ArticulateFunction(f, hexdata, ""); err != nil {
		t.Fatal(err)
	}

	if ilen := len(f.Inputs); ilen > 1 {
		t.Fatal("wrong input count", ilen)
	}
	if paramType := f.Inputs[0].ParameterType; paramType != "address" {
		t.Fatal("wrong parameter type", paramType)
	}
	if value := f.Inputs[0].Value; value != "0x0000000000000000000000000000000000000001" {
		t.Fatal("wrong value", value)
	}
}

func TestArticulateArguments(t *testing.T) {
	abiJson := `[{"inputs":[{"internalType":"uint256","name":"chainId_","type":"uint256"}],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"internalType":"address","name":"src","type":"address"},{"indexed":true,"internalType":"address","name":"guy","type":"address"},{"indexed":false,"internalType":"uint256","name":"wad","type":"uint256"}],"name":"Approval","type":"event"},{"anonymous":true,"inputs":[{"indexed":true,"internalType":"bytes4","name":"sig","type":"bytes4"},{"indexed":true,"internalType":"address","name":"usr","type":"address"},{"indexed":true,"internalType":"bytes32","name":"arg1","type":"bytes32"},{"indexed":true,"internalType":"bytes32","name":"arg2","type":"bytes32"},{"indexed":false,"internalType":"bytes","name":"data","type":"bytes"}],"name":"LogNote","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"internalType":"address","name":"src","type":"address"},{"indexed":true,"internalType":"address","name":"dst","type":"address"},{"indexed":false,"internalType":"uint256","name":"wad","type":"uint256"}],"name":"Transfer","type":"event"},{"constant":true,"inputs":[],"name":"DOMAIN_SEPARATOR","outputs":[{"internalType":"bytes32","name":"","type":"bytes32"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"PERMIT_TYPEHASH","outputs":[{"internalType":"bytes32","name":"","type":"bytes32"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"internalType":"address","name":"","type":"address"},{"internalType":"address","name":"","type":"address"}],"name":"allowance","outputs":[{"internalType":"uint256","name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"internalType":"address","name":"usr","type":"address"},{"internalType":"uint256","name":"wad","type":"uint256"}],"name":"approve","outputs":[{"internalType":"bool","name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"internalType":"address","name":"","type":"address"}],"name":"balanceOf","outputs":[{"internalType":"uint256","name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"internalType":"address","name":"usr","type":"address"},{"internalType":"uint256","name":"wad","type":"uint256"}],"name":"burn","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"internalType":"uint8","name":"","type":"uint8"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"internalType":"address","name":"guy","type":"address"}],"name":"deny","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"internalType":"address","name":"usr","type":"address"},{"internalType":"uint256","name":"wad","type":"uint256"}],"name":"mint","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"internalType":"address","name":"src","type":"address"},{"internalType":"address","name":"dst","type":"address"},{"internalType":"uint256","name":"wad","type":"uint256"}],"name":"move","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"name","outputs":[{"internalType":"string","name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"internalType":"address","name":"","type":"address"}],"name":"nonces","outputs":[{"internalType":"uint256","name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"internalType":"address","name":"holder","type":"address"},{"internalType":"address","name":"spender","type":"address"},{"internalType":"uint256","name":"nonce","type":"uint256"},{"internalType":"uint256","name":"expiry","type":"uint256"},{"internalType":"bool","name":"allowed","type":"bool"},{"internalType":"uint8","name":"v","type":"uint8"},{"internalType":"bytes32","name":"r","type":"bytes32"},{"internalType":"bytes32","name":"s","type":"bytes32"}],"name":"permit","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"internalType":"address","name":"usr","type":"address"},{"internalType":"uint256","name":"wad","type":"uint256"}],"name":"pull","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"internalType":"address","name":"usr","type":"address"},{"internalType":"uint256","name":"wad","type":"uint256"}],"name":"push","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"internalType":"address","name":"guy","type":"address"}],"name":"rely","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"internalType":"string","name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"internalType":"uint256","name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"internalType":"address","name":"dst","type":"address"},{"internalType":"uint256","name":"wad","type":"uint256"}],"name":"transfer","outputs":[{"internalType":"bool","name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"internalType":"address","name":"src","type":"address"},{"internalType":"address","name":"dst","type":"address"},{"internalType":"uint256","name":"wad","type":"uint256"}],"name":"transferFrom","outputs":[{"internalType":"bool","name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"version","outputs":[{"internalType":"string","name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"internalType":"address","name":"","type":"address"}],"name":"wards","outputs":[{"internalType":"uint256","name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"}]`
	abi, err := abi.JSON(strings.NewReader(abiJson))
	if err != nil {
		t.Fatal(err)
	}
	input := `0xa9059cbb000000000000000000000000f5aab2d0b50cb3bf2b6a5a9ed18580fd736668be000000000000000000000000000000000000000000000199d413696741200000`

	abiMethod := abi.Methods["transfer"]
	f := types.FunctionFromAbiMethod(&abiMethod)
	if err = articulateArguments(abiMethod.Inputs, input[10:], nil, f.Inputs); err != nil {
		t.Fatal(err)
	}

	if paramType := f.Inputs[0].ParameterType; paramType != "address" {
		t.Fatal("wrong type of the first parameter", paramType)
	}
	if value := f.Inputs[0].Value; value != "0xf5aab2d0b50cb3bf2b6a5a9ed18580fd736668be" {
		t.Fatal("wrong value of the first parameter", value)
	}

	if paramType := f.Inputs[1].ParameterType; paramType != "uint256" {
		t.Fatal("wrong type of the second parameter", paramType)
	}
	if value := f.Inputs[1].Value; value != "7560000000000000000000" {
		t.Fatal("wrong value of the second parameter", value)
	}
}

func TestArticulateArgumentsMixedIndexed(t *testing.T) {
	abi, err := abi.JSON(strings.NewReader(ensRegistrar))
	if err != nil {
		t.Fatal(err)
	}
	txData := `0x000000000000000000000000000000000000000000000000000000005911a79a`
	txTopics := []base.Hash{
		base.HexToHash("0x87e97e825a1d1fa0c54e1d36c7506c1dea8b1efd451fe68b000cf96f7cf40003"),
		base.HexToHash("0x000bf9f2adc93a1da7b9e61f44ee6504f99c467a2812b354d70a07f0b3cdc58c"),
	}
	abiEvent := abi.Events["AuctionStarted"]
	result := types.FunctionFromAbiEvent(&abiEvent)

	if err = articulateArguments(abiEvent.Inputs, txData[2:], txTopics, result.Inputs); err != nil {
		t.Fatal(err)
	}

	argNameToResultParam := make(map[string]types.Parameter)
	for _, param := range result.Inputs {
		argNameToResultParam[param.Name] = param
	}

	// Indexed
	if value := argNameToResultParam["hash"].Value; value != "0x000bf9f2adc93a1da7b9e61f44ee6504f99c467a2812b354d70a07f0b3cdc58c" {
		t.Fatal("wrong hash value:", value)
	}

	// Nonindexed
	if value := argNameToResultParam["registrationDate"].Value; value != "1494329242" {
		t.Fatal("wrong registrationDate value:", value)
	}
}

// We use this type alias and the following function only to encapsulate the big .Int
// type. Note that unlike elsewhere, we need to use the big .Int as an alias because
// the Go Ethereum code expects big.Ints for int256 types.
type myBig = big.Int

func newMyBig(v int64) *myBig { return big.NewInt(v) }

func TestArticulateArgumentsData(t *testing.T) {
	parsedAbi, err := abi.JSON(strings.NewReader(ensRegistrar))
	if err != nil {
		t.Fatal(err)
	}

	var abiMethod abi.Method
	var result *types.Function
	var expected any
	var packed []byte

	// type: hash
	abiMethod = parsedAbi.Methods["releaseDeed"]
	result = types.FunctionFromAbiMethod(&abiMethod)
	expected = "0x00120aa407bdbff1d93ea98dafc5f1da56b589b427167ec414bccbe0cfdfd573"
	packed, err = abiMethod.Inputs.Pack(expected)
	if err != nil {
		return
	}

	if err = articulateArguments(abiMethod.Inputs, string(packed), nil, result.Inputs); err != nil {
		t.Fatal(err)
	}
	if value := result.Inputs[0].Value; value != "0x00120aa407bdbff1d93ea98dafc5f1da56b589b427167ec414bccbe0cfdfd573" {
		t.Fatal("wrong result value:", value)
	}

	// type: uint256
	abiMethod = parsedAbi.Methods["getAllowedTime"]
	result = types.FunctionFromAbiMethod(&abiMethod)
	expected = newMyBig(123).String()
	packed, err = abiMethod.Inputs.Pack(newMyBig(123))
	if err != nil {
		return
	}
	if err = articulateArguments(abiMethod.Inputs, string(packed), nil, result.Inputs); err != nil {
		t.Fatal(err)
	}
	if value := result.Inputs[0].Value; value != expected {
		t.Fatal("wrong result value:", value)
	}

	// type: string
	abiMethod = parsedAbi.Methods["invalidateName"]
	result = types.FunctionFromAbiMethod(&abiMethod)
	expected = "some test string"
	packed, err = abiMethod.Inputs.Pack(expected)
	if err != nil {
		return
	}
	if err = articulateArguments(abiMethod.Inputs, string(packed), nil, result.Inputs); err != nil {
		t.Fatal(err)
	}
	if value := result.Inputs[0].Value; value != expected {
		t.Fatal("wrong result value:", value)
	}

	// type: address
	abiMethod = parsedAbi.Methods["cancelBid"]
	result = types.FunctionFromAbiMethod(&abiMethod)
	expected = "0x95222290dd7278aa3ddd389cc1e1d165cc4bafe5"
	packed, err = abiMethod.Inputs.Pack(expected)
	if err != nil {
		return
	}
	if err = articulateArguments(abiMethod.Inputs, string(packed), nil, result.Inputs); err != nil {
		t.Fatal(err)
	}
	if value := result.Inputs[0].Value; value != expected {
		t.Fatal("wrong result value:", value)
	}

	// type: uint8
	abiMethod = parsedAbi.Methods["state"]
	result = types.FunctionFromAbiMethod(&abiMethod)
	expected = 2
	packed, err = abiMethod.Inputs.Pack(expected)
	if err != nil {
		return
	}
	if err = articulateArguments(abiMethod.Inputs, string(packed), nil, result.Inputs); err != nil {
		t.Fatal(err)
	}
	if value := result.Inputs[0].Value; value != expected {
		t.Fatal("wrong result value:", value)
	}

	// type: bool
	abiMethod = parsedAbi.Methods["isAllowed"]
	result = types.FunctionFromAbiMethod(&abiMethod)
	expected = true
	packed, err = abiMethod.Inputs.Pack(expected)
	if err != nil {
		return
	}
	if err = articulateArguments(abiMethod.Inputs, string(packed), nil, result.Inputs); err != nil {
		t.Fatal(err)
	}
	if value := result.Inputs[0].Value; value != expected {
		t.Fatal("wrong result value:", value)
	}
}

func TestArticulateArgumentsSlice(t *testing.T) {
	abi, err := abi.JSON(strings.NewReader(ensRegistrar))
	if err != nil {
		t.Fatal(err)
	}
	txData := `0x0000000000000000000000000000000000000000000000000000000000000040cb93e7ddea88eb37f5419784b399cf13f7df44079d05905006044dd14bb898110000000000000000000000000000000000000000000000000000000000000003000bf9f2adc93a1da7b9e61f44ee6504f99c467a2812b354d70a07f0b3cdc58c0007cc5734453f8d7bbacd4b3a8e753250dc4a432aaa5be5b048c59e0b5ac5fc00120aa407bdbff1d93ea98dafc5f1da56b589b427167ec414bccbe0cfdfd573`
	expectedBytes, _ := json.Marshal(&[]string{
		"0x000bf9f2adc93a1da7b9e61f44ee6504f99c467a2812b354d70a07f0b3cdc58c",
		"0x0007cc5734453f8d7bbacd4b3a8e753250dc4a432aaa5be5b048c59e0b5ac5fc",
		"0x00120aa407bdbff1d93ea98dafc5f1da56b589b427167ec414bccbe0cfdfd573",
	})
	expected := string(expectedBytes)
	abiMethod := abi.Methods["startAuctionsAndBid"]
	result := types.FunctionFromAbiMethod(&abiMethod)

	if err = articulateArguments(abiMethod.Inputs, txData[2:], nil, result.Inputs); err != nil {
		t.Fatal(err)
	}

	if outputLen := len(result.Inputs); outputLen != 2 {
		t.Fatal("wrong output length:", outputLen)
	}

	got := result.Inputs[0]

	if name := got.Name; name != "hashes" {
		t.Fatal("wrong name", name)
	}
	if inputType := got.ParameterType; inputType != "bytes32[]" {
		t.Fatal("wrong type", inputType)
	}
	value, err := json.Marshal(&got.Value)
	if err != nil {
		t.Fatal("error while marshalling JSON:", err)
	}
	if string(value) != expected {
		t.Fatal("wrong value", string(value))
	}
}

func TestArticulateArgumentsComplex(t *testing.T) {
	// https://etherscan.io/tx/0xfa62ab036f36b4755bfa0d6e3e641d08daa7e41e95b0c1f2246485702319efb8
	// operate(tuple[] accounts,tuple[] actions)
	abiJson := `[{"constant":false,"inputs":[{"components":[{"name":"owner","type":"address"},{"name":"number","type":"uint256"}],"name":"accounts","type":"tuple[]"},{"components":[{"name":"actionType","type":"uint8"},{"name":"accountId","type":"uint256"},{"components":[{"name":"sign","type":"bool"},{"name":"denomination","type":"uint8"},{"name":"ref","type":"uint8"},{"name":"value","type":"uint256"}],"name":"amount","type":"tuple"},{"name":"primaryMarketId","type":"uint256"},{"name":"secondaryMarketId","type":"uint256"},{"name":"otherAddress","type":"address"},{"name":"otherAccountId","type":"uint256"},{"name":"data","type":"bytes"}],"name":"actions","type":"tuple[]"}],"name":"operate","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"}]`
	txData := `0x000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000000100000000000000000000000066215d23b8a247c80c2d1b7bef4befc2ab384bce0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010f0cf064dd59200000000000000000000000000000000000000000000000000000000000000000000300000000000000000000000000000000000000000000000000000000000000020000000000000000000000007a603dc3eb0f4e3883929ee15a3c86d2ac45f4450000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000016000000000000000000000000000000000000000000000000000000000000000e00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000045f783cce6b7ff23b2ab2d70e416cdb7d6055f510000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000000`
	abi, err := abi.JSON(strings.NewReader(abiJson))
	if err != nil {
		t.Fatal(err)
	}
	abiMethod := abi.Methods["operate"]
	result := types.FunctionFromAbiMethod(&abiMethod)

	if err = articulateArguments(abiMethod.Inputs, txData[2:], nil, result.Inputs); err != nil {
		t.Fatal(err)
	}

	if inputLength := len(result.Inputs); inputLength != 2 {
		t.Fatal("wrong input length:", inputLength)
	}

	first := result.Inputs[0]
	expectedMap := []map[string]any{{
		"number": "0",
		"owner":  "0x66215d23b8a247c80c2d1b7bef4befc2ab384bce",
	}}
	expected, _ := json.Marshal(&expectedMap)
	value, err := json.Marshal(&first.Value)
	if err != nil {
		t.Fatal("error while marshalling JSON:", err)
	}
	if string(value) != string(expected) {
		t.Fatal("wrong input #1 value:", string(value))
	}
}

func TestArticulateArgumentsTupleWrongType(t *testing.T) {
	// The below ABI is just like:
	// https://etherscan.io/tx/0xfa62ab036f36b4755bfa0d6e3e641d08daa7e41e95b0c1f2246485702319efb8
	// operate(tuple[] accounts,tuple[] actions)
	// but it uses bytes32 type for `owner` instead of address type. We should still articulate this as a hash
	abiJson := `[{"constant":false,"inputs":[{"components":[{"name":"owner","type":"bytes32"},{"name":"number","type":"uint256"}],"name":"accounts","type":"tuple[]"},{"components":[{"name":"actionType","type":"uint8"},{"name":"accountId","type":"uint256"},{"components":[{"name":"sign","type":"bool"},{"name":"denomination","type":"uint8"},{"name":"ref","type":"uint8"},{"name":"value","type":"uint256"}],"name":"amount","type":"tuple"},{"name":"primaryMarketId","type":"uint256"},{"name":"secondaryMarketId","type":"uint256"},{"name":"otherAddress","type":"address"},{"name":"otherAccountId","type":"uint256"},{"name":"data","type":"bytes"}],"name":"actions","type":"tuple[]"}],"name":"operate","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"}]`
	txData := `0x000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000000100000000000000000000000066215d23b8a247c80c2d1b7bef4befc2ab384bce0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010f0cf064dd59200000000000000000000000000000000000000000000000000000000000000000000300000000000000000000000000000000000000000000000000000000000000020000000000000000000000007a603dc3eb0f4e3883929ee15a3c86d2ac45f4450000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000016000000000000000000000000000000000000000000000000000000000000000e00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000045f783cce6b7ff23b2ab2d70e416cdb7d6055f510000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000000`
	abi, err := abi.JSON(strings.NewReader(abiJson))
	if err != nil {
		t.Fatal(err)
	}
	abiMethod := abi.Methods["operate"]
	result := types.FunctionFromAbiMethod(&abiMethod)

	if err = articulateArguments(abiMethod.Inputs, txData[2:], nil, result.Inputs); err != nil {
		t.Fatal(err)
	}

	if inputLength := len(result.Inputs); inputLength != 2 {
		t.Fatal("wrong input length:", inputLength)
	}

	first := result.Inputs[0]
	expectedMap := []map[string]any{{
		"number": "0",
		"owner":  "0x00000000000000000000000066215d23b8a247c80c2d1b7bef4befc2ab384bce",
	}}
	expected, _ := json.Marshal(&expectedMap)
	value, err := json.Marshal(&first.Value)
	if err != nil {
		t.Fatal("error while marshalling JSON:", err)
	}
	if string(value) != string(expected) {
		t.Fatal("wrong input #1 value:", string(value))
	}
}

func TestArticulateArgumentsTupleTuple(t *testing.T) {
	// This ABI comes from https://docs.soliditylang.org/en/latest/abi-spec.html#handling-tuple-types
	abiJson := `[
  {
    "name": "f",
    "type": "function",
    "inputs": [
      {
        "name": "s",
        "type": "tuple",
        "components": [
          {
            "name": "a",
            "type": "uint256"
          },
          {
            "name": "b",
            "type": "uint256[]"
          },
          {
            "name": "c",
            "type": "tuple[]",
            "components": [
              {
                "name": "x",
                "type": "uint256"
              },
              {
                "name": "y",
                "type": "uint256"
              }
            ]
          }
        ]
      },
      {
        "name": "t",
        "type": "tuple",
        "components": [
          {
            "name": "x",
            "type": "uint256"
          },
          {
            "name": "y",
            "type": "uint256"
          }
        ]
      },
      {
        "name": "a",
        "type": "uint256"
      }
    ],
    "outputs": []
  }
]`

	parsedAbi, err := abi.JSON(strings.NewReader(abiJson))
	if err != nil {
		t.Fatal(err)
	}
	abiMethod := parsedAbi.Methods["f"]
	result := types.FunctionFromAbiMethod(&abiMethod)

	first := struct {
		A *myBig   `json:"a"`
		B []*myBig `json:"b"`
		C []struct {
			X *myBig `json:"x"`
			Y *myBig `json:"y"`
		} `json:"c"`
	}{
		A: newMyBig(1),
		B: []*myBig{},
		C: []struct {
			X *myBig `json:"x"`
			Y *myBig `json:"y"`
		}{{
			X: newMyBig(1),
			Y: newMyBig(2),
		}},
	}
	second := struct {
		X *myBig `json:"x"`
		Y *myBig `json:"y"`
	}{
		X: newMyBig(1),
		Y: newMyBig(2),
	}
	third := newMyBig(3)

	payload, err := abiMethod.Inputs.Pack(first, second, third)
	if err != nil {
		t.Fatal(err)
	}
	txData := base.Bytes2Hex(payload)

	if err = articulateArguments(abiMethod.Inputs, txData, nil, result.Inputs); err != nil {
		t.Fatal(err)
	}

	if inputLength := len(result.Inputs); inputLength != 3 {
		t.Fatal("wrong input length:", inputLength)
	}

	expected, _ := json.Marshal(map[string]any{
		"a": "1",
		"b": []string{},
		"c": []map[string]string{
			{
				"x": "1",
				"y": "2",
			},
		},
	})
	value, err := json.Marshal(&result.Inputs[0].Value)
	if err != nil {
		t.Fatal("error while marshalling JSON:", err)
	}
	if string(value) != string(expected) {
		fmt.Println(string(expected))
		t.Fatal("wrong value of the first input:", string(value))
	}

	expected, _ = json.Marshal(map[string]string{
		"x": "1",
		"y": "2",
	})
	value, err = json.Marshal(&result.Inputs[1].Value)
	if err != nil {
		t.Fatal("error while marshalling JSON:", err)
	}
	if string(value) != string(expected) {
		t.Fatal("wrong value of the second input:", value)
	}

	expected, _ = json.Marshal(third)
	if value := result.Inputs[2].Value; value != string(expected) {
		t.Fatal("wrong value of the third input:", value)
	}
}

func TestArticulateAnonymousArguments(t *testing.T) {
	// peek() from 0x729d19f657bd0614b4985cf1d82531c67569197b
	abiJson := `[{"constant":true,"inputs":[],"name":"peek","outputs":[{"name":"","type":"bytes32"},{"name":"","type":"bool"}],"payable":false,"type":"function"}]`
	abi, err := abi.JSON(strings.NewReader(abiJson))
	if err != nil {
		t.Fatal(err)
	}
	output := `0x00000000000000000000000000000000000000000000002993a384ff8db780000000000000000000000000000000000000000000000000000000000000000001`

	abiMethod := abi.Methods["peek"]
	f := types.FunctionFromAbiMethod(&abiMethod)

	if err = articulateArguments(abiMethod.Outputs, output[2:], nil, f.Outputs); err != nil {
		t.Fatal(err)
	}

	if paramType := f.Outputs[0].ParameterType; paramType != "bytes32" {
		t.Fatal("wrong type of the first parameter", paramType)
	}
	if value := f.Outputs[0].Value; value != "0x00000000000000000000000000000000000000000000002993a384ff8db78000" {
		t.Fatal("wrong value of the first parameter", value)
	}

	if paramType := f.Outputs[1].ParameterType; paramType != "bool" {
		t.Fatal("wrong type of the second parameter", paramType)
	}
	if value := f.Outputs[1].Value; value != true {
		t.Fatal("wrong value of the second parameter", value)
	}
}
