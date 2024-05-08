package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoStatus tests the status sdk function
func DoStatus() {
	opts := sdk.StatusOptions{}
	ShowHeader("DoStatus", &opts)

	// FirstRecord uint64 `json:"firstRecord,omitempty"`
	// MaxRecords  uint64 `json:"maxRecords,omitempty"`
	// Chains      bool   `json:"chains,omitempty"`
	// func (opts *StatusOptions) StatusIndex() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusBlooms() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusBlocks() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusTransactions() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusTraces() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusLogs() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusStatements() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusResults() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusState() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusTokens() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusMonitors() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusNames() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusAbis() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusSlurps() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusStaging() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusUnripe() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusMaps() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusSome() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusAll() ([]types.Status, *types.MetaData, error) {
	// func (opts *StatusOptions) StatusDiagnose() ([]bool, *types.MetaData, error) {
	// NoSM    StatusModes = 0
	// SMIndex             = 1 << iota
	// SMBlooms
	// SMBlocks
	// SMTransactions
	// SMTraces
	// SMLogs
	// SMStatements
	// SMResults
	// SMState
	// SMTokens
	// SMMonitors
	// SMNames
	// SMAbis
	// SMSlurps
	// SMStaging
	// SMUnripe
	// SMMaps
	// SMSome = SMIndex | SMBlooms | SMBlocks | SMTransactions
	// SMAll  = SMIndex | SMBlooms | SMBlocks | SMTransactions | SMTraces | SMLogs | SMStatements | SMResults | SMState | SMTokens | SMMonitors | SMNames | SMAbis | SMSlurps | SMStaging | SMUnripe | SMMaps

	if indexes, _, err := opts.StatusIndex(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusIndex.json", indexes); err != nil {
			logger.Error(err)
		}
	}

	if blooms, _, err := opts.StatusBlooms(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusBlooms.json", blooms); err != nil {
			logger.Error(err)
		}
	}

	if blocks, _, err := opts.StatusBlocks(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusBlocks.json", blocks); err != nil {
			logger.Error(err)
		}
	}

	if transactions, _, err := opts.StatusTransactions(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusTransactions.json", transactions); err != nil {
			logger.Error(err)
		}
	}

	if traces, _, err := opts.StatusTraces(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusTraces.json", traces); err != nil {
			logger.Error(err)
		}
	}

	if logs, _, err := opts.StatusLogs(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusLogs.json", logs); err != nil {
			logger.Error(err)
		}
	}

	if statements, _, err := opts.StatusStatements(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusStatements.json", statements); err != nil {
			logger.Error(err)
		}
	}

	if results, _, err := opts.StatusResults(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusResults.json", results); err != nil {
			logger.Error(err)
		}
	}

	if state, _, err := opts.StatusState(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusState.json", state); err != nil {
			logger.Error(err)
		}
	}

	if tokens, _, err := opts.StatusTokens(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusTokens.json", tokens); err != nil {
			logger.Error(err)
		}
	}

	if monitors, _, err := opts.StatusMonitors(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusMonitors.json", monitors); err != nil {
			logger.Error(err)
		}
	}

	if names, _, err := opts.StatusNames(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusNames.json", names); err != nil {
			logger.Error(err)
		}
	}

	if abis, _, err := opts.StatusAbis(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusAbis.json", abis); err != nil {
			logger.Error(err)
		}
	}

	if slurps, _, err := opts.StatusSlurps(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusSlurps.json", slurps); err != nil {
			logger.Error(err)
		}
	}

	if staging, _, err := opts.StatusStaging(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusStaging.json", staging); err != nil {
			logger.Error(err)
		}
	}

	if unripe, _, err := opts.StatusUnripe(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusUnripe.json", unripe); err != nil {
			logger.Error(err)
		}
	}

	if maps, _, err := opts.StatusMaps(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusMaps.json", maps); err != nil {
			logger.Error(err)
		}
	}

	if some, _, err := opts.StatusSome(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusSome.json", some); err != nil {
			logger.Error(err)
		}
	}

	if all, _, err := opts.StatusAll(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("sdkFuzzer/statusAll.json", all); err != nil {
			logger.Error(err)
		}
	}

	// if diagnose, _, err := opts.StatusDiagnose(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveAndClean[bool]("sdkFuzzer/statusDiagnose.json", diagnose, &opts, func() error {
	// 		_, _, err := opts.StatusDiagnose()
	// 		return err
	// 	}); err != nil {
	// 		logger.Error(err)
	// 	}
	// }
}
