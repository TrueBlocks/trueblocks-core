package ledger2

import (
	"encoding/json"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ledger10"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/normalize"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pricing"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

type Reconciler2 struct {
	connection        *rpc.Connection
	accountFor        base.Address
	firstBlock        base.Blknum
	lastBlock         base.Blknum
	asEther           bool
	testMode          bool
	reversed          bool
	useTraces         bool
	assetFilter       []base.Address
	names             map[base.Address]types.Name
	hasStartBlock     bool
	transfers         map[blockTxKey][]ledger10.AssetTransfer
	accountLedger     map[assetHolderKey]base.Wei
	ledgerAssets      map[base.Address]bool
	correctionCounter base.Value
	entryCounter      base.Value
	ledgers           map[base.Address]Ledger
}

func (r *Reconciler2) String() string {
	bytes, _ := json.MarshalIndent(r, "", "  ")
	return string(bytes)
}

func NewReconciler(opts *ledger10.ReconcilerOptions) *Reconciler2 {
	parts := types.Custom | types.Prefund | types.Regular
	names, _ := names.LoadNamesMap(opts.Connection.Chain, parts, []string{})
	return &Reconciler2{
		connection:    opts.Connection,
		accountFor:    opts.AccountFor,
		firstBlock:    opts.FirstBlock,
		lastBlock:     opts.LastBlock,
		asEther:       opts.AsEther,
		testMode:      opts.TestMode,
		reversed:      opts.Reversed,
		useTraces:     opts.UseTraces,
		assetFilter:   opts.AssetFilters,
		names:         names,
		hasStartBlock: false,
		transfers:     make(map[blockTxKey][]ledger10.AssetTransfer),
		accountLedger: make(map[assetHolderKey]base.Wei),
		ledgerAssets:  make(map[base.Address]bool),
		ledgers:       make(map[base.Address]Ledger),
	}
}

func (r *Reconciler2) getStatementsInner(pos *types.AppPosition, trans *types.Transaction, allTransfers []ledger10.AssetTransfer) {
	appearanceByID := make(map[string]types.Appearance)
	app := types.Appearance{Address: r.accountFor, BlockNumber: uint32(trans.BlockNumber), TransactionIndex: uint32(trans.TransactionIndex)}
	appID := fmt.Sprintf("%d-%d", app.BlockNumber, app.TransactionIndex)
	appearanceByID[appID] = app

	entriesMap := make(map[string]*LedgerEntry)
	for _, at := range allTransfers {
		appID := fmt.Sprintf("%d-%d", at.BlockNumber, at.TransactionIndex)
		app, ok := appearanceByID[appID]
		if !ok {
			continue
		}

		key := fmt.Sprintf("%s|%s", at.Asset, appID)
		entry, found := entriesMap[key]
		if !found {
			newEntry := NewLedgerEntry(appID, base.Blknum(app.BlockNumber), base.Txnum(app.TransactionIndex))
			entriesMap[key] = &newEntry
			entry = &newEntry
		}

		posting := r.queryBalances(pos, at)

		if posting.IsMaterial() {
			posting.SpotPrice, posting.PriceSource, _ = pricing.PriceUsd(r.connection, (*types.Statement)(&posting.Statement))
		}

		if file.IsTestMode() {
			(*types.Statement)(&posting.Statement).DebugStatement(pos)
		}

		entry.Postings = append(entry.Postings, posting)
	}

	for key, lePtr := range entriesMap {
		asset := key[:findSeparator(key)]
		ll, found := r.GetLedger(base.HexToAddress(asset))
		if !found {
			ll = NewLedger(base.HexToAddress(asset))
		}
		ll.Entries = append(ll.Entries, *lePtr)
		r.ledgers[ll.Asset] = ll
	}
}

func (r *Reconciler2) queryBalances(pos *types.AppPosition, at ledger10.AssetTransfer) types.Posting {
	ret := types.Posting{}
	ret.Statement = at

	prevBal, _ := r.connection.GetBalanceAtToken(at.Asset, r.accountFor, pos.Prev)
	if at.BlockNumber == 0 {
		prevBal = new(base.Wei)
	}
	ret.PrevBal = *prevBal

	begBal, _ := r.connection.GetBalanceAtToken(at.Asset, r.accountFor, at.BlockNumber-1)
	if at.BlockNumber == 0 {
		begBal = new(base.Wei)
	}
	ret.BegBal = *begBal

	endBal, _ := r.connection.GetBalanceAtToken(at.Asset, r.accountFor, at.BlockNumber)
	ret.EndBal = *endBal

	return ret
}

func (r *Reconciler2) ReconcileCheckpoint(block base.Blknum) {
	_ = block
}

func findSeparator(s string) int {
	for i := 0; i < len(s); i++ {
		if s[i] == '|' {
			return i
		}
	}
	return len(s)
}

type blockTxKey struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
}

type assetHolderKey struct {
	Asset  base.Address
	Holder base.Address
}

