package transactionsPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
)

func (opts *TransactionsOptions) HandleUniq() (err error, disp bool) {
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		for _, rng := range opts.TransactionIds {
			txIds, err := rng.ResolveTxs(opts.Globals.Chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, app := range txIds {
				ts, err := tslib.FromBnToTs(opts.Globals.Chain, uint64(app.BlockNumber))
				if err != nil {
					errorChan <- err
					continue
				}
				modelChan <- &types.SimpleAppearance{
					// Address:          mon.Address,
					BlockNumber:      app.BlockNumber,
					TransactionIndex: app.TransactionIndex,
					Timestamp:        ts,
					Date:             utils.FormattedDate(ts),
				}
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts()), true
}

/*
bool assignReason(const CName& accountedFor, CAppearance& app, const CTransaction& trans) {
    if (app.transactionIndex > 99996) {  // leave this here for searching: 99999
        app.reason = "miner";
        return true;
    }
    if (app.address == trans.from) {
        app.reason = "from";
        return true;
    }
    if (app.address == trans.to) {
        app.reason = "to";
        return true;
    }
    if (app.address == trans.receipt.contractAddress) {
        app.reason = "creation";
        return true;
    }
    bool junk = false;
    string_q ss = substitute(app.address, "0x", "");
    if (contains(trans.input, ss)) {
        app.reason = "input";
        if (accountedFor.address == app.address) {
            return true;
        }
        junk = true;
    }

    for (size_t i = 0; i < trans.receipt.logs.size(); i++) {
        const CLog* l = &trans.receipt.logs[i];
        if (l->address == app.address) {
            app.reason = "log_" + uint_2_Str(i) + "_generator";
            return true;
        }
        if (contains(l->topics[0], ss)) {
            app.reason = "log_" + uint_2_Str(i) + "_topic_" + uint_2_Str(0);
            return true;
        }
        for (size_t j = 1; j < l->topics.size(); j++) {
            if (contains(l->topics[j], ss)) {
                app.reason = "log_" + uint_2_Str(i) + "_topic_" + uint_2_Str(j);
                if (accountedFor.address == app.address) {
                    return true;
                }
                junk = true;
            }
        }
        if (contains(l->data, ss)) {
            app.reason = "log_" + uint_2_Str(i) + "_data";
            if (accountedFor.address == app.address) {
                return true;
            }
            junk = true;
        }
    }

    if (!junk) {
        app.reason = "trace";
    }
    return true;  // !junk;
}
*/
