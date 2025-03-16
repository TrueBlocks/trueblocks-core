package ledger1

import (
	"encoding/json"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

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
	Amount     base.Wei
	PreviBlock base.Blknum
	PreviTxId  base.Txnum
	PreviLogId base.Lognum
}

func (r *Running) String() string {
	return fmt.Sprintf("running blkid: %d txid: %d logid: %d amount: %s", r.PreviBlock, r.PreviTxId, r.PreviLogId, r.Amount.Text(10))
}

type Reconciler1 struct {
	Connection     *rpc.Connection             `json:"-"`
	Opts           *ReconcilerOptions          `json:"opts"`
	Names          map[base.Address]types.Name `json:"-"`
	ShowDebugging  bool                        `json:"showDebugging"`
	RemoveAirdrops bool                        `json:"removeAirdrops"`
	Running        map[base.Address]Running    `json:"running"`
}

func (r *Reconciler1) String() string {
	bytes, _ := json.MarshalIndent(r, "", "  ")
	return string(bytes)
}

func NewReconciler(conn *rpc.Connection, opts *ReconcilerOptions) *Reconciler1 {
	parts := types.Custom | types.Prefund | types.Regular
	names, _ := names.LoadNamesMap("mainnet", parts, []string{})
	r := &Reconciler1{
		Opts:           opts,
		Connection:     conn,
		Names:          names,
		ShowDebugging:  true,
		RemoveAirdrops: true,
		Running:        make(map[base.Address]Running),
	}
	return r
}

func (r *Reconciler1) SkipAirdrop(addr base.Address) bool {
	_ = addr
	return false
	// if name, found := r.Names[addr]; !found {
	// 	return false
	// } else {
	// 	return name.IsAirdrop()
	// }
}

func (r *Reconciler1) adjustForIntraTransfer(reason string, pos *types.AppPosition, stmt *types.Statement, trans *types.Transaction) error {
	isToken := reason == "token"

	isPrevSame := pos.Prev.BlockNumber == pos.Current.BlockNumber
	isNextSame := pos.Next.BlockNumber == pos.Current.BlockNumber
	if isToken {
		isNextSame = pos.Next.BlockNumber == pos.Current.BlockNumber && pos.Next.TransactionIndex == pos.Current.TransactionIndex
	}

	running, found := r.Running[stmt.Asset]
	if found {
		isPrevSame := running.PreviBlock == pos.Current.BlockNumber
		if isToken {
			isPrevSame = running.PreviBlock == stmt.BlockNumber && running.PreviTxId == stmt.TransactionIndex
		}
		logger.TestLog(true, "adjustForIntraTransfer1", "isToken:", isToken, "isPrevSame:", isPrevSame, "isNextSame:", isNextSame)

		logger.TestLog(true, "XXXFound ", running.String())
		// We've seen this asset before. Beginning balance is either...
		if running.PreviBlock == trans.BlockNumber {
			// ...(a) the last running balance (if we're in the same block), or...
			logger.TestLog(true, "Same block ", stmt.Asset, "at block", running.PreviBlock, "and", trans.BlockNumber, "of", running.Amount.Text(10))
			stmt.BegBal = running.Amount
		} else {
			// ...(b) the balance from the chain at the last appearance.
			logger.TestLog(true, "Querying at block", running.PreviBlock, "for", stmt.Asset, stmt.Holder)
			if val, err := r.Connection.GetBalanceAtToken(stmt.Asset, stmt.Holder, running.PreviBlock); err != nil {
				logger.TestLog(true, "----------err GetBalanceAtToken --------------------------")
				logger.TestLog(true, "")
				return err
			} else {
				if val == nil {
					logger.TestLog(true, "Different block (nil)", stmt.Asset, "at block", running.PreviBlock, "and", trans.BlockNumber, "of", running.Amount.Text(10))
					stmt.BegBal = running.Amount
				} else {
					logger.TestLog(true, "Different block ", stmt.Asset, "at block", running.PreviBlock, "and", trans.BlockNumber, "of", val.Text(10))
					stmt.BegBal = *val
				}
			}
		}
		// The previous balance is the running balance
		stmt.PrevBal = running.Amount
		pos.Prev.BlockNumber = running.PreviBlock
		pos.Prev.TransactionIndex = running.PreviTxId
	} else {
		logger.TestLog(true, "adjustForIntraTransfer2", "isToken:", isToken, "isPrevSame:", isPrevSame, "isNextSame:", isNextSame)
		logger.TestLog(true, "XXXNot found ", stmt.Asset)
		// We've never seen this asset before. Beginning balance is already queried (at blockNumber-1) and
		// the previous balance is that beginning balance. Note that this will be zero if blockNumber is 0.
		logger.TestLog(true, "Using block-1 balance for ", stmt.Asset, "at block", running.PreviBlock, "of", running.Amount.Text(10))
		stmt.PrevBal = stmt.BegBal
		pos.Prev.BlockNumber = base.Blknum(base.Max(int(trans.BlockNumber), 1) - 1)
		pos.Prev.TransactionIndex = base.Blknum(base.Max(int(trans.TransactionIndex), 1) - 1)
	}

	if isNextSame {
		stmt.EndBal = *stmt.EndBalCalc()
	}
	return nil
}