func (r *Reconciler2) GetLedger(asset base.Address) (Ledger, bool) {
	l, ok := r.ledgers[asset]
	return l, ok
}

func (r *Reconciler2) Statements() ([]types.Statement, error) {
	var stmts []types.Statement

	for _, l := range r.ledgers {
		for _, entry := range l.Entries {
			for _, posting := range entry.Postings {
				s := posting.Statement
				stmts = append(stmts, s)
			}
		}
	}

	return stmts, nil
}

func (r *Reconciler2) getAssetTransfers2(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]ledger10.AssetTransfer, error) {
	if r.connection.Store != nil {
		statementGroup := &types.StatementGroup{
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			Address:          r.accountFor,
		}
		if err := r.connection.Store.Read(statementGroup); err == nil {
			return statementGroup.Statements, nil
		}
	}

	type AAA = ledger10.AssetTransfer

	results := make([]AAA, 0, 20)
	var err error
	if ledger10.AssetOfInterest(r.assetFilter, base.FAKE_ETH_ADDRESS) {
		accountedFor := r.accountFor
		at := AAA{
			AccountedFor:     accountedFor,
			Sender:           trans.From,
			Recipient:        trans.To,
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			TransactionHash:  trans.Hash,
			LogIndex:         0,
			Timestamp:        trans.Timestamp,
			Asset:            base.FAKE_ETH_ADDRESS,
			Symbol:           "WEI",
			Decimals:         18,
			SpotPrice:        0.0,
			PriceSource:      "not-priced",
		}

		if r.asEther {
			at.Symbol = "ETH"
		}

		if trans.To.IsZero() && trans.Receipt != nil && !trans.Receipt.ContractAddress.IsZero() {
			at.Recipient = trans.Receipt.ContractAddress
		}

		if at.Sender == accountedFor {
			gasUsed := new(base.Wei)
			if trans.Receipt != nil {
				gasUsed.SetUint64(uint64(trans.Receipt.GasUsed))
			}
			gasPrice := new(base.Wei).SetUint64(uint64(trans.GasPrice))
			gasOut := new(base.Wei).Mul(gasUsed, gasPrice)
			at.AmountOut = trans.Value
			at.GasOut = *gasOut
		}

		if at.Recipient == accountedFor {
			if at.BlockNumber == 0 {
				at.PrefundIn = trans.Value
			} else {
				if trans.Rewards != nil {
					at.MinerBaseRewardIn = trans.Rewards.Block
					at.MinerNephewRewardIn = trans.Rewards.Nephew
					at.MinerTxFeeIn = trans.Rewards.TxFee
					at.MinerUncleRewardIn = trans.Rewards.Uncle
				} else {
					at.AmountIn = trans.Value
				}
			}
		}

		if !utils.IsFuzzing() {
			if at.IsMaterial() {
				results = append(results, at)
			}
		}
	}

	if trans.Receipt != nil {
		receiptStatements := make([]types.Statement, 0, 20)
		for _, log := range trans.Receipt.Logs {
			if log.Topics[0] != topics.TransferTopic {
				continue
			}
			addrArray := []base.Address{r.accountFor}
			if filter.ApplyLogFilter(&log, addrArray) && ledger10.AssetOfInterest(r.assetFilter, log.Address) {
				normalized, err := normalize.NormalizeKnownLogs(&log)
				if err != nil {
					continue
				} else if normalized.IsNFT() {
					continue
				} else {
					sender := base.HexToAddress(normalized.Topics[1].Hex())
					recipient := base.HexToAddress(normalized.Topics[2].Hex())
					isSender, isRecipient := r.accountFor == sender, r.accountFor == recipient
					if utils.IsFuzzing() || (!isSender && !isRecipient) {
						continue
					}

					sym := normalized.Address.DefaultSymbol()
					decimals := base.Value(18)
					name := r.names[normalized.Address]
					if name.Address == normalized.Address {
						if name.Symbol != "" {
							sym = name.Symbol
						}
						if name.Decimals != 0 {
							decimals = base.Value(name.Decimals)
						}
					}

					var amountIn, amountOut base.Wei
					amount, _ := new(base.Wei).SetString(strings.ReplaceAll(normalized.Data, "0x", ""), 16)
					if amount == nil {
						amount = base.NewWei(0)
					}
					if r.accountFor == sender {
						amountOut = *amount
					}
					if r.accountFor == recipient {
						amountIn = *amount
					}
					s := AAA{
						AccountedFor:     r.accountFor,
						Sender:           sender,
						Recipient:        recipient,
						BlockNumber:      normalized.BlockNumber,
						TransactionIndex: normalized.TransactionIndex,
						LogIndex:         normalized.LogIndex,
						TransactionHash:  normalized.TransactionHash,
						Timestamp:        normalized.Timestamp,
						Asset:            normalized.Address,
						Symbol:           sym,
						AmountIn:         amountIn,
						AmountOut:        amountOut,
						Decimals:         decimals,
						SpotPrice:        0.0,
						PriceSource:      "not-priced",
					}

					if s.IsMaterial() {
						receiptStatements = append(receiptStatements, s)
					}
				}
			}
		}
		results = append(results, receiptStatements...)
	}

	allReconciled := true
	for _, statement := range results {
		if statement.IsMaterial() && !statement.Reconciled() {
			allReconciled = false
			break
		}
	}

	statementGroup := &types.StatementGroup{
		BlockNumber:      trans.BlockNumber,
		TransactionIndex: trans.TransactionIndex,
		Address:          r.accountFor,
		Statements:       results,
	}
	err = r.connection.Store.WriteToCache(statementGroup, walk.Cache_Statements, trans.Timestamp, allReconciled, false)
	return results, err
}

