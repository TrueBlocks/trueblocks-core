package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoChunks tests the When sdk function
func DoChunks() {
	logger.Info("DoChunks")

	opts := sdk.ChunksOptions{
		Mode: sdk.CMIndex,
	}

	buf := bytes.Buffer{}
	if err := opts.ChunksBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/chunks.json", buf.String())
	fmt.Println(buf.String())
}
// func (opts *ChunksOptions) ChunksManifest() ([]types.ChunkRecord, *types.MetaData, error) {
// func (opts *ChunksOptions) ChunksIndex() ([]types.ChunkIndex, *types.MetaData, error) {
// func (opts *ChunksOptions) ChunksBlooms() ([]types.ChunkBloom, *types.MetaData, error) {
// func (opts *ChunksOptions) ChunksPins() ([]types.ChunkPinReport, *types.MetaData, error) {
// func (opts *ChunksOptions) ChunksAddresses() ([]types.ChunkAddress, *types.MetaData, error) {
// // func (opts *ChunksOptions) ChunksAppearances() ([]types.ChunkAppearance, *types.MetaData, error) {
// func (opts *ChunksOptions) ChunkStats() ([]types.ChunkStats, *types.MetaData, error) {
