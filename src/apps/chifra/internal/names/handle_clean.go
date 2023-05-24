// New tests -- chifra names --autoname (address in the environment)
// New tests -- chifra names --autoname <address>
package namesPkg

import (
	"context"
	"fmt"
	"math"
	"strings"
	"sync"
	"sync/atomic"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/contract"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/prefunds"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/token"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *NamesOptions) HandleClean() error {
	label := "custom"
	db := names.DatabaseCustom
	if opts.Regular {
		label = "regular"
		db = names.DatabaseRegular
	}
	logger.Info("Processing", label, "names file", "("+names.GetDatabasePath(opts.Globals.Chain, db)+")")

	err := opts.cleanNames()
	if err != nil {
		logger.Warn("The", label, "names database was not cleaned")
	} else {
		logger.Info("The", label, "names database was cleaned")
	}
	return err
}

func (opts *NamesOptions) cleanNames() error {
	parts := names.Custom
	if opts.Regular {
		parts = names.Regular
	}

	// Load databases
	allNames, err := names.LoadNamesMap(opts.Globals.Chain, parts, []string{})
	if err != nil {
		return err
	}
	prefundMap, err := preparePrefunds(opts.Globals.Chain)
	if err != nil {
		return err
	}

	// Prepare progress reporting. We will report percentage.
	total := len(allNames)
	var done atomic.Int32
	progressChan := make(chan int)
	defer close(progressChan)
	// Listen on a channel and whenever it updates, call `reportProgress`
	go func() {
		for progress := range progressChan {
			doneNow := done.Add(int32(progress))
			reportProgress(doneNow, total)
		}
	}()

	// If nothing gets modified we won't bother with saving the files
	var anyNameModified bool
	// We'll use once to set `anyNameModified` from goroutines
	var onceMod sync.Once

	// For --dry_run, we don't want to write to the real database
	var overrideDatabase names.Database
	if opts.DryRun {
		overrideDatabase = names.DatabaseDryRun
	}

	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()
	errorChannel := make(chan error)
	go utils.IterateOverMap(ctx, errorChannel, allNames, func(address base.Address, name types.SimpleName) error {
		modified, err := cleanName(opts.Globals.Chain, &name)
		if err != nil {
			return wrapErrorWithAddr(&address, err)
		}
		if isPrefund := prefundMap[name.Address]; isPrefund != name.IsPrefund {
			name.IsPrefund = isPrefund
			modified = true
		}

		progressChan <- 1

		if !modified {
			return nil
		}

		// The name has been modified, so set the flag and...
		onceMod.Do(func() { anyNameModified = true })

		// ...update names in-memory cache
		if opts.Regular {
			if err = names.UpdateRegularName(&name); err != nil {
				return wrapErrorWithAddr(&address, err)
			}
		} else {
			if err = names.UpdateCustomName(&name); err != nil {
				return wrapErrorWithAddr(&address, err)
			}
		}
		return nil
	})

	// Block until we get an error from any of the iterations or `IterateOverMap` finishes
	if stepErr := <-errorChannel; stepErr != nil {
		cancel()
		return stepErr
	}

	// If nothing has been changed, we can exit here
	if !anyNameModified {
		return nil
	}

	// Write to disk
	if opts.Regular {
		return names.WriteRegularNames(opts.Globals.Chain, overrideDatabase)
	}

	return names.WriteCustomNames(opts.Globals.Chain, overrideDatabase)
}

