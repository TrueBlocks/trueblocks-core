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

	if indexes, _, err := opts.StatusIndex(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusIndex.json", indexes); err != nil {
			logger.Error(err)
		}
	}

	if blooms, _, err := opts.StatusBlooms(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusBlooms.json", blooms); err != nil {
			logger.Error(err)
		}
	}

	if blocks, _, err := opts.StatusBlocks(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusBlocks.json", blocks); err != nil {
			logger.Error(err)
		}
	}

	if transactions, _, err := opts.StatusTransactions(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusTransactions.json", transactions); err != nil {
			logger.Error(err)
		}
	}

	if traces, _, err := opts.StatusTraces(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusTraces.json", traces); err != nil {
			logger.Error(err)
		}
	}

	if logs, _, err := opts.StatusLogs(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusLogs.json", logs); err != nil {
			logger.Error(err)
		}
	}

	if statements, _, err := opts.StatusStatements(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusStatements.json", statements); err != nil {
			logger.Error(err)
		}
	}

	if results, _, err := opts.StatusResults(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusResults.json", results); err != nil {
			logger.Error(err)
		}
	}

	if state, _, err := opts.StatusState(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusState.json", state); err != nil {
			logger.Error(err)
		}
	}

	if tokens, _, err := opts.StatusTokens(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusTokens.json", tokens); err != nil {
			logger.Error(err)
		}
	}

	if monitors, _, err := opts.StatusMonitors(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusMonitors.json", monitors); err != nil {
			logger.Error(err)
		}
	}

	if names, _, err := opts.StatusNames(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusNames.json", names); err != nil {
			logger.Error(err)
		}
	}

	if abis, _, err := opts.StatusAbis(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusAbis.json", abis); err != nil {
			logger.Error(err)
		}
	}

	if slurps, _, err := opts.StatusSlurps(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusSlurps.json", slurps); err != nil {
			logger.Error(err)
		}
	}

	if staging, _, err := opts.StatusStaging(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusStaging.json", staging); err != nil {
			logger.Error(err)
		}
	}

	if unripe, _, err := opts.StatusUnripe(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusUnripe.json", unripe); err != nil {
			logger.Error(err)
		}
	}

	if maps, _, err := opts.StatusMaps(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusMaps.json", maps); err != nil {
			logger.Error(err)
		}
	}

	if some, _, err := opts.StatusSome(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusSome.json", some); err != nil {
			logger.Error(err)
		}
	}

	if all, _, err := opts.StatusAll(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Status]("usesSDK/statusAll.json", all); err != nil {
			logger.Error(err)
		}
	}

	// if diagnose, _, err := opts.StatusDiagnose(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveAndClean[bool]("usesSDK/statusDiagnose.json", diagnose, &opts, func() error {
	// 		_, _, err := opts.StatusDiagnose()
	// 		return err
	// 	}); err != nil {
	// 		logger.Error(err)
	// 	}
	// }
}
