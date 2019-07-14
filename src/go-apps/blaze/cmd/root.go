package cmd

import (
	"fmt"
	"os"
	"strings"

	"github.com/spf13/cobra"

	homedir "github.com/mitchellh/go-homedir"
	"github.com/spf13/viper"
)

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:   "blaze",
	Short: "Build, query, or share an index of Ethereum addresses per block",
	Long: `
Description:
  Blaze is an internal-use-only component called by blockScrape to 
  index blocks from the last visited block (startBlock) to the front
  of the chain (or for nBlocks if specified). It accumulates an index
  of every addresses as they appears anywhere on the chain. You may
  later query this index (with acctScrape) many orders of magnitude
  more quickly than scanning the blockchain directly. The component
  has four required command line options.`,
	Version: "GHC-TrueBlocks, LLC//0.0.1-alpha",
}

// Execute adds all child commands to the root command and sets flags appropriately.
// This is called by main.main(). It only needs to happen once to the rootCmd.
func Execute() {
	err := rootCmd.Execute()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}

// options_t Structure to carry command line and config file options
type optionsT struct {
	rpcProvider string
	cachePath   string
	startBlock  int
	nBlocks     int
	nBlockProcs int
	nAddrProcs  int
	dockerMode  bool
}

// Options Carries the configuration options (from both command line and config file)
var Options optionsT

// init Initalize options
func init() {
	cobra.OnInitialize(initConfig)

	rootCmd.PersistentFlags().StringVarP(&Options.rpcProvider, "rpcProvider", "r", "http://localhost:8545", "The location of the node's RPC")
	rootCmd.PersistentFlags().StringVarP(&Options.cachePath, "cachePath", "c", "", "The location of TrueBlocks' cache")

	rootCmd.PersistentFlags().IntVarP(&Options.startBlock, "startBlock", "s", 0, "First block to visit (required)")
	rootCmd.MarkPersistentFlagRequired("startBlock")
	rootCmd.PersistentFlags().IntVarP(&Options.nBlocks, "nBlocks", "n", 2500, "The number of blocks to scrape during this invocation (required)")
	rootCmd.MarkPersistentFlagRequired("nBlocks")
	rootCmd.PersistentFlags().IntVarP(&Options.nBlockProcs, "nBlockProcs", "b", 50, "The number of block processor go routines to create (required)")
	rootCmd.MarkPersistentFlagRequired("nBlockProcs")
	rootCmd.PersistentFlags().IntVarP(&Options.nAddrProcs, "nAddrProcs", "a", 100, "The number of address processor go routines to create (required)")
	rootCmd.MarkPersistentFlagRequired("nAddrProcs")

	Options.dockerMode = (os.Getenv("DOCKER_MODE") != "")
}

// initConfig reads in config file and ENV variables if set.
func initConfig() {
	// Find home directory.
	home, err := homedir.Dir()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	viper.AddConfigPath(home + "/.quickBlocks")
	viper.SetConfigName("quickBlocks")
	viper.SetConfigType("toml")
	err = viper.ReadInConfig()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	// read in environment variables that match
	viper.SetEnvPrefix("TB")
	viper.SetEnvKeyReplacer(strings.NewReplacer("SETTINGS.", ""))
	viper.AutomaticEnv()
	viper.BindPFlag("settings.rpcProvider", rootCmd.PersistentFlags().Lookup("rpcProvider"))
	viper.BindPFlag("settings.cachePath", rootCmd.PersistentFlags().Lookup("cachePath"))
	viper.BindEnv("rpcProvider")
	viper.BindEnv("cachePath")

	// Cleanup
	Options.rpcProvider = viper.GetString("settings.rpcProvider")
	if len(Options.rpcProvider) == 0 {
		fmt.Println("Your rpcProvider is empty. Quitting...")
		os.Exit(1)
	}
	if Options.rpcProvider[len(Options.rpcProvider)-1] != '/' {
		Options.rpcProvider += "/"
	}

	Options.cachePath = viper.GetString("settings.cachePath")
	if len(Options.cachePath) == 0 {
		fmt.Println("Your cachePath is empty. Quitting...")
		os.Exit(1)
	}
	if Options.cachePath == "" {
		Options.cachePath = home + "/.quickBlocks/cache/"
	}
	if Options.cachePath[len(Options.cachePath)-1] != '/' {
		Options.cachePath += "/"
	}
}