func reportProgress(done int32, total int) {
	if done%10 != 0 {
		return
	}

	percentage := math.Round(float64(done) / float64(total) * 100)
	logger.Progress(
		true,
		fmt.Sprintf("Cleaning: %.f%% (%d items, %d total)", percentage, done, total),
	)
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

func cleanName(chain string, name *types.SimpleName) (modified bool, err error) {
	isContract, err := contract.IsContractAt(chain, name.Address, nil)
	if err != nil {
		return
	}
	wasContract := name.IsContract && !isContract
	modified = cleanCommon(name)

	if !isContract {
		if mod := cleanNonContract(name, wasContract); mod {
			modified = true
		}
		return
	}

	tokenState, err := token.GetState(chain, name.Address, "latest")
	if _, ok := err.(token.ErrNodeConnection); ok {
		return
	}
	// It's not a token
	if err != nil {
		err = nil
	}

	contractModified, err := cleanContract(tokenState, name.Address, name)
	if err != nil {
		return
	}
	modified = modified || contractModified
	return
}

func cleanCommon(name *types.SimpleName) (modified bool) {
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

	if len(name.Petname) == 0 {
		name.Petname = names.AddrToPetname(name.Address.Hex(), "-")
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

func cleanContract(token *token.Token, address base.Address, name *types.SimpleName) (modified bool, err error) {
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

func cleanToken(name *types.SimpleName, token *token.Token) (modified bool) {
	if !name.IsErc20 && token.IsErc20() {
		name.IsErc20 = true
		modified = true
	}

	airdrop := strings.Contains(name.Name, "airdrop")
	if name.Tags == "60-Airdrops" {
		name.Tags = ""
		modified = true
	}

	if token.IsErc20() && (name.Tags == "" ||
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

	if token.IsErc721() && !name.IsErc721 {
		name.IsErc721 = true
		modified = true
	}

	if !token.IsErc721() && name.IsErc721 {
		name.IsErc721 = false
		modified = true
	}

	if token.IsErc721() && name.IsErc721 && name.Tags == "" {
		name.Tags = "50-Tokens:ERC721"
		modified = true
	}

	return
}

func cleanNonContract(name *types.SimpleName, wasContract bool) (modified bool) {
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
//		if tag is >= 8 (as a string), return without modification noting that tags over '8' character are reserved
//		latestBlock = testMode ? 10800000 : getLatestBlock_client()
//
// Source:
//		if contains (ignore case) 'etherscan' then the entire string becomes Etherscan.io
//		if contains (ignore case) 'trueblocks' then the entire string becomes TrueBlocks.io
//		change any white space to spaces, change double spaces to single spaces
//
// Petname:
//		based on address assign it without asking (never user assigned)
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
//     string_q val = getTokenState(addr, "supportsInterface", abi_spec, latest, erc721QueryBytes);
//     return val == "T" || val == "true";
// }

//     bool isAirdrop = containsI(account.name, "airdrop");
//     if (account.tags == "60-Airdrops")
//         account.tags = "";

//     if (!isContract) {
//         bool isEmpty = account.tags.empty();
//         bool isContract = contains(account.tags, "Contracts");
//         bool isToken = contains(account.tags, "Tokens");
//         account.tags = !isEmpty && !isContract && !isToken ? account.tags : "90-Individuals:Other";
//         if (wasContract) {
//             // This used to be a contract and now is not, so it must be a self destruct
//             account.isContract = true;
//             account.tags = "37-SelfDestructed";
//         }

//     } else {
//         // This is a contract...
//         account.isContract = true;

//         string_q name = getTokenState(account.address, "name", opts->abi_spec, latestBlock);
//         string_q symbol = getTokenState(account.address, "symbol", opts->abi_spec, latestBlock);
//         uint64_t decimals = str_2_Uint(getTokenState(account.address, "decimals", opts->abi_spec, latestBlock));
//         if (!name.empty() || !symbol.empty() || decimals > 0) {
//             account.isErc20 = true;
//             account.source =
//                 (account.source.empty() || account.source == "TrueBlocks.io" || account.source == "EtherScan.io")
//                     ? "On chain"
//                     : account.source;
//             // Use the values from on-chain if we can...
//             account.name = (!name.empty() ? name : account.name);
//             account.symbol = (!symbol.empty() ? symbol : account.symbol);
//             account.decimals = decimals ? decimals : (account.decimals ? account.decimals : 18);
//             account.isErc721 = isErc721(account.address, opts->abi_spec, latestBlock);
//             if (account.isErc721) {
//                 account.tags = "50-Tokens:ERC721";

//             } else {
//                 // This is an ERC20, so if we've not tagged it specifically, make it thus
//                 if (account.tags.empty() || containsI(account.tags, "token") ||
//                     containsI(account.tags, "30-contracts") || containsI(account.tags, "55-defi") || isAirdrop) {
//                     account.tags = "50-Tokens:ERC20";
//                 }
//             }

//         } else {
//             account.isErc20 = false;
//             account.isErc721 = false;
//         }
//         if (account.tags.empty())
//             account.tags = "30-Contracts";
//     }

//     if (isAirdrop && !containsI(account.name, "Airdrop")) {
//         replaceAll(account.name, " airdrop", "");
//         replaceAll(account.name, " Airdrop", "");
//         account.name = account.name + " Airdrop";
//     }

//     // Clean up name and symbol
//     account.name = trim(substitute(account.name, "  ", " "));
//     account.symbol = trim(substitute(account.symbol, "  ", " "));

//     return !account.name.empty();
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
