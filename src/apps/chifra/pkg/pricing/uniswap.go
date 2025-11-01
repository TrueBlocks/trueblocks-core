package pricing

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/call"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

var (
	daiAddress                = base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f") // DAI
	wethAddress               = base.HexToAddress("0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2") // WETH
	uniswapFactoryV2          = base.HexToAddress("0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f")
	uniswapFactoryV2_deployed = base.Blknum(10000835) // why query for this immutable value each time we need it?
)

// priceUsdUniswap returns the price of the given asset in USD as of the given block number.
func priceUsdUniswap(conn *rpc.Connection, statement *types.Statement) (price base.Float, source string, err error) {
	multiplier := *base.OneFloat
	var first base.Address
	var second base.Address
	if statement.IsEth() {
		first = daiAddress
		second = wethAddress

	} else {
		temp := *statement
		temp.Asset = base.FAKE_ETH_ADDRESS
		temp.Symbol = "WEI"
		multiplier, _, err = priceUsdUniswap(conn, &temp)
		if err != nil {
			return *base.ZeroFloat, "not-priced", err
		}
		first = wethAddress
		second = statement.Asset
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
		wrapped := fmt.Errorf("the --calldata value provided (%s) was not found: %s", theCall1, err)
		return *base.ZeroFloat, "not-priced", wrapped
	}
	contractCall.BlockNumber = statement.BlockNumber

	artFunc := func(str string, function *types.Function) error {
		return articulate.ArticulateFunction(function, "", str[2:])
	}
	result, err := contractCall.Call(artFunc)
	if err != nil {
		return *base.ZeroFloat, "not-priced", err
	}
	pairAddress := base.HexToAddress(result.Values["val_0"])
	if pairAddress.IsZero() {
		msg := fmt.Sprintf("no pair found for %s and %s", first.Hex(), second.Hex())
		return *base.ZeroFloat, "not-priced", errors.New(msg)
	}
	theCall2 := "getReserves()"
	contractCall, _, err = call.NewContractCall(conn, pairAddress, theCall2)
	if err != nil {
		wrapped := fmt.Errorf("the --calldata value provided (%s) was not found: %s", theCall2, err)
		return *base.ZeroFloat, "not-priced", wrapped
	}
	contractCall.BlockNumber = statement.BlockNumber
	result, err = contractCall.Call(artFunc)
	if err != nil {
		return *base.ZeroFloat, "not-priced", err
	}
	reserve0 := new(base.Float)
	if result.Values != nil && (result.Values["_reserve0"] == "" || result.Values["_reserve0"] == "0") {
		reserve0.SetString("1")
	} else {
		reserve0.SetString(result.Values["_reserve0"])
	}
	reserve1 := new(base.Float)
	if result.Values != nil && (result.Values["_reserve1"] == "" || result.Values["_reserve1"] == "0") {
		reserve1.SetString("1")
	} else {
		reserve1.SetString(result.Values["_reserve1"])
	}
	bigPrice := new(base.Float).Quo(reserve0, reserve1)
	price = *bigPrice
	price = *new(base.Float).Mul(&price, &multiplier)
	source = "uniswap"

	r := priceDebugger{
		address:     statement.Asset,
		symbol:      statement.Symbol,
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
		float2:      &multiplier,
		bigPrice:    bigPrice,
		price:       price,
		source:      source,
	}
	r.report("using Uniswap")

	return price, source, nil
}
