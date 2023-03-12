// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package identifiers

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/bykof/gostradamus"
)

func (id *Identifier) ResolveBlocks(chain string) ([]uint64, error) {
	current, end, err := id.getBounds(chain)
	if err != nil {
		return []uint64{}, err
	}
	blocks := []uint64{}
	for current < end {
		blocks = append(blocks, current)
		current, err = id.nextBlock(chain, current)
		if err != nil {
			return []uint64{}, err
		}
	}
	return blocks, nil
}

func (id *Identifier) getBounds(chain string) (uint64, uint64, error) {
	start := id.Start.resolvePoint(chain)
	switch id.ModifierType {
	case Period:
		start, _ = snapBnToPeriod(start, chain, id.Modifier.Period)
	default:
		// do nothing
	}
	end := id.End.resolvePoint(chain)
	if end == utils.NOPOS || end == 0 {
		end = start + 1
	}

	return start, end, nil
}

func snapBnToPeriod(bn uint64, chain, period string) (uint64, error) {
	dt, err := tslib.FromBnToDate(chain, bn)
	if err != nil {
		return bn, err
	}
	switch period {
	case "hourly":
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorHour()
	case "daily":
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorDay()
	case "weekly":
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorWeek() // returns Monday -- we want Sunday
		dt = dt.ShiftDays(-1).FloorDay()
	case "monthly":
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorMonth()
	case "annually":
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorYear()
	}

	firstDate := gostradamus.FromUnixTimestamp(rpc.GetBlockTimestamp(chain, 0))
	if dt.Time().Before(firstDate.Time()) {
		dt = firstDate
	}

	ts := dt.UnixTimestamp()
	return tslib.FromTsToBn(chain, ts)
}

func (id *Identifier) nextBlock(chain string, current uint64) (uint64, error) {
	bn := current

	if id.ModifierType == Step {
		bn = bn + uint64(id.Modifier.Step)

	} else if id.ModifierType == Period {
		dt, err := tslib.FromBnToDate(chain, bn)
		if err != nil {
			return bn, err
		} else {
			switch id.Modifier.Period {
			case "hourly":
				dt = dt.ShiftMinutes(5)
				dt = dt.ShiftHours(1)
				dt = dt.FloorHour()
			case "daily":
				dt = dt.ShiftMinutes(5)
				dt = dt.ShiftDays(1)
				dt = dt.FloorDay()
			case "weekly":
				dt = dt.ShiftDays(1)
				dt = dt.ShiftMinutes(5)
				dt = dt.ShiftWeeks(1)
				dt = dt.FloorWeek() // returns Monday -- we want Sunday
				dt = dt.ShiftDays(-1).FloorDay()
			case "monthly":
				dt = dt.ShiftMinutes(5)
				dt = dt.ShiftMonths(1)
				dt = dt.FloorMonth()
			case "annually":
				dt = dt.ShiftMinutes(5)
				dt = dt.ShiftYears(1)
				dt = dt.FloorYear()
			default:
				// should not happen
			}

			ts := dt.UnixTimestamp()
			bn, err = tslib.FromTsToBn(chain, ts)
			if err != nil {
				return bn, err
			}
		}

	} else {
		bn = bn + 1
	}

	return bn, nil
}

func (p *Point) resolvePoint(chain string) uint64 {
	var bn uint64
	if p.Hash != "" {
		provider := config.GetRpcProvider(chain)
		bn, _ = rpcClient.BlockNumberFromHash(provider, p.Hash)
	} else if p.Date != "" {
		bn, _ = tslib.FromDateToBn(chain, p.Date)
	} else if p.Special != "" {
		bn, _ = tslib.FromNameToBn(chain, p.Special)
	} else if p.Number >= utils.EarliestEvmTs {
		var err error
		bn, err = tslib.FromTsToBn(chain, int64(p.Number))
		if err == tslib.ErrInTheFuture {
			provider := config.GetRpcProvider(chain)
			latest := rpcClient.BlockNumber(provider)
			tsFuture := rpc.GetBlockTimestamp(chain, latest)
			secs := uint64(tsFuture - int64(p.Number))
			blks := (secs / 13)
			bn = latest + blks
		}
	} else {
		bn = uint64(p.Number)
	}
	return bn
}

