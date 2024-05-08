package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoSlurp tests the slurp sdk function
func DoSlurp() {
	opts := sdk.SlurpOptions{
		Addrs:   []string{testAddrs[0]},
		PerPage: 10,
	}
	ShowHeader("DoSlurp", &opts)

	if slurp, _, err := opts.Slurp(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Slurp]("sdkFuzzer/slurp.json", slurp, &opts, func() error {
			_, _, err := opts.Slurp()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if appearances, _, err := opts.SlurpAppearances(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Appearance]("sdkFuzzer/slurpAppearances.json", appearances, &opts, func() error {
			_, _, err := opts.SlurpAppearances()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if counts, _, err := opts.SlurpCount(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.SlurpCount]("sdkFuzzer/slurpCount.json", counts, &opts, func() error {
			_, _, err := opts.SlurpCount()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

}

// Addrs      []string    `json:"addrs,omitempty"`
// BlockIds   []string    `json:"blocks,omitempty"`
// Types      SlurpTypes  `json:"types,omitempty"`
// Articulate bool        `json:"articulate,omitempty"`
// Source     SlurpSource `json:"source,omitempty"`
// Page       uint64      `json:"page,omitempty"`
// PageId     string      `json:"pageId,omitempty"`
// PerPage    uint64      `json:"perPage,omitempty"`
// Sleep      float64     `json:"sleep,omitempty"`
// func (opts *SlurpOptions) Slurp() ([]types.Slurp, *types.MetaData, error) {
// func (opts *SlurpOptions) SlurpAppearances() ([]types.Appearance, *types.MetaData, error) {
// func (opts *SlurpOptions) SlurpCount() ([]types.SlurpCount, *types.MetaData, error) {
// NoST  SlurpTypes = 0
// STExt            = 1 << iota
// STInt
// STToken
// STNfts
// ST1155
// STMiner
// STUncles
// STWithdrawals
// STSome = STExt | STInt | STToken | STNfts
// STAll  = STExt | STInt | STToken | STNfts | ST1155 | STMiner | STUncles | STWithdrawals
// NoSS        SlurpSource = 0
// SSEtherscan             = 1 << iota
// SSKey
// SSCovalent
// SSAlchemy
