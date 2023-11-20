package pricing

import (
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/call"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var (
	daiAddress                = base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f") // DAI
	wethAddress               = base.HexToAddress("0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2") // WETH
	uniswapFactoryV2          = base.HexToAddress("0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f")
	uniswapFactoryV2_deployed = base.Blknum(10000835) // why query for this immutable value each time we need it?
)

// PriceUsdUniswap returns the price of the given asset in USD as of the given block number.
func PriceUsdUniswap(conn *rpc.Connection, testMode bool, statement *types.SimpleStatement) (price float64, source string, err error) {
	multiplier := float64(1.0)
	var first base.Address
	var second base.Address
	if statement.IsEth() {
		first = daiAddress
		second = wethAddress

	} else {
		temp := *statement
		temp.AssetAddr = base.FAKE_ETH_ADDRESS
		temp.AssetSymbol = "WEI"
		multiplier, _, err = PriceUsdUniswap(conn, testMode, &temp)
		if err != nil {
			return 0.0, "not-priced", err
		}
		first = wethAddress
		second = statement.AssetAddr
	}

	reversed := false
	if first.Hex() > second.Hex() {
		save := second
		second = first
		first = save
		reversed = true
	}

	theCall1 := fmt.Sprintf("getPair(%s, %s)", first.Hex(), second.Hex())
	contractCall, _, err := call.NewContractCall(conn, uniswapFactoryV2, theCall1)
	if err != nil {
		wrapped := fmt.Errorf("the --call value provided (%s) was not found: %s", theCall1, err)
		return 0.0, "not-priced", wrapped
	}
	contractCall.BlockNumber = statement.BlockNumber

	artFunc := func(str string, function *types.SimpleFunction) error {
		return articulate.ArticulateFunction(function, "", str[2:])
	}
	result, err := contractCall.Call(artFunc)
	if err != nil {
		return 0.0, "not-priced", err
	}
	pairAddress := base.HexToAddress(result.Values["val_0"])
	if pairAddress.IsZero() {
		msg := fmt.Sprintf("no pair found for %s and %s", first.Hex(), second.Hex())
		return 0.0, "not-priced", fmt.Errorf(msg)
	}
	theCall2 := "getReserves()"
	contractCall, _, err = call.NewContractCall(conn, pairAddress, theCall2)
	if err != nil {
		wrapped := fmt.Errorf("the --call value provided (%s) was not found: %s", theCall2, err)
		return 0.0, "not-priced", wrapped
	}
	contractCall.BlockNumber = statement.BlockNumber
	result, err = contractCall.Call(artFunc)
	if err != nil {
		return 0.0, "not-priced", err
	}
	reserve0 := new(big.Float)
	if result.Values != nil && result.Values["_reserve0"] == "" {
		reserve0.SetString("1")
	} else {
		reserve0.SetString(result.Values["_reserve0"])
	}
	reserve1 := new(big.Float)
	if result.Values != nil && result.Values["_reserve1"] == "" {
		reserve0.SetString("1")
	} else {
		reserve1.SetString(result.Values["_reserve1"])
	}
	bigPrice := new(big.Float)
	bigPrice = bigPrice.Quo(reserve0, reserve1)

	price, _ = bigPrice.Float64()
	price *= multiplier
	source = "uniswap"

	r := priceDebugger{
		address:     statement.AssetAddr,
		symbol:      statement.AssetSymbol,
		blockNumber: statement.BlockNumber,
		source1:     uniswapFactoryV2,
		theCall1:    theCall1,
		source2:     pairAddress,
		theCall2:    theCall2,
		first:       first,
		second:      second,
		reversed:    reversed,
		float0:      reserve0,
		float1:      reserve1,
		float2:      new(big.Float).SetFloat64(multiplier),
		bigPrice:    bigPrice,
		price:       price,
		source:      source,
	}
	r.report("using Uniswap", testMode)

	return price, source, nil
}
