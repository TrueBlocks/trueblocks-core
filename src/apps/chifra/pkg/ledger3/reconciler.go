package ledger3

import (
	"encoding/csv"
	"encoding/json"
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger1"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type Reconciler3 struct {
	opts              *ledger1.ReconcilerOptions
	names             map[base.Address]types.Name
	hasStartBlock     bool
	transfers         map[blockTxKey][]types.AssetTransfer
	accountLedger     map[assetHolderKey]base.Wei
	ledgerAssets      map[base.Address]bool
	correctionCounter base.Value
	entryCounter      base.Value
	Connection        *rpc.Connection
}

func (r *Reconciler3) String() string {
	bytes, _ := json.MarshalIndent(r, "", "  ")
	return string(bytes)
}

func NewReconciler(conn *rpc.Connection, opts *ledger1.ReconcilerOptions) *Reconciler3 {
	parts := types.Custom | types.Prefund | types.Regular
	names, _ := names.LoadNamesMap(conn.Chain, parts, []string{})
	return &Reconciler3{
		Connection:    conn,
		opts:          opts,
		names:         names,
		hasStartBlock: false,
		transfers:     make(map[blockTxKey][]types.AssetTransfer),
		accountLedger: make(map[assetHolderKey]base.Wei),
		ledgerAssets:  make(map[base.Address]bool),
	}
}

func (r *Reconciler3) getTransferChannel(app *types.Appearance) <-chan types.AssetTransfer {
	ch := make(chan types.AssetTransfer)
	go func() {
		defer close(ch)
		key := blockTxKey{BlockNumber: base.Blknum(app.BlockNumber), TransactionIndex: base.Txnum(app.TransactionIndex)}
		for _, p := range r.transfers[key] {
			if p.Holder == r.opts.AccountFor {
				ch <- p
			}
		}
	}()
	return ch
}

func (r *Reconciler3) correctingEntry(reason string, onChain, curBal base.Wei, p *types.AssetTransfer) types.AssetTransfer {
	correction := *p
	correctionDiff := *new(base.Wei).Sub(&onChain, &curBal)
	correction.BegBal = curBal
	correction.EndBal = onChain
	correction.CorrectingReason = reason
	correction.AmountIn = *base.ZeroWei
	correction.AmountOut = *base.ZeroWei
	if correctionDiff.Cmp(base.ZeroWei) > 0 {
		correction.AmountIn = correctionDiff
	} else if correctionDiff.Cmp(base.ZeroWei) < 0 {
		correction.AmountOut = *correctionDiff.Neg()
	}
	return correction
}

func (r *Reconciler3) flushBlock(postings []types.AssetTransfer, modelChan chan<- types.Modeler) {
	blockProcessedAssets := make(map[base.Address]bool)
	assetLastSeen := make(map[base.Address]int)
	for i, p := range postings {
		key := assetHolderKey{Asset: p.Asset, Holder: p.Holder}
		if !blockProcessedAssets[p.Asset] {
			if r.hasStartBlock && !r.ledgerAssets[p.Asset] {
				if onChain, err := r.Connection.GetBalanceAtToken(p.Asset, p.Holder, p.BlockNumber-1); err == nil {
					if p.BlockNumber == 0 {
						onChain = base.ZeroWei
					}
					r.accountLedger[key] = *onChain
				}
				r.ledgerAssets[p.Asset] = true
			}
			if onChain, err := r.Connection.GetBalanceAtToken(p.Asset, p.Holder, p.BlockNumber-1); err == nil {
				if p.BlockNumber == 0 {
					onChain = base.ZeroWei
				}
				curBal := r.accountLedger[key]
				if !onChain.Equal(&curBal) {
					correctingEntry := r.correctingEntry("mis", *onChain, curBal, &p)
					r.correctionCounter++
					correctingEntry.CorrectionId = r.correctionCounter
					r.entryCounter++
					correctingEntry.StatementId = r.entryCounter
					modelChan <- &correctingEntry
				}
			}
			blockProcessedAssets[p.Asset] = true
		}

		p.BegBal = r.accountLedger[key]
		r.accountLedger[key] = *p.EndBalCalc()
		r.entryCounter++
		p.StatementId = r.entryCounter
		postings[i] = p
		assetLastSeen[p.Asset] = i
		modelChan <- &p
	}

	var corrections []types.AssetTransfer
	for _, idx := range assetLastSeen {
		p := postings[idx]
		key := assetHolderKey{Asset: p.Asset, Holder: p.Holder}
		curBal := r.accountLedger[key]
		if !p.EndBal.Equal(&curBal) {
			correctingEntry := r.correctingEntry("imb", p.EndBal, curBal, &p)
			corrections = append(corrections, correctingEntry)
		}
	}

	sort.SliceStable(corrections, func(i, j int) bool {
		if corrections[i].TransactionIndex != corrections[j].TransactionIndex {
			return corrections[i].TransactionIndex < corrections[j].TransactionIndex
		}
		if corrections[i].LogIndex != corrections[j].LogIndex {
			return corrections[i].LogIndex < corrections[j].LogIndex
		}
		return corrections[i].Asset.Hex() < corrections[j].Asset.Hex()
	})

	for _, correction := range corrections {
		r.correctionCounter++
		correction.CorrectionId = r.correctionCounter
		r.entryCounter++
		correction.StatementId = r.entryCounter
		modelChan <- &correction
		key := assetHolderKey{Asset: correction.Asset, Holder: correction.Holder}
		r.accountLedger[key] = correction.EndBal
	}
}