func (id *Identifier) ResolveTxs(chain string) ([]types.RawAppearance, error) {
	txs := []types.RawAppearance{}

	if id.StartType == BlockNumber {
		if id.Modifier.Period == "all" {
			provider := config.GetRpcProvider(chain)
			cnt, err := rpcClient.TxCountByBlockNumber(provider, uint64(id.Start.Number))
			if err != nil {
				return txs, err
			}
			for i := uint32(0); i < uint32(cnt); i++ {
				app := types.RawAppearance{BlockNumber: uint32(id.Start.Number), TransactionIndex: i}
				txs = append(txs, app)
			}
			return txs, nil
		}

		if id.EndType == TransactionIndex {
			app := types.RawAppearance{BlockNumber: uint32(id.Start.Number), TransactionIndex: uint32(id.End.Number)}
			return append(txs, app), nil
		}

		msg := fmt.Sprintf("unknown transaction type: %s", id)
		return txs, errors.New(msg)
	}

	if id.StartType == BlockHash && id.EndType == TransactionIndex {
		if id.Modifier.Period == "all" {
			provider := config.GetRpcProvider(chain)
			cnt, err := rpcClient.TxCountByBlockNumber(provider, uint64(id.Start.resolvePoint(chain)))
			if err != nil {
				return txs, err
			}
			for i := uint32(0); i < uint32(cnt); i++ {
				app := types.RawAppearance{BlockNumber: uint32(id.Start.resolvePoint(chain)), TransactionIndex: i}
				txs = append(txs, app)
			}
			return txs, nil
		}

		app := types.RawAppearance{BlockNumber: uint32(id.Start.resolvePoint(chain)), TransactionIndex: uint32(id.End.Number)}
		return append(txs, app), nil
	}

	if id.StartType == TransactionHash {
		provider := config.GetRpcProvider(chain)
		bn, txid, err := rpcClient.TxNumberAndIdFromHash(provider, id.Start.Hash)
		app := types.RawAppearance{BlockNumber: uint32(bn), TransactionIndex: uint32(txid)}
		return append(txs, app), err
	}

	app := types.RawAppearance{BlockNumber: uint32(0), TransactionIndex: uint32(0)}
	msg := fmt.Sprintf("unknown transaction type %s", id)
	return append(txs, app), errors.New(msg)
}

/*
// TODO: next and previous skip markers - search for this to find other things related
bool wrangleTxId(string_q& argOut, string_q& errorMsg) {
    if (contains(argOut, "0x"))
        return true;

    // valid args are 'latest', 'bn.txid', 'bn.txid.next', or 'bn.txid.prev'
    replaceReverse(argOut, ":", ".");

    CStringArray parts;
    explode(parts, argOut, '.');

    if ((parts.size() == 2) && endsWith(argOut, ".*")) {
        CBlock block;
        getBlockLight(block, str_2_Uint(parts[0]));
        argOut = parts[0] + ".0";
        if (block.transactions.size() > 0)
            argOut += ("-to-" + uint_2_Str(block.transactions.size()));
        return true;
    }

    if (parts.size() == 0) {
        errorMsg = argOut + " does not appear to be a valid transaction index (no parts).";
        return false;

    } else if (parts.size() == 1 && (parts[0] != "latest" && parts[0] != "first")) {
        errorMsg = argOut + " does not appear to be a valid transaction index (one part, not first or latest).";
        return false;

    } else if (parts.size() > 3) {
        errorMsg = argOut + " does not appear to be a valid transaction index (too many).";
        return false;
    } else {
        // two or three parts...
        if (!isNumeral(parts[0]) || !isNumeral(parts[1])) {
            errorMsg = argOut + " does not appear to be a valid transaction index.";
            return false;
        }
    }

    if (parts.size() == 2)
        return true;

    // it's directional
    if (parts[0] == "latest") {
        CBlock block;
        getBlockLight(block, "latest");
        if (block.transactions.size() > 0) {
            ostringstream os;
            os << block.blockNumber << "." << (block.transactions.size() - 1);
            argOut = os.str();
            return true;
        }
        parts[0] = uint_2_Str(block.blockNumber);
        parts[1] = "0";
        parts[2] = "prev";
    }

    if (parts[0] == "first") {
        argOut = "46147.0";
        return true;
    }

    ASSERT(parts[2] == "prev" || parts[2] == "next");
    return getDirectionalTxId(str_2_Uint(parts[0]), str_2_Uint(parts[1]), parts[2], argOut, errorMsg);
}

//--------------------------------------------------------------------------------
bool getDirectionalTxId(blknum_t bn, txnum_t txid, const string_q& dir, string_q& argOut, string_q& errorMsg) {
    blknum_t lastBlock = getLatestBlock_client();

    if (bn < firstTransactionBlock()) {
        argOut = uint_2_Str(firstTransactionBlock()) + ".0";
        return true;
    }

    CBlock block;
    getBlock(block, bn);

    argOut = "";
    txnum_t nextid = txid + 1;
    while (argOut.empty() && bn >= firstTransactionBlock() && bn <= lastBlock) {
        if (dir == "next") {
            if (nextid < block.transactions.size()) {
                argOut = uint_2_Str(block.blockNumber) + "." + uint_2_Str(nextid);
                return true;
            }
            block = CBlock();
            getBlock(block, ++bn);
            nextid = 0;
        } else if (dir == "prev") {
            if (txid > 0 && block.transactions.size() > 0) {
                argOut = uint_2_Str(block.blockNumber) + "." + uint_2_Str(txid - 1);
                return true;
            }
            if (bn == 0)
                return true;
            block = CBlock();
            getBlock(block, --bn);
            txid = block.transactions.size();
        }
    }
    errorMsg = "Could not find " + dir + " transaction to " + uint_2_Str(bn) + "." + uint_2_Str(txid);
    return false;
}
*/

func GetBlockNumbers(chain string, ids []Identifier) ([]uint64, error) {
	var nums []uint64
	for _, br := range ids {
		blockNums, err := br.ResolveBlocks(chain)
		if err != nil {
			return []uint64{}, err
		}
		nums = append(nums, blockNums...)
	}
	return nums, nil
}

func GetTransactionIds(chain string, ids []Identifier) ([]types.RawAppearance, error) {
	var txids []types.RawAppearance
	for _, br := range ids {
		blockNums, err := br.ResolveTxs(chain)
		if err != nil {
			return []types.RawAppearance{}, err
		}
		txids = append(txids, blockNums...)
	}
	return txids, nil
}
