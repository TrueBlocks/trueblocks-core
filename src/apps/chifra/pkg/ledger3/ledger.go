package ledger3

import (
	"encoding/csv"
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger4"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// ---------------------------------------------------------
type Reconciler3 struct {
	// conn *rpc.Connection
	conn              *Connection
	apps              []types.Appearance
	account           base.Address
	accountLedger     map[assetHolderKey]base.Wei
	transfers         map[blockTxKey][]ledger4.AssetTransfer
	correctionCounter base.Value
	entryCounter      base.Value
	hasStartBlock     bool
	ledgerAssets      map[base.Address]bool
}

// ---------------------------------------------------------
func NewReconciler3(chain string, addr base.Address) *Reconciler3 {
	r := &Reconciler3{
		account:       addr,
		accountLedger: make(map[assetHolderKey]base.Wei),
		transfers:     make(map[blockTxKey][]ledger4.AssetTransfer),
		// conn:          rpc.NewConnection(chain, false, map[walk.CacheType]bool{}),
		conn:          NewConnection(),
		hasStartBlock: false,
		ledgerAssets:  make(map[base.Address]bool),
	}

	r.initData()

	return r
}

// ---------------------------------------------------------
type Connection struct {
	balanceMap map[bnAssetHolderKey]base.Wei
}

// ---------------------------------------------------------
func NewConnection() *Connection {
	return &Connection{
		balanceMap: make(map[bnAssetHolderKey]base.Wei),
	}
}

// ---------------------------------------------------------
func (conn *Connection) GetBalanceAtToken(asset base.Address, holder base.Address, bn base.Blknum) (*base.Wei, error) {
	key := bnAssetHolderKey{BlockNumber: bn, AssetAddress: asset, Holder: holder}
	if bal, ok := conn.balanceMap[key]; ok {
		return &bal, nil
	}
	return base.ZeroWei, fmt.Errorf("balance not found")
}

// ---------------------------------------------------------
func (r *Reconciler3) getPostingChannel(app *types.Appearance) <-chan ledger4.AssetTransfer {
	ch := make(chan ledger4.AssetTransfer)
	go func() {
		defer close(ch)
		key := blockTxKey{BlockNumber: base.Blknum(app.BlockNumber), TransactionIndex: base.Txnum(app.TransactionIndex)}
		for _, p := range r.transfers[key] {
			if p.Holder == r.account {
				ch <- p
			}
		}
	}()
	return ch
}

// ---------------------------------------------------------
func (r *Reconciler3) correctingEntry(reason string, onChain, curBal base.Wei, p *ledger4.AssetTransfer) ledger4.AssetTransfer {
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

// ---------------------------------------------------------
func (r *Reconciler3) flushBlock(postings []ledger4.AssetTransfer, modelChan chan<- types.Modeler) {
	blockProcessedAssets := make(map[base.Address]bool)
	assetLastSeen := make(map[base.Address]int)
	for i, p := range postings {
		key := assetHolderKey{AssetAddress: p.AssetAddress, Holder: p.Holder}
		if !blockProcessedAssets[p.AssetAddress] {
			if r.hasStartBlock && !r.ledgerAssets[p.AssetAddress] {
				if onChain, err := r.conn.GetBalanceAtToken(p.AssetAddress, p.Holder, p.BlockNumber-1); err == nil {
					if p.BlockNumber == 0 {
						onChain = base.ZeroWei
					}
					r.accountLedger[key] = *onChain
				}
				r.ledgerAssets[p.AssetAddress] = true
			}
			if onChain, err := r.conn.GetBalanceAtToken(p.AssetAddress, p.Holder, p.BlockNumber-1); err == nil {
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
			blockProcessedAssets[p.AssetAddress] = true
		}

		p.BegBal = r.accountLedger[key]
		r.accountLedger[key] = *p.EndBalCalc()
		r.entryCounter++
		p.StatementId = r.entryCounter
		postings[i] = p
		assetLastSeen[p.AssetAddress] = i
		modelChan <- &p
	}

	var corrections []ledger4.AssetTransfer
	for _, idx := range assetLastSeen {
		p := postings[idx]
		key := assetHolderKey{AssetAddress: p.AssetAddress, Holder: p.Holder}
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
		return corrections[i].AssetAddress.Hex() < corrections[j].AssetAddress.Hex()
	})

	for _, correction := range corrections {
		r.correctionCounter++
		correction.CorrectionId = r.correctionCounter
		r.entryCounter++
		correction.StatementId = r.entryCounter
		modelChan <- &correction
		key := assetHolderKey{AssetAddress: correction.AssetAddress, Holder: correction.Holder}
		r.accountLedger[key] = correction.EndBal
	}
}

// ---------------------------------------------------------
func (r *Reconciler3) ProcessStream(modelChan chan<- types.Modeler) {
	postingStream := make(chan ledger4.AssetTransfer, 100)
	go func() {
		defer close(postingStream)
		var prevBlock base.Blknum
		for _, app := range r.apps {
			bn := base.Blknum(app.BlockNumber)
			if bn != prevBlock && prevBlock != 0 {
				postingStream <- ledger4.AssetTransfer{
					BlockNumber:  prevBlock,
					AssetAddress: base.EndOfBlockSentinel,
				}
			}
			for p := range r.getPostingChannel(&app) {
				postingStream <- p
			}
			prevBlock = bn
		}
		if prevBlock != 0 {
			postingStream <- ledger4.AssetTransfer{
				BlockNumber:  prevBlock,
				AssetAddress: base.EndOfBlockSentinel,
			}
		}
		postingStream <- ledger4.AssetTransfer{
			AssetAddress: base.EndOfStreamSentinel,
		}
	}()

	var postings []ledger4.AssetTransfer
	for posting := range postingStream {
		switch posting.AssetAddress {
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

// ---------------------------------------------------------
func (r *Reconciler3) initData() {
	folder := os.Getenv("FOLDER")
	if folder == "" {
		folder = "tests"
	}
	// blockNumber,transactionIndex
	appsFn := filepath.Join(folder, "apps.csv")
	appsFile, _ := os.Open(appsFn)
	defer appsFile.Close()
	appsReader := csv.NewReader(appsFile)
	appsReader.Comment = '#'
	if appsRecords, err := appsReader.ReadAll(); err != nil {
		fmt.Println("Problem with data file:", appsFn)
		logger.Fatal(err)
	} else if len(appsRecords) == 0 {
		logger.Fatal("no transfers")
	} else {
		for _, record := range appsRecords[1:] {
			if strings.HasPrefix(record[0], "#") {
				continue
			}
			r.apps = append(r.apps, types.Appearance{
				BlockNumber:      uint32(base.MustParseInt64(record[0])),
				TransactionIndex: uint32(base.MustParseInt64(record[1])),
			})
		}
	}

	// blockNumber,assetAddress,accountedFor,endBal
	balFn := filepath.Join(folder, "balances.csv")
	balFile, _ := os.Open(balFn)
	defer balFile.Close()
	balReader := csv.NewReader(balFile)
	balReader.Comment = '#'
	if balRecords, err := balReader.ReadAll(); err != nil {
		fmt.Println("Problem with data file:", balFn)
		logger.Fatal(err)
	} else if len(balRecords) == 0 {
		logger.Fatal("no transfers")
	} else {
		for _, record := range balRecords[1:] {
			if strings.HasPrefix(record[0], "#") {
				continue
			}
			b := ledger4.AssetTransfer{
				BlockNumber:  base.Blknum(base.MustParseUint64(record[0])),
				AssetAddress: base.HexToAddress(record[1]),
				Holder:       base.HexToAddress(record[2]),
				EndBal:       *base.NewWeiStr(record[3]),
			}

			key := bnAssetHolderKey{BlockNumber: b.BlockNumber, AssetAddress: b.AssetAddress, Holder: b.Holder}
			r.conn.balanceMap[key] = b.EndBal
		}
	}

	// blockNumber,transactionIndex,logIndex,assetAddress,accountedFor,amountNet,endBal
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
			p := ledger4.AssetTransfer{
				BlockNumber:      base.Blknum(base.MustParseUint64(record[0])),
				TransactionIndex: base.Txnum(base.MustParseUint64(record[1])),
				LogIndex:         base.Lognum(base.MustParseUint64(record[2])),
				AssetAddress:     base.HexToAddress(record[3]),
				AmountIn:         *base.ZeroWei,
				AmountOut:        *base.ZeroWei,
				Holder:           base.HexToAddress(record[4]),
			}
			if amt.Cmp(base.ZeroWei) > 0 {
				p.AmountIn = *amt
			} else if amt.Cmp(base.ZeroWei) < 0 {
				p.AmountOut = *amt.Neg()
			}
			eb, _ := r.conn.GetBalanceAtToken(p.AssetAddress, p.Holder, p.BlockNumber)
			p.EndBal = *eb

			key := blockTxKey{BlockNumber: p.BlockNumber, TransactionIndex: p.TransactionIndex}
			r.transfers[key] = append(r.transfers[key], p)
		}
		if firstBlock := os.Getenv("FIRST_BLOCK"); firstBlock != "" {
			r.hasStartBlock = true
		}
	}
}

// ---------------------------------------------------------
type blockTxKey struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
}

// ---------------------------------------------------------
type assetHolderKey struct {
	AssetAddress base.Address
	Holder       base.Address
}

// ---------------------------------------------------------
type bnAssetHolderKey struct {
	BlockNumber  base.Blknum
	AssetAddress base.Address
	Holder       base.Address
}