func (r *Reconciler1) trialBalance(reason string, trans *types.Transaction, stmt *types.Statement, pos *types.AppPosition, correct bool) (bool, error) {
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

	if err := r.adjustForIntraTransfer(reason, pos, stmt, trans); err != nil {
		return false, err
	}

	if correct && !stmt.Reconciled() {
		before := stmt.Report()
		if trans.Receipt != nil && stmt.IsNullTransfer(len(trans.Receipt.Logs), trans.To) {
			stmt.CorrectForNullTransfer()
		}
		// var after1, after2, after3 string
		var after3 string
		// after1 = stmt.Report()
		if !stmt.Reconciled() {
			stmt.CorrectBeginBalance()
			// after2 = stmt.Report()
			stmt.CorrectEndBalance()
			after3 = stmt.Report()
		}
		// logger.TestLog(before != after1, "\nbefore,after1", ShowDiff(before, after1))
		// logger.TestLog(before != after2, "\nbefore,after2", ShowDiff(before, after2))
		logger.TestLog(before != after3, "\nbefore,after3", ShowDiff(before, after3))
		// logger.TestLog(after1 != after2, "\nafter1,after2", ShowDiff(after1, after2))
		// logger.TestLog(after1 != after3, "\nafter1,after3", ShowDiff(after1, after3))
		// logger.TestLog(after2 != after3, "\nafter2,after3", ShowDiff(after2, after3))

	} else if !stmt.Reconciled() {
		logger.TestLog(true, "Not correcting unreconciled balances", reason)
	}
	logger.TestLog(true, "Statement reconciles?", stmt.Reconciled(), reason)

	logger.TestLog(true, "EndBalCalc:", stmt.BegBal.Text(10), stmt.AmountNet().Text(10), stmt.EndBalCalc().Text(10))
	newEndBal := stmt.EndBalCalc() //new(base.Wei).Add(&stmt.BegBal, stmt.AmountNet())
	logger.TestLog(true, "Inserting ", stmt.Asset, "at block", trans.BlockNumber, "of", newEndBal.Text(10))
	r.Running[stmt.Asset] = Running{
		Amount:     *newEndBal,
		PreviBlock: trans.BlockNumber,
	}
	if found, k := r.Running[stmt.Asset]; k {
		logger.TestLog(true, "Found ", stmt.Asset, "at block", found.PreviBlock, "of", found.Amount.Text(10))
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
		stmt.DebugStatement(reason, pos)
	}

	logger.TestLog(true, "----------Done ", stmt.Reconciled(), " --------------------------")
	logger.TestLog(true, "")
	return stmt.Reconciled(), nil
}

