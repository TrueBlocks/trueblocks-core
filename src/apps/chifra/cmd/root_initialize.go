// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cmd

import (
	"context"
	"fmt"
	"os"
	"os/user"
	"path/filepath"
	"runtime"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// Initialize makes sure everything is ready to run. These routines don't return if they aren't
func Initialize() bool {
	if os.Getenv("NO_COLOR") == "true" {
		colors.ColorsOff()
	}

	VerifyOs()
	VerifyMigrations()

	return true
}

// VerifyOs will panic if the operating system isn't cooperating
func VerifyOs() {
	userOs := runtime.GOOS
	if userOs == "windows" {
		logger.Fatal("Windows is not supported\n")
	}

	if userOs != "linux" && userOs != "darwin" {
		logger.Fatal("Unsupported operating system: ", userOs, "\n")
	}

	_, err := user.Current()
	if err != nil {
		logger.Fatal("Could not read user home directory\n")
	}
}

// VerifyMigrations will panic if the installation is not properly migrated
func VerifyMigrations() {
	if isStatusOrConfig() {
		// Allow certain status and config routes to pass so as to aide user in migrating...
		return
	}

	user, _ := user.Current()

	const doesNotExist string = `0002. A config item ({0}) is missing. See {https://trueblocks.io/docs/install/install-core/}.`
	const shouldNotExist string = `0002. A config item ({0}) exists but should not. See {https://trueblocks.io/docs/install/install-core/}.`

	// The old $HOME/.quickBlocks folder should not exist...
	if _, err := os.Stat(filepath.Join(user.HomeDir, ".quickBlocks")); err == nil {
		msg := strings.Replace(shouldNotExist, "{0}", "{~/.quickBlocks}", -1)
		msg = colors.ColoredWith(msg, colors.Yellow)
		logger.Fatal(msg)
	}

	// Both the config folder...
	configFolder := config.GetPathToRootConfig()
	if _, err := os.Stat(configFolder); err != nil {
		msg := strings.Replace(doesNotExist, "{0}", "{"+configFolder+"}", -1)
		msg = colors.ColoredWith(msg, colors.Yellow)
		logger.Fatal(msg)
	}

	// ...and the config file better exist.
	configFile := filepath.Join(configFolder + "trueBlocks.toml")
	if _, err := os.Stat(configFile); err != nil {
		msg := strings.Replace(doesNotExist, "{0}", "{"+configFile+"}", -1)
		msg = colors.ColoredWith(msg, colors.Yellow)
		logger.Fatal(msg)
	}

	// ...and some chains...
	chainMap, _ := config.GetChainLists()
	if len(chainMap) == 0 {
		const noChains string = `0003. The configuration file ({0}) contains no chain specifications. See {https://trueblocks.io/docs/install/install-core/}.`
		msg := strings.Replace(noChains, "{0}", "{"+configFile+"}", -1)
		msg = colors.ColoredWith(msg, colors.Yellow)
		logger.Fatal(msg)
	}

	// We need to find the chain configuration path
	chainConfigPath := config.MustGetPathToChainConfig("")
	if _, err := os.Stat(chainConfigPath); err != nil {
		msg := strings.Replace(doesNotExist, "{0}", "{"+chainConfigPath+"}", -1)
		msg = colors.ColoredWith(msg, colors.Yellow)
		logger.Fatal(msg)
	}

	// Make sure they've completed migrations prior to v1.0.0
	items := []string{
		"blocks",
		"objs",
		"recons",
		"slurps",
		"traces",
		"txs",
	}
	for _, item := range items {
		itemPath := filepath.Join(config.GetPathToCache(""), item)
		if _, err := os.Stat(itemPath); err == nil {
			msg := strings.Replace(shouldNotExist, "{0}", "{"+itemPath+"}", -1)
			msg = colors.ColoredWith(msg, colors.Yellow)
			logger.Fatal(msg)
		}
	}

	requiredVer := version.NewVersion("v1.0.0-release")
	currentVer := version.NewVersion(config.GetRootConfig().Version.Current)
	if currentVer.Uint64() < requiredVer.Uint64() {
		upgradeConfigs(requiredVer)
	}
}

func isStatusOrConfig() bool {
	isStatus := false
	isConfig := false
	hasPaths := false
	hasEdit := false
	cnt := len(os.Args)
	for _, arg := range os.Args {
		if arg == "help" {
			return false
		} else if arg == "status" {
			isStatus = true
		} else if arg == "config" {
			isConfig = true
		} else if arg == "--paths" {
			hasPaths = true
		} else if arg == "edit" {
			hasEdit = true
		} else if arg != "--verbose" {
			isStatus = false
			isConfig = false
		} else {
			cnt-- // allow --verbose
		}
	}

	if isStatus && cnt == 2 {
		return true
	}

	return isConfig && (hasPaths || hasEdit) && cnt < 4
}

// upgradeConfigs will upgrade the config files to the latest versions
func upgradeConfigs(newVersion version.Version) {
	scraperConfigs := findConfigFiles()

	fn := config.GetPathToRootConfig() + "trueBlocks.toml"
	contents := file.AsciiFileToString(fn)
	lines := strings.Split(contents, "\n")
	linesOut := make([]string, 0, len(lines))

	hasScrapeSection := false
	for _, line := range lines {
		if strings.Contains(line, "[scrape]") {
			hasScrapeSection = true
		}
		if strings.HasPrefix(line, "current") {
			linesOut = append(linesOut, "current = \""+newVersion.String()+"\"")
		} else if !strings.Contains(line, "apiProvider") {
			linesOut = append(linesOut, line)
		}
	}

	if !hasScrapeSection && len(scraperConfigs) > 0 {
		linesOut = append(linesOut, "[scrape]")
	}

	for _, f := range scraperConfigs {
		if strings.Contains(f.Path, "ethslurp.toml") {
			os.Remove(f.Path)
			continue
		}

		contents := file.AsciiFileToString(f.Path)
		theseLines := strings.Split(contents, "\n")
		for i, line := range theseLines {
			if strings.Contains(line, "[settings]") {
				parts := strings.Split(f.Path, "/")
				chain := parts[len(parts)-2]
				theseLines[i] = "[scrape." + chain + "]"
			} else {
				theseLines[i] = strings.Replace(theseLines[i], "apps_per_chunk", "appsPerChunk", -1)
				theseLines[i] = strings.Replace(theseLines[i], "snap_to_grid", "snapToGrid", -1)
				theseLines[i] = strings.Replace(theseLines[i], "first_snap", "firstSnap", -1)
				theseLines[i] = strings.Replace(theseLines[i], "unripe_dist", "unripeDist", -1)
				theseLines[i] = strings.Replace(theseLines[i], "channel_count", "channelCount", -1)
				theseLines[i] = strings.Replace(theseLines[i], "allow_missing", "allowMissing", -1)
			}
		}
		linesOut = append(linesOut, "")
		linesOut = append(linesOut, theseLines...)
		os.Remove(f.Path)
	}

	_ = file.LinesToAsciiFile(fn, linesOut)
	logger.Fatal(fmt.Sprintf("Your configuration files were upgraded to %s. Rerun your command.", newVersion.String()))
}

func findConfigFiles() []walk.CacheFileInfo {
	filenameChan := make(chan walk.CacheFileInfo)
	var nRoutines = 1
	go walk.WalkConfigFolders(context.Background(), nil, filenameChan)

	files := make([]walk.CacheFileInfo, 0, 10)
	for result := range filenameChan {
		switch result.Type {
		case walk.Config:
			if strings.Contains(result.Path, "ethslurp.toml") || strings.Contains(result.Path, "blockScrape.toml") {
				files = append(files, result)
			}
		case walk.Cache_NotACache:
			nRoutines--
			if nRoutines == 0 {
				close(filenameChan)
			}
		default:
			logger.Fatal("should not happen in upgradeConfigs")
		}
	}
	return files
}
