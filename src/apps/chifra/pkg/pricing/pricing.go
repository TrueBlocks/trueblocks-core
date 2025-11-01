package pricing

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

// TODO: Much of this reporting could be removed as it's only used for debugging

// PriceUsd returns the price of the asset in USD
func PriceUsd(conn *rpc.Connection, statement *types.Statement) (base.Float, string, error) {
	if statement.IsStableCoin() {
		r := priceDebugger{
			address: statement.Asset,
			symbol:  statement.Symbol,
		}
		r.report("stable-coin")
		return *base.OneFloat, "stable-coin", nil
	}

	if statement.BlockNumber <= uniswapFactoryV2_deployed {
		if statement.IsEth() {
			return priceUsdMaker(conn, statement)
		} else {
			msg := fmt.Sprintf("Block %d is prior to deployment (%d) of Uniswap V2. No other source for tokens prior to UniSwap", statement.BlockNumber, uniswapFactoryV2_deployed)
			logger.TestLog(true, msg)
			return *base.ZeroFloat, "token-not-priced-pre-uni", nil
		}
	}

	return priceUsdUniswap(conn, statement)
}
