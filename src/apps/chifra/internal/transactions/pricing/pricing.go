package pricing

import (
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/call"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var (
	uniswapFactoryV2          = base.HexToAddress("0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f")
	uniswapFactoryV2_deployed = base.Blknum(10000835) // why query for this immutable value each time we need it?
)

// PriceUsd returns the price of the asset in USD
func PriceUsd(chain string, testMode bool, statement *types.SimpleStatement) (price float64, source string, err error) {
	if statement.BlockNumber <= uniswapFactoryV2_deployed {
		msg := fmt.Sprintf("Block %d is prior to deployment (%d) of Uniswap V2. Falling back to Maker", statement.BlockNumber, uniswapFactoryV2_deployed)
		logger.TestLog(true, msg)
		return 0.0, "maker", nil
	}
	multiplier := float64(1.0)
	var first base.Address
	var second base.Address
	if statement.AssetAddr == base.FAKE_ETH_ADDRESS {
		first = base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f")  // DAI
		second = base.HexToAddress("0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2") // WETH
	} else {
		temp := *statement
		temp.AssetAddr = base.FAKE_ETH_ADDRESS
		multiplier, _, err = PriceUsd(chain, testMode, &temp)
		if err != nil {
			return 0.0, "not-priced", err
		}
	}
	reversed := false
	if first.Hex() > second.Hex() {
		save := second
		second = first
		first = save
		reversed = true
	}

	theCall1 := fmt.Sprintf("getPair(%s, %s)", first.Hex(), second.Hex())
	contractCall, err := call.NewContractCall(chain, uniswapFactoryV2, theCall1, false)
	if err != nil {
		return 0.0, "not-priced", err
	}
	contractCall.BlockNumber = statement.BlockNumber
	result, _ := call.CallContract(chain, contractCall)
	pairAddress := base.HexToAddress(result.Outputs["val_0"])
	theCall2 := "getReserves()"
	contractCall, err = call.NewContractCall(chain, pairAddress, theCall2, false)
	if err != nil {
		return 0.0, "not-priced", err
	}
	contractCall.BlockNumber = statement.BlockNumber
	result, err = call.CallContract(chain, contractCall)
	if err != nil {
		return 0.0, "not-priced", err
	}
	reserve0 := new(big.Float)
	if result.Outputs != nil && result.Outputs["_reserve0"] == "" {
		reserve0.SetString("1")
	} else {
		reserve0.SetString(result.Outputs["_reserve0"])
	}
	reserve1 := new(big.Float)
	if result.Outputs != nil && result.Outputs["_reserve1"] == "" {
		reserve0.SetString("1")
	} else {
		reserve1.SetString(result.Outputs["_reserve1"])
	}
	bigPrice := new(big.Float)
	bigPrice.Quo(reserve0, reserve1)

	if testMode {
		logger.TestLog(true, "=========================================================")
		logger.TestLog(true, "===> PRICING")
		logger.TestLog(true, "=========================================================")
		logger.TestLog(true, "blockNumber:		  ", statement.BlockNumber)
		logger.TestLog(true, "uniswapFactoryV2:   ", uniswapFactoryV2.Hex())
		logger.TestLog(true, "theCall:            ", theCall1)
		logger.TestLog(true, "pairAddress:        ", pairAddress.Hex())
		logger.TestLog(true, "theCall:            ", theCall2)
		logger.TestLog(true, "first:              ", first.Hex())
		logger.TestLog(true, "second:             ", second.Hex())
		logger.TestLog(true, "reversed:           ", reversed)
		logger.TestLog(true, "reserve0:           ", reserve0)
		logger.TestLog(true, "reserve1:           ", reserve1)
		logger.TestLog(true, "price:              ", bigPrice)
	}

	price, _ = bigPrice.Float64()
	price *= multiplier
	source = "uniswap"
	return price, source, nil
}
