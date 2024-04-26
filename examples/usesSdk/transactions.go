package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoTransactions tests the When sdk function
func DoTransactions() {
	logger.Info("DoTransactions")

	opts := sdk.TransactionsOptions{
		TransactionIds: []string{"10001002.0"},
	}

	buf := bytes.Buffer{}
	if err := opts.TransactionsBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/transactions.json", buf.String())
	fmt.Println(buf.String())
}
// func (opts *TransactionsOptions) Transactions() ([]types.Transaction, *types.MetaData, error) {
// func (opts *TransactionsOptions) TransactionsTraces() ([]types.Trace, *types.MetaData, error) {
// func (opts *TransactionsOptions) TransactionsUniq() ([]types.Appearance, *types.MetaData, error) {
// func (opts *TransactionsOptions) TransactionsLogs() ([]types.Log, *types.MetaData, error) {
