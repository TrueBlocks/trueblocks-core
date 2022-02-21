// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cmd

import (
	"fmt"
	"os"
	"strings"

	"github.com/spf13/cobra"

	"github.com/spf13/viper"
)

// blazeCmd represents the base command when called without any subcommands
var blazeCmd = &cobra.Command{
	Use:   "blaze",
	Short: "Build, query, or share an index of Ethereum addresses per block",
	Long: `
Description:
  Blaze is an internal-use-only component called by 'blockScrape' to 
  index blocks from the last visited block (startBlock) to the front
  of the chain (or for block_cnt if specified). It accumulates an index
  of every address as it appears anywhere in the data. You may
  later query this index (with acctExport) many orders of magnitude
  more quickly than scanning the blockchain directly. All of the
  component's numeric command line options are required.`,
	Version: "GHC-TrueBlocks, LLC//0.8.1-alpha",
}

// Execute adds all child commands to the root command and sets flags appropriately.
// This is called by main(). It only needs to happen once for the blazeCmd.
func Execute() {
	err := blazeCmd.Execute()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}

// OptionsType Structure to carry command line and config file options
type OptionsType struct {
	rpcProvider    string
	indexPath      string
	ripePath       string
	unripePath     string
	startBlock     int
	block_cnt      int
	block_chan_cnt int
	addr_chan_cnt  int
	ripeBlock      int
	verbose        int
}

// Options Carries the configuration options (from both command line and config file)
var Options OptionsType

// init Initalize options
func init() {
	cobra.OnInitialize(initConfig)


	// BOGUS - why do we have these command line options?
	var unused1, unused2 string
	blazeCmd.PersistentFlags().StringVarP(&unused1, "rpcProvider", "r", "http://localhost:8545", "URL to the node's RPC")
	blazeCmd.PersistentFlags().StringVarP(&unused2, "indexPath", "c", "", "The location of TrueBlocks' appearance cache (default \"$CONFIG/unchained\")")
	blazeCmd.PersistentFlags().IntVarP(&Options.startBlock, "startBlock", "s", 0, "First block to visit (required)")
	blazeCmd.PersistentFlags().IntVarP(&Options.block_cnt, "block_cnt", "n", 0, "The number of blocks to scrape (required)")
	blazeCmd.PersistentFlags().IntVarP(&Options.block_chan_cnt, "block_chan_cnt", "b", 20, "The number of block processors to create (required)")
	blazeCmd.PersistentFlags().IntVarP(&Options.addr_chan_cnt, "addr_chan_cnt", "a", 60, "The number of address processors to create (required)")
	blazeCmd.PersistentFlags().IntVarP(&Options.ripeBlock, "ripeBlock", "e", 0, "Blocks prior to this value are written to 'ripe' folder (required)")
	blazeCmd.PersistentFlags().IntVarP(&Options.verbose, "verbose", "v", 0, "Display more or less information")
	blazeCmd.MarkPersistentFlagRequired("startBlock")
	blazeCmd.MarkPersistentFlagRequired("block_cnt")
	blazeCmd.MarkPersistentFlagRequired("ripeBlock")

	// BOGUS - what if we're called with some BOGUS data
	envs := strings.Split(os.Getenv("TB_CONFIG_ENV"), ",")
	Options.indexPath = envs[4]
	Options.rpcProvider = envs[6]

	Options.ripePath = Options.indexPath + "ripe/"
	if _, err := os.Stat(Options.ripePath); os.IsNotExist(err) {
		os.MkdirAll(Options.ripePath, 0777)
	}

	Options.unripePath = Options.indexPath + "unripe/"
	if _, err := os.Stat(Options.unripePath); os.IsNotExist(err) {
		os.MkdirAll(Options.unripePath, 0777)
	}

	if Options.verbose > 4 {
		fmt.Println("blaze.rpcProvider: ", Options.rpcProvider)
		fmt.Println("blaze.indexPath:   ", Options.indexPath)
		fmt.Println("blaze.ripePath:    ", Options.ripePath)
		fmt.Println("blaze.unripePath:  ", Options.unripePath)
		fmt.Println("blaze.startBlock:     ", Options.startBlock)
		fmt.Println("blaze.block_cnt:      ", Options.block_cnt)
		fmt.Println("blaze.block_chan_cnt: ", Options.block_chan_cnt)
		fmt.Println("blaze.addr_chan_cnt:  ", Options.addr_chan_cnt)
		fmt.Println("blaze.ripeBlock:      ", Options.ripeBlock)
		fmt.Println("blaze.verbose:        ", Options.verbose)
	}
}

// initConfig reads in config file and ENV variables if set.
func initConfig() {
    // BOGUS - is this okay? Does config work with env variables?
	envs := strings.Split(os.Getenv("TB_CONFIG_ENV"), ",")
	configPath := envs[1]

	// TODO: The caller should have set up all configuration, so this is probably not needed.
	viper.AddConfigPath(configPath)
	viper.SetConfigName("trueBlocks")
	viper.SetConfigType("toml")
	err := viper.ReadInConfig()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	viper.SetEnvPrefix("TB")
	viper.SetEnvKeyReplacer(strings.NewReplacer("SETTINGS.", ""))
	viper.AutomaticEnv()
}
