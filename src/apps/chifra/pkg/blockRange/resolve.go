// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blockRange

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	tslibPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/bykof/gostradamus"
)

func (br *Identifier) ResolveBlocks(chain string) ([]uint64, error) {
	current, end, err := br.getBounds(chain)
	if err != nil {
		return []uint64{}, err
	}
	blocks := []uint64{}
	for current < end {
		blocks = append(blocks, current)
		current, err = br.nextBlock(chain, current)
		if err != nil {
			return []uint64{}, err
		}
	}
	return blocks, nil
}

func (br *Identifier) getBounds(chain string) (uint64, uint64, error) {
	start := br.Start.resolvePoint(chain)
	switch br.ModifierType {
	case Period:
		start, _ = snapBnToPeriod(start, chain, br.Modifier.Period)
	default:
		// do nothing
	}
	end := br.End.resolvePoint(chain)
	if end == utils.NOPOS || end == 0 {
		end = start + 1
	}

	// meta, _ := rpcClient.GetMetaData(chain, false)
	// if start > meta.Latest {
	// 	return start, end, fmt.Errorf("start block (%d) is in the future", start)
	// } else if end > meta.Latest {
	// 	return start, end, fmt.Errorf("end block (%d) is in the future", end)
	// }

	return start, end, nil
}

func snapBnToPeriod(bn uint64, chain, period string) (uint64, error) {
	dt, err := tslibPkg.FromBnToDate(chain, bn)
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
	firstDate := gostradamus.FromUnixTimestamp(utils.EarliestTs)
	if dt.Time().Before(firstDate.Time()) {
		dt = firstDate
	}
	ts := uint64(dt.UnixTimestamp())
	return tslibPkg.FromTsToBn(chain, ts)
}

func (br *Identifier) nextBlock(chain string, current uint64) (uint64, error) {
	bn := current

	if br.ModifierType == Step {
		bn = bn + uint64(br.Modifier.Step)

	} else if br.ModifierType == Period {
		dt, err := tslibPkg.FromBnToDate(chain, bn)
		if err != nil {
			return bn, err
		} else {
			switch br.Modifier.Period {
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

			ts := uint64(dt.UnixTimestamp())
			bn, err = tslibPkg.FromTsToBn(chain, ts)
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
		bn, _ = tslibPkg.FromDateToBn(chain, p.Date)
	} else if p.Special != "" {
		bn, _ = tslibPkg.FromNameToBn(chain, p.Special)
	} else if p.Number >= utils.EarliestTs {
		bn, _ = tslibPkg.FromTsToBn(chain, uint64(p.Number))
	} else {
		bn = uint64(p.Number)
	}
	return bn
}

func (br *Identifier) ResolveTxs(chain string) ([]types.SimpleAppearance, error) {
	txs := []types.SimpleAppearance{}
	if br.StartType == BlockNumber && br.EndType == TransactionIndex {
		app := types.SimpleAppearance{BlockNumber: uint32(br.Start.Number), TransactionIndex: uint32(br.End.Number)}
		return append(txs, app), nil
	}
	if br.StartType == BlockHash && br.EndType == TransactionIndex {
		app := types.SimpleAppearance{BlockNumber: 12, TransactionIndex: 12}
		return append(txs, app), nil
	}
	if br.StartType == TransactionHash {
		app := types.SimpleAppearance{BlockNumber: 22, TransactionIndex: 22}
		return append(txs, app), nil
	}
	app := types.SimpleAppearance{BlockNumber: uint32(0), TransactionIndex: uint32(0)}
	return append(txs, app), nil
}

/*
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
