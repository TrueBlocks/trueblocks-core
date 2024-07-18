package namesPkg

// TODO: New tests -- chifra names --autoname (address in the environment)
// TODO: New tests -- chifra names --autoname <address>

import (
	"context"
	"errors"
	"fmt"
	"path/filepath"
	"strings"
	"sync/atomic"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/prefunds"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *NamesOptions) HandleClean(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain

	label := "custom"
	db := names.DatabaseCustom
	if opts.Regular {
		label = "regular"
		db = names.DatabaseRegular
	}
	sourcePath := filepath.Join(config.MustGetPathToChainConfig(chain), string(db))
	logger.Info("Processing", label, "names file", "("+sourcePath+")")
	destinationLabel := sourcePath
	if opts.DryRun {
		destinationLabel = "standard output"
	}
	logger.Info("Writing results to", destinationLabel)

	var message string
	modifiedCount, err := opts.cleanNames()
	if err != nil {
		message = fmt.Sprintf("The %s names database was not cleaned", label)
		logger.Warn(message)
	} else {
		message = fmt.Sprintf("The %s names database was cleaned. %d names have been modified", label, modifiedCount)
		if modifiedCount == 1 {
			message = strings.Replace(message, "names have been", "name has been", 1)
		}
		logger.Info(message)
	}

	if opts.Globals.IsApiMode() {
		fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
			modelChan <- &types.Message{
				Msg: message,
			}
		}
		rCtx := output.NewRenderContext()
		_ = output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
	}
	return err
}

func (opts *NamesOptions) cleanNames() (int, error) {
	chain := opts.Globals.Chain

	parts := names.Custom
	if opts.Regular {
		parts = names.Regular
	}

	// Load databases
	allNames, err := names.LoadNamesMap(chain, parts, []string{})
	if err != nil {
		return 0, err
	}
	prefundMap, err := preparePrefunds(chain)
	if err != nil {
		return 0, err
	}

	// Prepare progress reporting. We will report percentage.
	total := len(allNames)
	var done atomic.Int32
	modifiedCount := 0
	type Progress struct {
		ProgressDelta int32
		Modified      bool
	}
	progressChan := make(chan Progress)
	defer close(progressChan)
	// Listen on a channel and whenever it updates, call `reportProgress`
	go func() {
		for progress := range progressChan {
			doneNow := done.Add(progress.ProgressDelta)
			if progress.Modified {
				modifiedCount += int(progress.ProgressDelta)
			}
			logger.PctProgress(doneNow, total, 10)
		}
	}()

	defer func() {
		// Clean line after progress report.
		if done.Load() > 0 {
			logger.CleanLine()
		}
	}()

	iterFunc := func(address base.Address, name types.Name) error {
		modified, err := cleanName(chain, &name)
		if err != nil {
			return wrapErrorWithAddr(&address, err)
		}
		if isPrefund := prefundMap[name.Address]; isPrefund != name.IsPrefund {
			name.IsPrefund = isPrefund
			modified = true
		}

		progressChan <- Progress{
			ProgressDelta: 1,
			Modified:      modified,
		}

		if !modified {
			return nil
		}

		// update names in-memory cache
		if opts.Regular {
			if err = names.UpdateName(names.DatabaseRegular, chain, &name); err != nil {
				return wrapErrorWithAddr(&address, err)
			}
		} else {
			if err = names.UpdateName(names.DatabaseCustom, chain, &name); err != nil {
				return wrapErrorWithAddr(&address, err)
			}
		}
		return nil
	}

	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()
	errorChan := make(chan error)
	go utils.IterateOverMap(ctx, errorChan, allNames, iterFunc)

	// Block until we get an error from any of the iterations or the iteration finishes
	if stepErr := <-errorChan; stepErr != nil {
		cancel()
		return 0, stepErr
	}

	if modifiedCount == 0 {
		return 0, nil
	}

	if opts.Regular {
		return modifiedCount, names.RegularWriteNames(chain, opts.DryRun)
	}

	return modifiedCount, names.CustomWriteNames(chain, opts.DryRun)
}

