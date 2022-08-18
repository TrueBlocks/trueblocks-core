/*
31900,apps,Admin,chunks,chunkMan,mode,,,true,false,true,true,local,positional,enum[status|manifest|index|blooms|addresses|appearances|stats],the type of data to process
31905,apps,Admin,chunks,chunkMan,blocks,,,false,false,true,true,header,positional,list<blknum>,an optional list of blocks to intersect with chunk ranges
31910,apps,Admin,chunks,chunkMan,addrs,,,false,false,true,true,header,positional,list<addr>,an optional list of addresses to use with the --belongs option
31920,apps,Admin,chunks,chunkMan,check,c,,false,false,true,false,gocmd,switch,<boolean>,check the manifest&#44; index&#44; or blooms for internal consistency
31925,apps,Admin,chunks,chunkMan,truncate,n,,false,false,true,false,gocmd,flag,<blknum>,truncate the entire index at this block (requires a block identifier)
31930,apps,Admin,chunks,chunkMan,repair,r,,false,false,true,false,gocmd,switch,<boolean>,repair the manifest or a single index chunk (index repair requires a block identifier)
31935,apps,Admin,chunks,chunkMan,pin,i,,false,false,true,false,gocmd,switch,<boolean>,pin the manifest or each index chunk and bloom
31940,apps,Admin,chunks,chunkMan,publish,p,,false,false,true,false,gocmd,switch,<boolean>,publish the manifest to the Unchained Index smart contract
31945,apps,Admin,chunks,chunkMan,remote,m,,false,false,true,false,gocmd,switch,<boolean>,prior to processing&#44; retreive the manifest from the Unchained Index smart contract
31950,apps,Admin,chunks,chunkMan,belongs,b,,false,false,true,false,gocmd,switch,<boolean>,in index mode only&#44; checks if the given address appears in the given index chunk
31955,apps,Admin,chunks,chunkMan,,,,false,false,true,true,--,description,,Manage&#44; investigate&#44; and display the Unchained Index.
31960,apps,Admin,chunks,chunkMan,n1,,,false,false,false,false,--,note,,Mode determines which type of data to display or process.
31965,apps,Admin,chunks,chunkMan,n2,,,false,false,false,false,--,note,,Certain options are only available in certain modes.
31970,apps,Admin,chunks,chunkMan,n3,,,false,false,false,false,--,note,,If blocks are provided&#44; only chunks intersecting with those blocks are displayed.
31980,apps,Admin,chunks,chunkMan,n5,,,false,false,false,false,--,note,,The --repair and --truncate options update data&#44; but do not --pin or --publish.
31985,apps,Admin,chunks,chunkMan,n6,,,false,false,false,false,--,note,,You may combine the --pin and --publish options.
31990,apps,Admin,chunks,chunkMan,n7,,,false,false,false,false,--,note,,The --belongs option is only available in the index mode.

if publish then manifest

if pin then manifest or index

pin all chunks (locally if IPFS daemon is running&#44; and/or remotely with --remote flag)

if truncate then index and requires block

if belongs then address
if address then belongs

if check then manifest (check all), index or blooms (check either)

if repair then either manifest or index, if index then requires a block
*/

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/migrate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrape"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ChunksOptions) validateChunks() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if len(opts.Mode) == 0 {
		return validate.Usage("Please choose at least one of {0}.", "[status|manifest|index|blooms|addresses|appearances|stats]")
	}

	err := validate.ValidateEnum("mode", opts.Mode, "[status|manifest|index|blooms|addresses|appearances|stats]")
	if err != nil {
		return err
	}

	if opts.Mode == "addresses" && opts.Globals.LogLevel > 0 && opts.Globals.Format == "json" {
		return validate.Usage("You may not use --format json and log_level > 0 in addresses mode")
	}

	if opts.Publish {
		return validate.Usage("The {0} option is not yet enabled", "--publish")
	}

	if opts.Mode != "index" && opts.Mode != "manifest" {
		if opts.Pin {
			return validate.Usage("The {0} option is available only in {1} mode.", "--pin", "index or manifest")
		}
		if opts.Publish {
			return validate.Usage("The {0} option is available only in {1} mode.", "--publish", "index or manifest")
		}
		if opts.Remote {
			return validate.Usage("The {0} option is available only in {1} mode.", "--remote", "index or manifest")
		}
		if opts.Check {
			return validate.Usage("The {0} option is available only in {1} mode.", "--check", "index or manifest")
		}
		if opts.Repair {
			return validate.Usage("The {0} option is only available in {1} mode", "--repair", "index or manifest")
		}
	}

	if opts.Repair {
		if opts.Globals.TestMode {
			return validate.Usage("The --repair option is not available in test mode")
		}

		if opts.Mode == "manifest" {
			if opts.Globals.Format != "json" {
				return validate.Usage("The {0} requires the {1} option", "manifest --repair", "--fmt json")
			}
		}
	}

	if opts.Pin {
		if opts.Remote {
			pinataKey, pinataSecret, estuaryKey := scrape.PinningKeys(opts.Globals.Chain)
			if (pinataKey == "" || pinataSecret == "") && estuaryKey == "" {
				return validate.Usage("The {0} option requires {1}", "--pin --remote", "an api key")
			}

		} else if !pinning.LocalDaemonRunning() {
			return validate.Usage("The {0} option requires {1}", "--pin", "a locally running IPFS daemon or --remote")

		}

		if opts.Mode != "index" {
			return validate.Usage("The {0} option is available only in {1} mode.", "--pin", "index")
		}
	}

	if len(opts.Belongs) > 0 {
		if opts.Belongs[0] == "" {
			return validate.Usage("The --belongs option requires an address.")
		}
		if opts.Mode != "index" {
			return validate.Usage("The {0} option requires {1}", "--belongs", "the --index option")
		}
		if len(opts.Blocks) == 0 {
			return validate.Usage("You must specifiy at least one {0} with the {1} option", "block identifier", "--belongs")
		}
		if opts.Globals.Verbose {
			return validate.Usage("Choose either {0} or {1}, not both.", "--verbose", "--belongs")
		}
	}

	if opts.Mode != "index" && opts.Truncate != utils.NOPOS {
		return validate.Usage("The {0} option is only available {1}.", "--truncate", "in index mode")
	}

	if !opts.Globals.Verbose && opts.Globals.ToFile {
		return validate.Usage("You may not use the {0} option without {1}.", "--to_file", "--verbose")
	}

	err = validate.ValidateIdentifiers(
		opts.Globals.Chain,
		opts.Blocks,
		validate.ValidBlockIdWithRangeAndDate,
		1,
		&opts.BlockIds,
	)
	if err != nil {
		if invalidLiteral, ok := err.(*validate.InvalidIdentifierLiteralError); ok {
			return invalidLiteral
		}
		if errors.Is(err, validate.ErrTooManyRanges) {
			return validate.Usage("Specify only a single block range at a time.")
		}
		return err
	}

	// Note this does not return if a migration is needed
	migrate.CheckBackLevelIndex(opts.Globals.Chain, true)

	return opts.Globals.Validate()
}
