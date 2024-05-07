package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoTransactions tests the transactions sdk function
func DoTransactions() {
	opts := sdk.TransactionsOptions{
		TransactionIds: testTransactions,
	}
	ShowHeader("DoTransactions", &opts)

	if transactions, _, err := opts.Transactions(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Transaction]("usesSDK/transactions.json", transactions, &opts, func() error {
			_, _, err := opts.Transactions()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if transactionsTraces, _, err := opts.TransactionsTraces(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Trace]("usesSDK/transactionsTraces.json", transactionsTraces, &opts, func() error {
			_, _, err := opts.TransactionsTraces()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if transactionsUniq, _, err := opts.TransactionsUniq(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Appearance]("usesSDK/transactionsUniq.json", transactionsUniq, &opts, func() error {
			_, _, err := opts.TransactionsUniq()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	opts.Articulate = true
	if transactionsLogs, _, err := opts.TransactionsLogs(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Log]("usesSDK/transactionsLogs.json", transactionsLogs, &opts, func() error {
			_, _, err := opts.TransactionsLogs()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}
}