type LedgerEntry struct {
	AppearanceID     string
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Postings         []types.Posting
}

func NewLedgerEntry(appearanceID string, blockNum base.Blknum, txIndex base.Txnum) LedgerEntry {
	return LedgerEntry{
		AppearanceID:     appearanceID,
		BlockNumber:      blockNum,
		TransactionIndex: txIndex,
		Postings:         make([]types.Posting, 0),
	}
}

func (le *LedgerEntry) String() string {
	bytes, _ := json.Marshal(le)
	return string(bytes)
}

func (le *LedgerEntry) TotalIn() base.Wei {
	total := base.ZeroWei
	for _, posting := range le.Postings {
		tmp := posting.TotalIn()
		total = new(base.Wei).Add(total, tmp)
	}
	return *total
}

func (le *LedgerEntry) TotalOut() base.Wei {
	total := base.ZeroWei
	for _, posting := range le.Postings {
		tmp := posting.TotalOut()
		total = new(base.Wei).Add(total, tmp)
	}
	return *total
}

func (le *LedgerEntry) NetValue() base.Wei {
	in := le.TotalIn()
	out := le.TotalOut()
	net := new(base.Wei).Sub(&in, &out)
	return *net
}

type Ledger struct {
	Asset   base.Address
	Entries []LedgerEntry
}

func NewLedger(asset base.Address) Ledger {
	return Ledger{
		Asset:   asset,
		Entries: make([]LedgerEntry, 0),
	}
}

func (l *Ledger) String() string {
	totIn := l.TotalIn()
	totOut := l.TotalOut()
	net := l.NetValue()
	return fmt.Sprintf(
		"Ledger(Asset=%s Entries=%d In=%s Out=%s Net=%s)",
		l.Asset,
		len(l.Entries),
		totIn.String(),
		totOut.String(),
		net.String(),
	)
}

func (l *Ledger) TotalIn() base.Wei {
	total := base.NewWei(0)
	for _, entry := range l.Entries {
		entryIn := entry.TotalIn()
		total = new(base.Wei).Add(total, &entryIn)
	}
	return *total
}

func (l *Ledger) TotalOut() base.Wei {
	total := base.NewWei(0)
	for _, entry := range l.Entries {
		entryOut := entry.TotalOut()
		total = new(base.Wei).Add(total, &entryOut)
	}
	return *total
}

func (l *Ledger) NetValue() base.Wei {
	in := l.TotalIn()
	out := l.TotalOut()
	net := new(base.Wei).Sub(&in, &out)
	return *net
}

func (r *Reconciler2) GetStatements(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Statement, error) {
	if transfers, err := r.getAssetTransfers2(pos, filter, trans); err != nil {
		return []types.Statement{}, err
	} else {
		r.getStatementsInner(pos, trans, transfers)
		return r.Statements()
	}
}

func (r *Reconciler2) GetTransfers(pos *types.AppPosition, filter *filter.AppearanceFilter, trans *types.Transaction) ([]types.Transfer, error) {
	if r.connection.Store != nil {
		transferGroup := &types.TransferGroup{
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
		}
		if err := r.connection.Store.Read(transferGroup); err == nil {
			return transferGroup.Transfers, nil
		}
	}

	var err error
	var statements []types.Statement
	ledgerOpts := &ledger10.ReconcilerOptions{
		Connection:   r.connection,
		AccountFor:   r.accountFor,
		AsEther:      r.asEther,
		AssetFilters: r.assetFilter,
	}
	r2 := NewReconciler(ledgerOpts)
	if statements, err = r2.GetStatements(pos, filter, trans); err != nil {
		return nil, err
	}

	if transfers, err := types.ConvertToTransfers(statements); err != nil {
		return nil, err
	} else {

		allReconciled := true
		for _, transfer := range transfers {
			if transfer.IsMaterial() {
				allReconciled = false
				break
			}
		}

		transfersGroup := &types.TransferGroup{
			BlockNumber:      trans.BlockNumber,
			TransactionIndex: trans.TransactionIndex,
			Transfers:        transfers,
		}
		err = r.connection.Store.WriteToCache(transfersGroup, walk.Cache_Transfers, trans.Timestamp, allReconciled, false)
		return transfers, err
	}
}
