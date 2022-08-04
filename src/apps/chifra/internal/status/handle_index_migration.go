// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

// TODO: BOGUS - THIS WORK NEEDS WAY MORE TESTING AND IS INCOMPLETE
package statusPkg

import (
	"fmt"
	"log"
	"os"
	"path/filepath"
	"strings"

	initPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/init"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/migrate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

func (opts *StatusOptions) HandleIndexMigration() error {
	// TODO: BOGUS - MIGRATION SENTINAL? REENTRANCY SAFE?
	unused := ""
	if !migrate.HasBackLevelIndex(opts.Globals.Chain, &unused) {
		log.Println(colors.Yellow, "The index does not need to be migrated.", colors.Off)
		return nil
	}

	opts.checkTrueBlocksFile()
	opts.removeUnusedFolders()
	opts.removeTaintedData("v0.40.0-beta")
	opts.cleanMonitors("v0.40.0-beta")

	return nil
}

func (opts *StatusOptions) checkTrueBlocksFile() {
	log.Println(colors.Yellow + "Checking " + config.GetPathToRootConfig() + "trueBlocks.toml" + colors.Off)
	lines := file.AsciiFileToLines(config.GetPathToRootConfig() + "trueBlocks.toml")
	for i, line := range lines {
		if !strings.HasPrefix(line, "#") {
			// fmt.Fprintf(os.Stderr, "%s\r", colors.Yellow+"Checking "+line+colors.Off)
			if strings.Contains(line, "pinGateway") {
				log.Printf("%s    Error at line %d: %sChange 'pinGateway' to 'ipfsGateway'.", colors.Red, i+1, colors.Off)
				log.Fatal()
			}
			if strings.Contains(line, "unchainedindex.io/ipfs/") {
				isMainnet := strings.Contains(line, "ipfs.unchainedindex.io/ipfs/")
				isGnosis := strings.Contains(line, "gnosis.unchainedindex.io/ipfs/")
				isSepolia := strings.Contains(line, "sepolia.unchainedindex.io/ipfs/")
				if !isMainnet && !isGnosis && !isSepolia {
					log.Printf("%s    Error at line %d: %sUnsupported endpoint: %s", colors.Red, i+1, colors.Off, line)
					log.Fatal()
				}
			}
		}
	}
	done()
}

func (opts *StatusOptions) removeUnusedFolders() {
	log.Println(colors.Yellow + "Removing temporary folders (staging, unripe, ripe, maps)" + colors.Off)
	index.CleanTemporaryFolders(config.GetPathToIndex(opts.Globals.Chain), true)
	done()
}

func (opts *StatusOptions) removeTaintedData(vers string) error {
	isPartial := false

	globs := []string{}
	switch vers {
	case "v0.40.0-beta":
		globs = []string{"013*", "014*", "015*"}
	}

	for _, glob := range globs {
		files, err := filepath.Glob(filepath.Join(config.GetPathToIndex(opts.Globals.Chain), "blooms", glob))
		if err != nil {
			panic(err)
		}
		for _, ff := range files {
			fmt.Println("===>", ff)
			err := os.Remove(ff)
			if err != nil {
				log.Panic(err)
			}
			ff = index.ToIndexPath(ff)
			if file.FileExists(ff) {
				fmt.Println("===>", ff)
				err := os.Remove(ff)
				if err != nil {
					log.Panic(err)
				}
			} else {
				isPartial = true
			}
		}
	}

	initOpts := initPkg.InitOptions{
		Globals: opts.Globals,
	}
	if !isPartial {
		initOpts.All = true
	}
	err := initOpts.HandleInit()
	if err != nil {
		return err
	}

	done()
	return nil
}

func (opts *StatusOptions) cleanMonitors(vers string) error {
	log.Println("Remove records in any mainnet monitor file after block 13,000,000")
	return nil
}

func done() {
	log.Println(colors.Green + "Done." + colors.Off)
}
