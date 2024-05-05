// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package identifiers

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/bykof/gostradamus"
)

// ResolveBlocks resolves a list of identifiers to a list of blocks (excluding the last block)
func (id *Identifier) ResolveBlocks(chain string) ([]base.Blknum, error) {
	bound, err := id.getBounds(chain)
	if err != nil {
		return []base.Blknum{}, err
	}

	blocks := []base.Blknum{}
	current, end := bound.First, bound.Last
	for current < end {
		blocks = append(blocks, current)
		current, err = id.nextBlock(chain, current)
		if err != nil {
			return []base.Blknum{}, err
		}
	}
	return blocks, nil
}

// GetBounds returns the earliest and latest blocks for an array of identifiers
func GetBounds(chain string, ids *[]Identifier) (ret base.BlockRange, err error) {
	ret = base.BlockRange{
		First: base.NOPOSN2,
		Last:  0,
	}

	for _, id := range *ids {
		idRange, err := id.getBounds(chain)
		if err != nil {
			return ret, err
		}
		ret.First = base.Min2(ret.First, idRange.First)
		ret.Last = base.Max2(ret.Last, idRange.Last)
	}

	return ret, nil
}

// getBounds returns the earliest and latest blocks for the identifier
func (id *Identifier) getBounds(chain string) (ret base.BlockRange, err error) {
	ret.First = id.Start.resolvePoint(chain)
	switch id.ModifierType {
	case Period:
		ret.First, _ = snapBnToPeriod(ret.First, chain, id.Modifier.Period)
	default:
		// do nothing
	}
	ret.Last = id.End.resolvePoint(chain)
	if ret.Last == base.NOPOSN2 || ret.Last == 0 {
		ret.Last = ret.First + 1
	}

	return ret, nil
}

func snapBnToPeriod(bn base.Blknum, chain, period string) (base.Blknum, error) {
	conn := rpc.TempConnection(chain)

	dt, err := tslib.FromBnToDate(chain, bn)
	if err != nil {
		return bn, err
	}

	// within five minutes of the period, snap to the future, otherwise snap to the past
	switch period {
	case "hourly":
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorHour()
	case "daily":
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorDay()
	case "weekly":
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorWeek()
		dt = dt.ShiftDays(-1).FloorDay() // returns Monday -- we want Sunday
	case "monthly":
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorMonth()
	case "quarterly": // we assume here that the data is already on the quarter
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorMonth()
		for {
			if dt.Month() == 1 || dt.Month() == 4 || dt.Month() == 7 || dt.Month() == 10 {
				break
			}
			dt = dt.ShiftMonths(1)
			dt = dt.FloorMonth()
		}
	case "annually":
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorYear()
	}

	firstDate := gostradamus.FromUnixTimestamp(conn.GetBlockTimestamp(0))
	if dt.Time().Before(firstDate.Time()) {
		dt = firstDate
	}

	ts := dt.UnixTimestamp()
	return tslib.FromTsToBn(chain, ts)
}

func (id *Identifier) nextBlock(chain string, current base.Blknum) (base.Blknum, error) {
	bn := current

	if id.ModifierType == Step {
		bn = bn + base.Blknum(id.Modifier.Step)

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
			case "quarterly":
				dt = dt.ShiftMinutes(5)
				dt = dt.ShiftMonths(3)
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
			if bn == current {
				// might happen if the block spans the period
				bn++ // ensure at least one block's advancement
			}
		}

	} else {
		bn = bn + 1
	}

	return bn, nil
}

func (p *Point) resolvePoint(chain string) base.Blknum {
	conn := rpc.TempConnection(chain)

	var bn base.Blknum
	if p.Hash != "" {
		bn, _ = conn.GetBlockNumberByHash(p.Hash)
	} else if p.Date != "" {
		bn, _ = tslib.FromDateToBn(chain, p.Date)
	} else if p.Special != "" {
		bn, _ = tslib.FromNameToBn(chain, p.Special)
	} else if p.Number >= utils.EarliestEvmTs {
		var err error
		bn, err = tslib.FromTsToBn(chain, base.Timestamp(p.Number))
		if err == tslib.ErrInTheFuture {
			latest := conn.GetLatestBlockNumber()
			tsFuture := conn.GetBlockTimestamp(latest)
			secs := base.Blknum(tsFuture - base.Timestamp(p.Number))
			blks := (secs / 13)
			bn = latest + blks
		}
	} else {
		bn = base.Blknum(p.Number)
	}
	return bn
}

func (id *Identifier) ResolveTxs(chain string) ([]types.RawAppearance, error) {
	conn := rpc.TempConnection(chain)
	txs := []types.RawAppearance{}

	if id.StartType == BlockNumber {
		if id.Modifier.Period == "all" {
			cnt, err := conn.GetTransactionCountInBlock(base.Blknum(id.Start.Number))
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
			cnt, err := conn.GetTransactionCountInBlock(base.Blknum(id.Start.resolvePoint(chain)))
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
		app, err := conn.GetTransactionAppByHash(id.Start.Hash)
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
    replaceReverse(argOut, ":",  ".");

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
bool getDirectionalTxId(blknum_t bn, tx num_t txid, const string_q& dir, string_q& argOut, string_q& errorMsg) {
    blknum_t lastBlock = getLatestBlock_client();

    if (bn < firstTransactionBlock()) {
        argOut = uint_2_Str(firstTransactionBlock()) + ".0";
        return true;
    }

    CBlock block;
    getBlock(block, bn);

    argOut = "";
    tx num_t nextid = txid + 1;
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

func GetBlockNumberMap(chain string, ids []Identifier) (map[base.Blknum]bool, error) {
	numMap := make(map[base.Blknum]bool, 1000)
	for _, br := range ids {
		blockNums, err := br.ResolveBlocks(chain)
		if err != nil {
			return numMap, err
		}
		for _, bn := range blockNums {
			numMap[bn] = true
		}
	}
	return numMap, nil
}

func GetBlockNumbers(chain string, ids []Identifier) ([]base.Blknum, error) {
	var nums []base.Blknum
	for _, br := range ids {
		blockNums, err := br.ResolveBlocks(chain)
		if err != nil {
			return []base.Blknum{}, err
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