func (r *Reconciler1) GetStatements(pos *types.AppPosition, trans *types.Transaction) ([]types.Statement, error) {
	results := make([]types.Statement, 0, 20)
	if types.AssetOfInterest(r.Opts.AssetFilters, base.FAKE_ETH_ADDRESS) {
		if r.Connection.Store != nil {
			// walk.Cache_Statement
			s := &types.StatementGroup{
				Address:          base.FAKE_ETH_ADDRESS,
				BlockNumber:      trans.BlockNumber,
				TransactionIndex: trans.TransactionIndex,
			}
			if err := r.Connection.Store.Read(s); err == nil {
				// success
				return s.Statements, nil
			}
		}

		reconciled := false
		if !r.Opts.UseTraces {
			if stmt, err := trans.FetchStatement(r.Opts.AsEther, base.FAKE_ETH_ADDRESS, r.Opts.AccountFor); err != nil {
				return nil, err
			} else {
				var err error
				if reconciled, err = r.trialBalance("top-level", trans, stmt, pos, false); err != nil {
					return nil, err
				} else {
					if reconciled && stmt.IsMaterial() {
						_ = r.WriteToCache(base.FAKE_ETH_ADDRESS, stmt, trans.Timestamp)
						ReportProgress(stmt)
						results = append(results, *stmt)
					}
				}
			}
		}

		if r.Opts.UseTraces || !reconciled {
			if traces, err := r.Connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
				return nil, err
			} else {
				if stmt, err := trans.FetchStatementFromTraces(traces, r.Opts.AccountFor, r.Opts.AsEther); err != nil {
					// logger.Warn(colors.Yellow+"Statement at ", fmt.Sprintf("%d.%d", trans.BlockNumber, trans.TransactionIndex), " does not reconcile."+colors.Off)
					// TODO: Silent fail?
					logger.Error(err.Error())
				} else {
					if _, err = r.trialBalance("traces", trans, stmt, pos, true); err != nil {
						return nil, err
					} else {
						if stmt.IsMaterial() { // append even if not reconciled
							_ = r.WriteToCache(base.FAKE_ETH_ADDRESS, stmt, trans.Timestamp)
							ReportProgress(stmt)
							results = append(results, *stmt)
						}
					}
				}
			}
		}
	}

	if trans.Receipt != nil {
		if statements, err := trans.Receipt.FetchStatements(r.Opts.AccountFor, r.Opts.AssetFilters, r.Opts.AppFilters); err != nil {
			return nil, err
		} else {
			for _, s := range statements {
				if r.Connection.Store != nil {
					// walk.Cache_Statement
					sg := &types.StatementGroup{
						Address:          s.Asset,
						BlockNumber:      trans.BlockNumber,
						TransactionIndex: trans.TransactionIndex,
					}
					if err := r.Connection.Store.Read(sg); err == nil {
						// success
						results = append(results, sg.Statements...)
						continue
					}
				}

				stmt := &s
				stmt.Symbol = stmt.Asset.DefaultSymbol()
				stmt.Decimals = base.Value(18)
				if name, found := r.Names[stmt.Asset]; found {
					if r.RemoveAirdrops && r.SkipAirdrop(stmt.Asset) {
						logger.TestLog(true, "Removing airdrop for", stmt.Asset, "at block", stmt.BlockNumber, ".", stmt.TransactionIndex)
						continue
					}
					if name.Symbol != "" {
						stmt.Symbol = name.Symbol
					}
					if name.Decimals != 0 {
						stmt.Decimals = base.Value(name.Decimals)
					}
				}

				if _, err := r.trialBalance("token", trans, stmt, pos, true); err != nil {
					// TODO: Silent fail?
					logger.Error(err.Error())
					continue
				} else {
					// if reconciled {
					// 	id := fmt.Sprintf(" %d.%d.%d", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex)
					// 	logger.Progress(true, colors.Green+"Transaction", id, "reconciled       "+colors.Off)
					// }
					// order matters - don't move
					if stmt.IsMaterial() { // add even if not reconciled
						_ = r.WriteToCache(base.FAKE_ETH_ADDRESS, stmt, trans.Timestamp)
						ReportProgress(stmt)
						results = append(results, *stmt)
					}
				}
			}
		}
	}
	return results, nil
}

