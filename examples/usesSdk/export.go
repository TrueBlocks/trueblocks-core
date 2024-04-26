package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoExport tests the When sdk function
func DoExport() {
	logger.Info("DoExport")

	opts := sdk.ExportOptions{
		Addrs: []string{testAddrs[0]},
	}

	buf := bytes.Buffer{}
	if err := opts.ExportBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/export.json", buf.String())
	fmt.Println(buf.String())
}
// func (opts *ExportOptions) Export() ([]types.Transaction, *types.MetaData, error) {
// func (opts *ExportOptions) ExportAppearances() ([]types.Appearance, *types.MetaData, error) {
// func (opts *ExportOptions) ExportReceipts() ([]types.Receipt, *types.MetaData, error) {
// func (opts *ExportOptions) ExportLogs() ([]types.Log, *types.MetaData, error) {
// func (opts *ExportOptions) ExportTraces() ([]types.Trace, *types.MetaData, error) {
// func (opts *ExportOptions) ExportStatements() ([]types.Statement, *types.MetaData, error) {
// func (opts *ExportOptions) ExportBalances() ([]types.State, *types.MetaData, error) {
// func (opts *ExportOptions) ExportWithdrawals() ([]types.Withdrawal, *types.MetaData, error) {
// func (opts *ExportOptions) ExportCount() ([]types.AppearanceCount, *types.MetaData, error) {