// wrapErrorWithAddr prepends `err` with `address`, so that we can learn which name caused troubles
func wrapErrorWithAddr(address *base.Address, err error) error {
	return fmt.Errorf("%s: %w", address, err)
}

func preparePrefunds(chain string) (results map[base.Address]bool, err error) {
	prefunds, err := prefunds.LoadPrefunds(
		chain,
		prefunds.GetPrefundPath(chain),
		nil,
	)
	if err != nil {
		return
	}

	results = make(map[base.Address]bool, len(prefunds))
	for _, prefund := range prefunds {
		results[prefund.Address] = true
	}
	return
}

func cleanName(chain string, name *types.Name) (modified bool, err error) {
	conn := rpc.TempConnection(chain)
	if err = conn.IsContractAtLatest(name.Address); err != nil && !errors.Is(err, rpc.ErrNotAContract) {
		return
	}

	isContract := !errors.Is(err, rpc.ErrNotAContract)
	wasContract := name.IsContract && !isContract
	modified = cleanCommon(name)

	if !isContract {
		err = nil // not an error to not be a contract
		if mod := cleanNonContract(name, wasContract); mod {
			modified = true
		}
		return
	}

	// If this address is not a token, we're done
	tokenState, err := conn.GetTokenState(name.Address, "latest")
	if err != nil {
		err = nil
	}

	contractModified, err := cleanContract(tokenState, name)
	if err != nil {
		return
	}
	modified = modified || contractModified
	return
}

func cleanCommon(name *types.Name) (modified bool) {
	if name.Tags > "79999" {
		return false
	}

	lowerCaseSource := strings.ToLower(name.Source)
	if lowerCaseSource == "etherscan" {
		name.Source = "EtherScan.io"
		modified = true
	} else if lowerCaseSource == "trueblocks" {
		name.Source = "TrueBlocks.io"
		modified = true
	}

	sourceDedup, strModified := removeDoubleSpaces(name.Source)
	if strModified && name.Source != sourceDedup {
		name.Source = sourceDedup
		modified = true
	}

	return
}

func removeDoubleSpaces(str string) (string, bool) {
	if !strings.Contains(str, "  ") {
		return str, false
	}

	result := strings.ReplaceAll(str, "  ", " ")
	return result, true
}

func cleanContract(token *types.Token, name *types.Name) (modified bool, err error) {
	if !name.IsContract {
		name.IsContract = true
		modified = true
	}

	if token != nil {
		tokenModified := cleanToken(name, token)
		if !modified && tokenModified {
			modified = true
		}
	} else {
		if name.IsErc20 || name.IsErc721 {
			// Not a token
			name.IsErc20 = false
			name.IsErc721 = false
			name.Decimals = 0
			name.Symbol = ""
			if name.Tags == "50-Tokens:ERC20" || name.Tags == "50-Tokens:ERC721" {
				name.Tags = ""
			}
			modified = true
		}
	}

	if name.Tags == "" {
		name.Tags = "30-Contracts"
		modified = true
	}

	trimmedName := strings.Trim(name.Name, " ")
	if name.Name != trimmedName {
		name.Name = trimmedName
		modified = true
	}

	trimmedSymbol := strings.Trim(name.Symbol, " ")
	if name.Symbol != trimmedSymbol {
		name.Symbol = trimmedSymbol
		modified = true
	}

	return
}

