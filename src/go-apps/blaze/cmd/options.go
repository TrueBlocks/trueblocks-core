package cmd

import (
	"fmt"
	"os"
	"runtime"
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
  Blaze is an internal-use-only component called by 'blockScrape' to 
  index blocks from the last visited block (startBlock) to the front
  of the chain (or for nBlocks if specified). It accumulates an index
  of every address as it appears anywhere in the data. You may
  later query this index (with acctExport) many orders of magnitude
  more quickly than scanning the blockchain directly. All of the
  component's numeric command line options are required.`,
	Version: "GHC-TrueBlocks, LLC//0.8.1-alpha",
}

// Execute adds all child commands to the root command and sets flags appropriately.
// This is called by main(). It only needs to happen once for the rootCmd.
func Execute() {
	err := rootCmd.Execute()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}

// OptionsType Structure to carry command line and config file options
type OptionsType struct {
	rpcProvider string
	indexPath   string
	ripePath    string
	unripePath  string
	startBlock  int
	nBlocks     int
	nBlockProcs int
	nAddrProcs  int
	ripeBlock   int
	verbose     int
}

// Options Carries the configuration options (from both command line and config file)
var Options OptionsType

// init Initalize options
func init() {
	cobra.OnInitialize(initConfig)

	rootCmd.PersistentFlags().StringVarP(&Options.rpcProvider, "rpcProvider", "r", "http://localhost:8545", "URL to the node's RPC")
	rootCmd.PersistentFlags().StringVarP(&Options.indexPath, "indexPath", "c", "", "The location of TrueBlocks' appearance cache (default \"$CONFIG/unchained\")")
	rootCmd.PersistentFlags().IntVarP(&Options.startBlock, "startBlock", "s", 0, "First block to visit (required)")
	rootCmd.PersistentFlags().IntVarP(&Options.nBlocks, "nBlocks", "n", 0, "The number of blocks to scrape (required)")
	rootCmd.PersistentFlags().IntVarP(&Options.nBlockProcs, "nBlockProcs", "b", 20, "The number of block processors to create (required)")
	rootCmd.PersistentFlags().IntVarP(&Options.nAddrProcs, "nAddrProcs", "a", 60, "The number of address processors to create (required)")
	rootCmd.PersistentFlags().IntVarP(&Options.ripeBlock, "ripeBlock", "e", 0, "Blocks prior to this value are written to 'ripe' folder (required)")
	rootCmd.PersistentFlags().IntVarP(&Options.verbose, "verbose", "v", 0, "Display more or less information")
	rootCmd.MarkPersistentFlagRequired("startBlock")
	rootCmd.MarkPersistentFlagRequired("nBlocks")
	rootCmd.MarkPersistentFlagRequired("ripeBlock")
}

// initConfig reads in config file and ENV variables if set.
func initConfig() {
	// Find home directory.
	home, err := homedir.Dir()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	configPath := "<unset>"
	// TODO(tjayrush): This should read XDG_DATA_HOME and if the folder exists, use it
	if runtime.GOOS == "darwin" {
		configPath = home + "/Library/Application Support/TrueBlocks"
	} else if runtime.GOOS == "linux" {
		configPath = home + "/.local/share/trueblocks"
	} else {
		fmt.Println("Windows not supported.")
		// TODO(tjayrush): This should fail without proceeding
	}
	viper.AddConfigPath(configPath)
	viper.SetConfigName("trueBlocks")
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
	viper.BindPFlag("settings.indexPath", rootCmd.PersistentFlags().Lookup("indexPath"))
	viper.BindEnv("rpcProvider")
	viper.BindEnv("indexPath")

	// Cleanup
	Options.rpcProvider = viper.GetString("settings.rpcProvider")
	if len(Options.rpcProvider) == 0 {
		fmt.Println("Your rpcProvider is empty. Quitting...")
		os.Exit(1)
	}
	if Options.rpcProvider[len(Options.rpcProvider)-1] != '/' {
		Options.rpcProvider += "/"
	}

	Options.indexPath = viper.GetString("settings.indexPath")
	if Options.indexPath == "" {
		Options.indexPath = configPath + "/unchained/"
	}
	if Options.indexPath[len(Options.indexPath)-1] != '/' {
		Options.indexPath += "/"
	}

	Options.ripePath = Options.indexPath + "ripe/"
	if _, err := os.Stat(Options.ripePath); os.IsNotExist(err) {
		os.MkdirAll(Options.ripePath, 0777)
	}

	Options.unripePath = Options.indexPath + "unripe/"
	if _, err := os.Stat(Options.unripePath); os.IsNotExist(err) {
		os.MkdirAll(Options.unripePath, 0777)
	}

	if Options.verbose > 8 {
		fmt.Println("blaze.rpcProvider: ", Options.rpcProvider)
		fmt.Println("blaze.indexPath:   ", Options.indexPath)
		fmt.Println("blaze.ripePath:    ", Options.ripePath)
		fmt.Println("blaze.unripePath:  ", Options.unripePath)
	}

	if Options.verbose > 4 {
		fmt.Println("blaze.startBlock:  ", Options.startBlock)
		fmt.Println("blaze.nBlocks:     ", Options.nBlocks)
		fmt.Println("blaze.nBlockProcs: ", Options.nBlockProcs)
		fmt.Println("blaze.nAddrProcs:  ", Options.nAddrProcs)
		fmt.Println("blaze.ripeBlock:   ", Options.ripeBlock)
		fmt.Println("blaze.verbose:     ", Options.verbose)
	}
}
