/*

func (r *Reconciler) trialBalanceOld(reason string, stmt *types.Statement, node *types.AppNode[types.Transaction], correct bool) (bool, error) {
	trans := node.Data()

	logger.TestLog(true, "------------------------------------")
	logger.TestLog(true, "# Reason:", reason)
	logger.TestLog(true, "Trial balance for ", stmt.Asset, stmt.Holder, "at stmt", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex)
	logger.TestLog(true, "------------------------------------")

	var err error
	if stmt.BegBal, stmt.EndBal, err = r.Connection.GetReconBalances(&rpc.BalanceOptions{
		CurrBlk: trans.BlockNumber,
		Asset:   stmt.Asset,
		Holder:  stmt.AccountedFor,
	}); err != nil {
		logger.TestLog(true, "----------err GetReconBalances --------------------------")
		logger.TestLog(true)
		return false, err
	}

	isSender := stmt.AccountedFor == stmt.Sender
	isRecipient := stmt.AccountedFor == stmt.Recipient
	if !isSender && !isRecipient {
		logger.TestLog(true, "Not sender or recipient", stmt.AccountedFor, stmt.Sender, stmt.Recipient, reason)
	}
	logger.TestLog(true, "Sender:", isSender, "Recipient:", isRecipient, stmt.Sender, stmt.Recipient, stmt.AccountedFor)

	endCorrected := false
	running, found := r.Running[stmt.Asset]
	if found {
		logger.TestLog(true, "adjustForIntraTransfer1", "firstInBlock:", node.IsFirstInBlock(), "lastInBlock:", node.IsLastInBlock())

		logger.TestLog(true, "XXXFound ", running.String())
		// We've seen this asset before. Beginning balance is either...
		if running.Block() == trans.BlockNumber {
			// ...(a) the last running balance (if we're in the same block), or...
			logger.TestLog(true, "Same block ", stmt.Asset, "at block", running.Block(), "and", trans.BlockNumber, "of", running.Amount().Text(10))
			stmt.BegBal = *running.Amount()
		} else {
			// ...(b) the balance from the chain at the last appearance.
			logger.TestLog(true, "Querying at block", running.Block(), "for", stmt.Asset, stmt.Holder)
			if val, err := r.Connection.GetBalanceAtToken(stmt.Asset, stmt.Holder, running.Block()); err != nil {
				logger.TestLog(true, "----------err GetBalanceAtToken --------------------------")
				logger.TestLog(true, "")
				return false, err
			} else {
				if val == nil {
					logger.TestLog(true, "Different block (nil)", stmt.Asset, "at block", running.Block(), "and", trans.BlockNumber, "of", running.Amount().Text(10))
					stmt.BegBal = *running.Amount()
				} else {
					logger.TestLog(true, "Different block ", stmt.Asset, "at block", running.Block(), "and", trans.BlockNumber, "of", val.Text(10))
					stmt.BegBal = *val
				}
			}
		}
		// The previous balance is the running balance
		stmt.PrevBal = *running.Amount()

	} else {
		logger.TestLog(true, "adjustForIntraTransfer2", "firstInBlock:", node.IsFirstInBlock(), "lastInBlock:", node.IsLastInBlock())
		logger.TestLog(true, "XXXNot found ", stmt.Asset)
		// We've never seen this asset before. Beginning balance is already queried (at blockNumber-1) and
		// the previous balance is that beginning balance. Note that this will be zero if blockNumber is 0.
		logger.TestLog(true, "Using block-1 balance for ", stmt.Asset, "at block", running.Block(), "of", running.Amount().Text(10))
		stmt.PrevBal = stmt.BegBal
	}

	if !node.IsLastInBlock() {
		endCorrected = true
		stmt.EndBal = *stmt.EndBalCalc()
	}

	if endCorrected {
		logger.TestLog(true, "end balance already corrected", stmt.BegBal.Text(10), stmt.AmountNet().Text(10), stmt.EndBal.Text(10), "==?", stmt.EndBalCalc().Text(10))
	} else {
		if correct && !stmt.Reconciled() {
			if trans.Receipt != nil && stmt.IsNullTransfer(len(trans.Receipt.Logs), trans.To) {
				stmt.CorrectForNullTransfer()
			}
			if !stmt.Reconciled() {
				stmt.CorrectBeginBalance()
				stmt.CorrectEndBalance()
			}
		} else if !stmt.Reconciled() {
			logger.TestLog(true, "Not correcting unreconciled balances", reason)
		}
	}
	logger.TestLog(true, "Statement reconciles?", stmt.Reconciled(), reason)

	logger.TestLog(true, "EndBalCalc:", stmt.BegBal.Text(10), stmt.AmountNet().Text(10), stmt.EndBalCalc().Text(10))
	newEndBal := stmt.EndBalCalc() //new(base.Wei).Add(&stmt.BegBal, stmt.AmountNet())
	logger.TestLog(true, "Inserting ", stmt.Asset, "at block", trans.BlockNumber, "of", newEndBal.Text(10))
	r.Running[stmt.Asset] = Running{
		amt:  *newEndBal,
		stmt: stmt,
	}
	if found, k := r.Running[stmt.Asset]; k {
		logger.TestLog(true, "Found ", stmt.Asset, "at block", found.Block(), "of", found.Amount().Text(10))
	} else {
		logger.TestLog(true, "Not found ", stmt.Asset)
	}

	if !stmt.IsMaterial() {
		// logger.TestLog(true, "----------Not Material --------------------------")
		// logger.TestLog(true, "")
		return stmt.Reconciled(), nil
	}

	stmt.SpotPrice, stmt.PriceSource, _ = pricing.PriceUsd(r.Connection, stmt)
	if r.ShowDebugging {
		stmt.DebugStatement(node)
	}

	logger.TestLog(true, "----------Done ", stmt.Reconciled(), " --------------------------")
	logger.TestLog(true, "")
	return stmt.Reconciled(), nil
}
*/