func cleanToken(name *types.Name, token *types.Token) (modified bool) {
	if !name.IsErc20 && token.TokenType.IsErc20() {
		name.IsErc20 = true
		modified = true
	}

	airdrop := strings.Contains(name.Name, "airdrop")
	if name.Tags == "60-Airdrops" {
		name.Tags = ""
		modified = true
	}

	if token.TokenType.IsErc20() && (name.Tags == "" ||
		strings.Contains(name.Tags, "token") ||
		strings.Contains(name.Tags, "30-contracts") ||
		strings.Contains(name.Tags, "55-defi") ||
		airdrop) {
		name.Tags = "50-Tokens:ERC20"
		modified = true
	}

	if name.Source != "On chain" &&
		(name.Source == "" || name.Source == "TrueBlocks.io" || name.Source == "EtherScan.io") {
		name.Source = "On chain"
		modified = true
	}

	tokenName := token.Name
	var strModified bool
	if tokenName != "" {
		tokenName, strModified = removeDoubleSpaces(tokenName)
		if strModified && name.Name != tokenName {
			name.Name = tokenName
			modified = true
		}
	}

	// If token name contains 3x `-`, it's Kickback Event, so we need to ignore
	// token.Name, e.g.: 0x2ac0ac19f8680d5e9fdebad515f596265134f018. Comment from C++ code:
	// some sort of hacky renaming for Kickback
	if tokenName != "" && strings.Count(tokenName, "-") < 4 {
		tokenName = strings.Trim(tokenName, " ")
		if name.Name != tokenName {
			name.Name = tokenName
			modified = true
		}
	}

	if token.Symbol != "" {
		tokenSymbol := strings.Trim(token.Symbol, " ")

		if name.Symbol != tokenSymbol {
			name.Symbol = tokenSymbol
			modified = true
		}

		tokenSymbol, strModified = removeDoubleSpaces(token.Symbol)
		if strModified && name.Symbol != tokenSymbol {
			name.Symbol = tokenSymbol
			modified = true
		}
	}

	if token.Decimals > 0 && name.Decimals != uint64(token.Decimals) {
		name.Decimals = uint64(token.Decimals)
		modified = true
	}

	if token.TokenType.IsErc721() && !name.IsErc721 {
		name.IsErc721 = true
		modified = true
	}

	if !token.TokenType.IsErc721() && name.IsErc721 {
		name.IsErc721 = false
		modified = true
	}

	if token.TokenType.IsErc721() && name.IsErc721 && name.Tags == "" {
		name.Tags = "50-Tokens:ERC721"
		modified = true
	}

	return
}

func cleanNonContract(name *types.Name, wasContract bool) (modified bool) {
	if name.Tags == "30-Contracts:Humanity DAO" {
		name.Tags = "90-Individuals:Humanity DAO"
		modified = true
	}

	tagsEmpty := len(name.Tags) == 0
	tagContract := strings.Contains(name.Tags, "Contracts")
	tagToken := strings.Contains(name.Tags, "Tokens")

	if wasContract && name.Tags != "37-SelfDestructed" {
		name.IsContract = true
		name.Tags = "37-SelfDestructed"
		return true
	}

	if (tagsEmpty || tagContract || tagToken) && name.Tags != "90-Individuals:Other" {
		name.Tags = "90-Individuals:Other"
		modified = true
	}
	return
}

// Finish clean
//
// Prequisite:
//		if tags is >= 8 (as a string), return without modification noting that tags over '8' character are reserved
//		latestBlock = testMode ? 10800000 : getLatestBlock_client()
//
// Source:
//		if contains (ignore case) 'etherscan' then the entire string becomes Etherscan.io
//		if contains (ignore case) 'trueblocks' then the entire string becomes TrueBlocks.io
//		change any white space to spaces, change double spaces to single spaces
//
// IsPrefund:
//		is the address a prefund for this chain?
//
// IsContract:
//		'wasContract' (is there a current record, and does that current record have isContract set?)
//		'isContract' (is the address a contract at the current block?)
//		'isAirdrop' (does the account's name contain the word "airdrop"

// static const string_q erc721QueryBytes = "0x" + padRight(substitute(_INTERFACE_ID_ERC721, "0x", ""), 64, '0');
// inline bool isErc721(const address_t& addr, const CAbi& abi_spec, blknum_t latest) {
//     string_q val = get TokenState(addr, "supportsInterface", abi_spec, latest, erc721QueryBytes);
//     return val == "T" || val == "true";
// }

//     bool isAirdrop = containsI(ac count.name, "airdrop");
//     if (ac count.tags == "60-Airdrops")
//         ac count.tags = "";