func ReportProgress(stmt *types.Statement) {
	msg := fmt.Sprintf("Ether statement at % 9d.%d.%d %s %s", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex, stmt.Asset.Hex(), stmt.Holder.Hex())
	if !stmt.IsEth() {
		msg = fmt.Sprintf("Token statement at % 9d.%d.%d %s %s", stmt.BlockNumber, stmt.TransactionIndex, stmt.LogIndex, stmt.Asset.Hex(), stmt.Holder.Hex())
	}
	spacer := strings.Repeat(" ", 100-base.Min(100, len(msg)))
	if !stmt.Reconciled() {
		// logger.Progress(true, colors.Green+msg+" reconciled.", colors.Off, spacer)
		// } else {
		logger.Warn(msg+" did not reconcile.", spacer)
	}
}

func (r *Reconciler1) WriteToCache(addr base.Address, stmt *types.Statement, ts base.Timestamp) error {
	// if !stmt.Reconciled() {
	_ = addr
	_ = stmt
	_ = ts
	return nil
	// }

	// sg := &types.StatementGroup{
	// 	Address:          base.FAKE_ETH_ADDRESS,
	// 	BlockNumber:      stmt.BlockNumber,
	// 	TransactionIndex: stmt.TransactionIndex,
	// 	Statements:       []types.Statement{*stmt},
	// }
	// return r.Connection.Store.WriteToCache(sg, walk.Cache_Statements, ts)
}

type DiffOp struct {
	Op   string
	Text string
}

func diffLines(a, b []string) []DiffOp {
	C := make([][]int, len(a)+1)
	for i := range C {
		C[i] = make([]int, len(b)+1)
	}

	for i := 1; i <= len(a); i++ {
		for j := 1; j <= len(b); j++ {
			if a[i-1] == b[j-1] {
				C[i][j] = C[i-1][j-1] + 1
			} else {
				if C[i-1][j] > C[i][j-1] {
					C[i][j] = C[i-1][j]
				} else {
					C[i][j] = C[i][j-1]
				}
			}
		}
	}

	var diff []DiffOp
	i, j := len(a), len(b)
	for i > 0 || j > 0 {
		if i > 0 && j > 0 && a[i-1] == b[j-1] {
			diff = append([]DiffOp{{"equal", a[i-1]}}, diff...)
			i--
			j--
		} else if i > 0 && (j == 0 || C[i-1][j] >= C[i][j-1]) {
			diff = append([]DiffOp{{"delete", a[i-1]}}, diff...)
			i--
		} else if j > 0 && (i == 0 || C[i-1][j] < C[i][j-1]) {
			diff = append([]DiffOp{{"insert", b[j-1]}}, diff...)
			j--
		}
	}
	return diff
}

func formatDiff(diff []DiffOp) string {
	var sb strings.Builder
	for _, op := range diff {
		switch op.Op {
		// case "equal":
		// 	sb.WriteString("  " + op.Text + "\n")
		case "delete":
			sb.WriteString("- " + op.Text + "\n")
		case "insert":
			sb.WriteString("+ " + op.Text + "\n")
		}
	}
	return sb.String()
}

func ShowDiff(str1, str2 string) string {
	lines1 := strings.Split(str1, ",")
	lines2 := strings.Split(str2, ",")
	diff := diffLines(lines2, lines1)
	return formatDiff(diff)
}