package ledger

import (
	"encoding/json"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type AssetHolder struct {
	Asset  base.Address
	Holder base.Address
}

func NewAssetHolderKey(asset, holder base.Address) AssetHolder {
	return AssetHolder{Asset: asset, Holder: holder}
}

type ReconcilerOptions struct {
	AccountFor   base.Address            `json:"accountFor"`
	FirstBlock   base.Blknum             `json:"firstBlock"`
	LastBlock    base.Blknum             `json:"lastBlock"`
	AsEther      bool                    `json:"asEther"`
	UseTraces    bool                    `json:"useTraces"`
	Reversed     bool                    `json:"reversed"`
	AssetFilters []base.Address          `json:"assetFilters"`
	AppFilters   *types.AppearanceFilter `json:"appFilters"`
}

type Running struct {
	amt  base.Wei
	stmt *types.Statement
}

func (r *Running) String() string {
	return fmt.Sprintf("running blkid: %d amount: %s", r.Block(), r.Amount().Text(10))
}

func (r *Running) Block() base.Blknum {
	if r.stmt == nil {
		return 0
	}
	return r.stmt.BlockNumber
}

func (r *Running) Amount() *base.Wei {
	return &r.amt
}

type Reconciler struct {
	Connection     *rpc.Connection             `json:"-"`
	Opts           *ReconcilerOptions          `json:"opts"`
	Names          map[base.Address]types.Name `json:"-"`
	ShowDebugging  bool                        `json:"showDebugging"`
	RemoveAirdrops bool                        `json:"removeAirdrops"`
	Running        map[AssetHolder]Running     `json:"running"`
}

func (r *Reconciler) String() string {
	bytes, _ := json.MarshalIndent(r, "", "  ")
	return string(bytes)
}

func NewReconciler(conn *rpc.Connection, opts *ReconcilerOptions) *Reconciler {
	parts := types.Custom | types.Prefund | types.Regular
	names, _ := names.LoadNamesMap("mainnet", parts, []string{})
	r := &Reconciler{
		Opts:           opts,
		Connection:     conn,
		Names:          names,
		ShowDebugging:  true,
		RemoveAirdrops: true,
		Running:        make(map[AssetHolder]Running),
	}
	return r
}