//     if (!isContract) {
//         bool isEmpty = ac count.tags.empty();
//         bool isContract = contains(ac count.tags, "Contracts");
//         bool isToken = contains(ac count.tags, "Tokens");
//         ac count.tags = !isEmpty && !isContract && !isToken ? ac count.tags : "90-Individuals:Other";
//         if (wasContract) {
//             // This used to be a contract and now is not, so it must be a self destruct
//             ac count.isContract = true;
//             ac count.tags = "37-SelfDestructed";
//         }

//     } else {
//         // This is a contract...
//         ac count.isContract = true;

//         string_q name = getToken State(ac count.address, "name", opts->abi_spec, latestBlock);
//         string_q symbol = getToken State(ac count.address, "symbol", opts->abi_spec, latestBlock);
//         uint64_t decimals = str_2_Uint(getToken State(ac count.address, "decimals", opts->abi_spec, latestBlock));
//         if (!name.empty() || !symbol.empty() || decimals > 0) {
//             ac count.isErc20 = true;
//             ac count.source =
//                 (ac count.source.empty() || ac count.source == "TrueBlocks.io" || ac count.source == "EtherScan.io")
//                     ? "On chain"
//                     : ac count.source;
//             // Use the values from on-chain if we can...
//             ac count.name = (!name.empty() ? name : ac count.name);
//             ac count.symbol = (!symbol.empty() ? symbol : ac count.symbol);
//             ac count.decimals = decimals ? decimals : (ac count.decimals ? ac count.decimals : 18);
//             ac count.isErc721 = isErc721(ac count.address, opts->abi_spec, latestBlock);
//             if (ac count.isErc721) {
//                 ac count.tags = "50-Tokens:ERC721";

//             } else {
//                 // This is an ERC20, so if we've not tagged it specifically, make it thus
//                 if (ac count.tags.empty() || containsI(ac count.tags, "token") ||
//                     containsI(ac count.tags, "30-contracts") || containsI(ac count.tags, "55-defi") || isAirdrop) {
//                     ac count.tags = "50-Tokens:ERC20";
//                 }
//             }

//         } else {
//             ac count.isErc20 = false;
//             ac count.isErc721 = false;
//         }
//         if (ac count.tags.empty())
//             ac count.tags = "30-Contracts";
//     }

//     if (isAirdrop && !containsI(ac count.name, "Airdrop")) {
//         replaceAll(ac count.name, " airdrop", "");
//         replaceAll(ac count.name, " Airdrop", "");
//         ac count.name = ac count.name + " Airdrop";
//     }

//     // Clean up name and symbol
//     ac count.name = trim(substitute(ac count.name, "  ", " "));
//     ac count.symbol = trim(substitute(ac count.symbol, "  ", " "));

//     return !ac count.name.empty();
// }

// 1) There are five files:
//		binary database
//		names.tab in $configPath
//		names_custom.tab in $configPath
//		the original source for names.tab (../src/other/install/names/)
//		the original source for custom_names.tab (in my case ~/Desktop, but generally ../src/other/install/names/)
//
// 2) Clean changes the binary database and updates the files in $configPath
// 3) If [settings]source=<path> and/or [settings]custom=<path> is set in $configPath/ethNames.toml, then
//      re-write these files too.
// 4) We can probably use Update quite easily
// 5) Clean - high level:
// 		Config paths should handle ~ and $HOME
// 		If the source doesn't exist (misconfig?), do nothing
// 		We edit the source and copy it to the destination, then the next time it runs it updates the binary
// 		Message the user that we are updateing source into dest
// 		Read entire source file
// 			for each name
// 				cleanName (use the same function we would use regularly)
// 				report progress on each clean of each name
// 		sort resulting array by address
// 		remove dups if there are any
// 		output the header
// 		output the array of data
//
// Open issues:
//
// 1) Should names span chains?
// 2) How does ENS interplay?
// 3) Can we extend to query ENS for registered names for the given address?
//

// establishFolder(cacheFolder_names);
// if ((contains(item.tags, "Kickback") || contains(item.tags, "Humanity")))  // don't expose people during testing
// Last name in wins in case the name is customized and overlays an existing name -- update if
//      - the new name is not empty
//      - the new name is different or if the new name is a Prefund
//      - only update the name
