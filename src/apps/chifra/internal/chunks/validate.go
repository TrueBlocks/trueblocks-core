// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"errors"
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

func (opts *ChunksOptions) validateChunks() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.Globals.IsApiMode() {
		if len(opts.Tag) > 0 {
			return validate.Usage("The {0} option is not available{1}.", "--tag", " in api mode")
		}
		if opts.Truncate != base.NOPOSN {
			return validate.Usage("The {0} option is not available{1}.", "--truncate", " in api mode")
		}
		if opts.Mode == "pins" {
			return validate.Usage("The {0} mode is not available{1}.", "pins", " in api mode")
		}
	} else if len(opts.Tag) > 0 {
		if !version.IsValidVersion(opts.Tag) {
			return validate.Usage("The {0} ({1}) must be a valid version string.", "--tag", opts.Tag)
		}
		if !config.KnownVersionTag(opts.Tag) {
			return validate.Usage("The only valid value for {0} is {1}.", "--tag", "trueblocks-core@v2.0.0-release")
		}
	}

	if opts.Mode == "pins" {
		if !opts.List && !opts.Unpin && !opts.Count {
			return validate.Usage("{0} mode requires {1}.", "pins", "either --list, --count, or --unpin")
		}

		if opts.Unpin {
			if !file.FileExists("./unpins") {
				return validate.Usage("The file {0} was not found in the local folder.", "./unpins")
			}

			hasOne := false
			lines := file.AsciiFileToLines("./unpins")
			for _, line := range lines {
				if pinning.IsValid(line) {
					hasOne = true
					break
				}
			}
			if !hasOne {
				return validate.Usage("The {0} file does not contain any valid CIDs.", "./unpins")
			}
		}

	} else {
		if opts.List {
			return validate.Usage("The {0} option is only available in {1} mode.", "--list", "pins")
		} else if opts.Unpin {
			return validate.Usage("The {0} option is only available in {1} mode.", "--unpin", "pins")
		}
	}

	if opts.Count {
		if !strings.Contains("manifest|index|blooms|pins|stats", opts.Mode) {
			return validate.Usage("The {0} option is only available in {1}.", "--count", "these modes: manifest|index|blooms|pins|stats")
		}
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	err := validate.ValidateEnumRequired("mode", opts.Mode, "[manifest|index|blooms|pins|addresses|appearances|stats]")
	if err != nil {
		return err
	}

	if opts.Diff {
		if opts.Mode != "index" {
			return validate.Usage("The {0} option is only available in {1} mode.", "--diff", "index")
		}
		path := os.Getenv("TB_CHUNKS_DIFFPATH")
		if path == "" {
			return validate.Usage("The {0} option requires {1}.", "--diff", "TB_CHUNKS_DIFFPATH to be set")
		}
		if !file.FolderExists(path) {
			return fmt.Errorf("the path TB_CHUNKS_DIFFPATH=%s does not exist", path)
		}
	}

	isIndexOrManifest := opts.Mode == "index" || opts.Mode == "manifest"
	if !isIndexOrManifest {
		if err = opts.isDisallowed(!isIndexOrManifest /* i.e., true */, opts.Mode); err != nil {
			return err
		}

		if opts.Check {
			return validate.Usage("The {0} option is not available{1}.", "--check", " in "+opts.Mode+" mode")
		}
	}

	isPin := opts.Pin
	isCheck := opts.Check
	isPublish := opts.Publish
	isRemote := opts.Remote
	isRewrite := opts.Rewrite
	isDeep := opts.Deep

	if !isIndexOrManifest && (isPin || isPublish || isRemote || isDeep || isCheck) {
		return validate.Usage("The {0} options is only available in {1} or {2} mode.", "--pin, --publish, --remote, --check, and --deep", "manifest", "index")
	}

	if opts.Mode == "manifest" && opts.Remote {
		// this is okay
	} else if !isCheck && !isPin && (isRemote || isDeep) {
		return validate.Usage("The {0} options require {1}.", "--remote and --deep", "--pin or --check")
	}

	if isPin {
		if isRemote {
			apiKey, secret, jwt := config.GetKey("pinata").ApiKey, config.GetKey("pinata").Secret, config.GetKey("pinata").Jwt
			if secret == "" && jwt == "" {
				return validate.Usage("Either the {0} key or the {1} is required.", "secret", "jwt")
			}
			if secret != "" && apiKey == "" {
				return validate.Usage("If the {0} key is present, so must be the {1}.", "secret", "apiKey")
			}
			if config.GetPinning().RemotePinUrl == "" {
				return validate.Usage("The {0} option requires {1}.", "--pin --remote", "a remotePinUrl")
			}
		} else {
			if !config.IpfsRunning() {
				return validate.Usage("The {0} option requires {1}.", "--pin", "a locally running IPFS daemon")
			}
			if config.GetPinning().LocalPinUrl == "" {
				return validate.Usage("The {0} option requires {1}.", "--pin", "a localPinUrl")
			}
		}
	} else if isRewrite {
		return validate.Usage("The {0} option requires {1}.", "--rewrite", "--pin")
	}

	if opts.Publish {
		return validate.Usage("The {0} option is currenlty unavailable.", "--publish")
	}

	if opts.Mode != "index" {
		if len(opts.Tag) > 0 {
			return validate.Usage("The {0} option is only available {1}.", "--tag", "in index mode")
		}
		if opts.Truncate != base.NOPOSN {
			return validate.Usage("The {0} option is only available {1}.", "--truncate", "in index mode")
		}
		if len(opts.Belongs) > 0 {
			return validate.Usage("The {0} option requires {1}.", "--belongs", "the index mode")
		}

	} else {
		if len(opts.Belongs) > 0 {
			err := validate.ValidateAtLeastOneAddr(opts.Belongs)
			if err != nil {
				return err
			}
			if opts.Globals.Verbose {
				return validate.Usage("Choose either {0} or {1}, not both.", "--verbose", "--belongs")
			}
			if len(opts.Blocks) == 0 {
				return validate.Usage("You must specify at least one {0} with the {1} option", "block identifier", "--belongs")
			}
			if opts.Globals.Format != "json" {
				return validate.Usage("The {0} option only works with {1}", "--belongs", "--fmt json")
			}
		}
	}

	if err = opts.isDisallowed(opts.Globals.IsApiMode(), "API"); err != nil {
		return err
	}

	if err = opts.isDisallowed(opts.Globals.TestMode, "test"); err != nil {
		return err
	}

	if len(opts.Publisher) > 0 {
		err := validate.ValidateExactlyOneAddr([]string{opts.Publisher})
		if err != nil {
			return err
		}
	}

	err = validate.ValidateIdentifiers(
		chain,
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

	if opts.Diff && len(opts.BlockIds) != 1 {
		return validate.Usage("The {0} option requires exactly one block identifier.", "--diff")
	}

	if opts.FirstBlock != 0 || opts.LastBlock != base.NOPOSN || opts.MaxAddrs != base.NOPOS {
		if opts.FirstBlock >= opts.LastBlock {
			msg := fmt.Sprintf("first_block (%d) must be strictly earlier than last_block (%d).", opts.FirstBlock, opts.LastBlock)
			return validate.Usage(msg)
		}
		if len(opts.Belongs) == 0 && opts.Mode != "addresses" && opts.Mode != "appearances" {
			return validate.Usage("some options are only available with {0}.", "the addresses, the appearances, or the index --belongs modes")
		}
		// TODO: We should check that the first and last blocks are inside the ranges implied by the block ids
		// if len(opts.BlockIds) > 0 {
		// }
	}

	if err := index.IsInitialized(chain, config.ExpectedVersion()); err != nil {
		isTag := len(opts.Tag) != 0
		isRemoteMan := opts.Mode == "manifest" && opts.Remote
		if !isTag && !isRemoteMan {
			if (errors.Is(err, index.ErrNotInitialized) || errors.Is(err, index.ErrIncorrectHash)) &&
				!opts.Globals.IsApiMode() {
				logger.Fatal(err)
			} else {
				return err
			}
		}
		// It's okay to mismatch versions if we're tagging or downloading a new manifest
	}

	return opts.Globals.Validate()
}

func (opts *ChunksOptions) isDisallowed(test bool, mode string) error {
	if test {
		if opts.Pin {
			return validate.Usage("The {0} option is not available{1}.", "--pin", " in "+mode+" mode")
		}
		if opts.Publish {
			return validate.Usage("The {0} option is not available{1}.", "--publish", " in "+mode+" mode")
		}
		if opts.Remote {
			return validate.Usage("The {0} option is not available{1}.", "--remote", " in "+mode+" mode")
		}
		if opts.Truncate != base.NOPOSN {
			return validate.Usage("The {0} option is not available{1}.", "--truncate", " in "+mode+" mode")
		}
	}
	return nil
}