func (r *Reconciler3) ProcessStream(apps []types.Appearance, modelChan chan<- types.Modeler) {
	postingStream := make(chan types.AssetTransfer, 100)
	go func() {
		defer close(postingStream)
		var prevBlock base.Blknum
		for _, app := range apps {
			bn := base.Blknum(app.BlockNumber)
			if bn != prevBlock && prevBlock != 0 {
				postingStream <- types.NewAssetTransfer(types.Transfer{
					BlockNumber: prevBlock,
					Asset:       base.EndOfBlockSentinel,
				})
			}
			for p := range r.getTransferChannel(&app) {
				postingStream <- p
			}
			prevBlock = bn
		}
		if prevBlock != 0 {
			postingStream <- types.NewAssetTransfer(types.Transfer{
				BlockNumber: prevBlock,
				Asset:       base.EndOfBlockSentinel,
			})
		}
		postingStream <- types.NewAssetTransfer(types.Transfer{
			Asset: base.EndOfStreamSentinel,
		})
	}()

	var postings []types.AssetTransfer
	for posting := range postingStream {
		switch posting.Asset {
		case base.EndOfBlockSentinel:
			r.flushBlock(postings, modelChan)
			postings = nil
		case base.EndOfStreamSentinel:
			if len(postings) > 0 {
				r.flushBlock(postings, modelChan)
			}
			return
		default:
			postings = append(postings, posting)
		}
	}
}

func (r *Reconciler3) InitData() {
	folder := os.Getenv("FOLDER")
	if folder == "" {
		folder = "tests"
	}

	transfersFn := filepath.Join(folder, "transfers.csv")
	transfersFile, _ := os.Open(transfersFn)
	defer transfersFile.Close()
	transfersReader := csv.NewReader(transfersFile)
	transfersReader.Comment = '#'
	if transfersRecords, err := transfersReader.ReadAll(); err != nil {
		fmt.Println("Problem with data file:", transfersFn)
		logger.Fatal(err)
	} else if len(transfersRecords) == 0 {
		logger.Fatal("no transfers")
	} else {
		for _, record := range transfersRecords[1:] {
			if strings.HasPrefix(record[0], "#") {
				continue
			}
			amt := base.NewWeiStr(record[5])
			p := types.NewAssetTransfer(types.Transfer{
				BlockNumber:      base.Blknum(base.MustParseUint64(record[0])),
				TransactionIndex: base.Txnum(base.MustParseUint64(record[1])),
				LogIndex:         base.Lognum(base.MustParseUint64(record[2])),
				Asset:            base.HexToAddress(record[3]),
				Holder:           base.HexToAddress(record[4]),
			})
			if amt.Cmp(base.ZeroWei) > 0 {
				p.AmountIn = *amt
			} else if amt.Cmp(base.ZeroWei) < 0 {
				p.AmountOut = *amt.Neg()
			}
			eb, _ := r.Connection.GetBalanceAtToken(p.Asset, p.Holder, p.BlockNumber)
			p.EndBal = *eb

			key := blockTxKey{BlockNumber: p.BlockNumber, TransactionIndex: p.TransactionIndex}
			r.transfers[key] = append(r.transfers[key], p)
		}
		if firstBlock := os.Getenv("FIRST_BLOCK"); firstBlock != "" {
			r.hasStartBlock = true
		}
	}
}

type blockTxKey struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
}

type assetHolderKey struct {
	Asset  base.Address
	Holder base.Address
}
