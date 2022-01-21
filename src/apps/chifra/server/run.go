// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package servePkg

import (
	"log"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/spf13/cobra"
)

// Run runs serve command and sends any errors to log.Fatal
func RunServe(cmd *cobra.Command, args []string) error {
	err := Validate(cmd, args)
	if err != nil {
		return err
	}

	PrintServeSettings(os.Getenv("TEST_MODE") == "true")
	log.Fatal(RunInternal(Options.Port))

	return nil
}

func PrintServeSettings(testMode bool) {
	// 	log.Print(utils.Green, "Client:       ", utils.Off, Options.Status.Latest)
	// 	log.Print(utils.Green, "TrueBlocks:   ", utils.Off, Options.Status.TrueBlocks)

	apiUrl := Options.Port
	if !strings.Contains(apiUrl, "http") {
		apiUrl = "http://localhost" + apiUrl
	}
	chain := Options.Globals.Chain
	configPath := config.GetPathToRootConfig()
	chainConfigPath := config.GetPathToChainConfig(chain)
	cachePath := config.GetPathToCache(chain)
	indexPath := config.GetPathToIndex(chain)
	rpcProvider := config.GetRpcProvider(chain)
	meta := rpcClient.GetMetaData(chain, testMode)

	log.Printf("%s%-18.18s%s%s\n", colors.Green, "Server URL:", colors.Off, apiUrl)
	log.Printf("%s%-18.18s%s%s\n", colors.Green, "RootConfig Path:", colors.Off, configPath)
	log.Printf("%s%-18.18s%s%s\n", colors.Green, "ChainConfig Path:", colors.Off, chainConfigPath)
	log.Printf("%s%-18.18s%s%s\n", colors.Green, "Cache Path:", colors.Off, cachePath)
	log.Printf("%s%-18.18s%s%s\n", colors.Green, "Index Path:", colors.Off, indexPath)
	log.Printf("%s%-18.18s%s%s\n", colors.Green, "RPC Provider:", colors.Off, rpcProvider)
	log.Printf("%s%-18.18s%s%d, %d, %d, %d\n", colors.Green, "Progress:", colors.Off, meta.Latest, meta.Finalized, meta.Staging, meta.Unripe)
}

// TODO: Can probably remove this
// func ParseOptions() error {
// 	flag.IntVar(&Options.Verbose, "verbose", 0, "verbose level (between 0 and 10 inclusive)")
// 	flag.BoolVar(&Options.Pin, "pin", false, "pins Bloom filters and chunks to pinning service (requires API key)")
// 	flag.IntVar(&Options.Sleep, "sleep", 14, "specifies sleep interval between scrapes")

// 	flag.Parse()

// 	Options.Status, _ = GetChifraData()
// 	Options.Meta, _ = GetChifraMeta()

// 	if Options.Port != ":8080" {
// 		log.Print(utils.Green, "port:        ", utils.Off, Options.Port, "\n")
// 	} else {
// 		Options.Port = "127.0.0.1:8080"
// 	}

// 	if Options.Verbose > 0 {
// 		log.Print(utils.Green, "verbose:     ", utils.Off, Options.Verbose, "\n")
// 	}

// }

// // ChifraStatus
// type ChifraStatus struct {
// 	Client     string `json:"clientVersion"`
// 	TrueBlocks string `json:"trueblocksVersion"`
// 	RPC        string `json:"rpcProvider"`
// 	ConfigPath string `json:"config Path"`
// 	CachePath  string `json:"cache Path"`
// 	IndexPath  string `json:"index Path"`
// 	IsTesting  bool   `json:"isTesting"`
// 	IsDocker   bool   `json:"isDocker"`
// 	IsArchive  bool   `json:"isArchive"`
// 	IsTracing  bool   `json:"isTracing"`
// 	HasEsKey   bool   `json:"hasEskey"`
// 	HasPinKey  bool   `json:"hasPinkey"`
// 	Date       string `json:"date"`
// }

// // ChifraMeta
// type ChifraMeta struct {
// 	Client    int64
// 	Finalized int64
// 	Staging   int64
// 	Ripe      int64
// 	Unripe    int64
// }

// // ChifraResponse is sent by the command line chifra
// type ChifraResponse struct {
// 	Status []ChifraStatus `json:"data"`
// 	Meta   ChifraMeta     `json:"meta"`
// }

// func GetChifraResponse() (ChifraResponse, error) {
// 	cmd := exec.Command(config.GetPathToCommands("cacheStatus"), "--terse")
// 	cmd.Env = append(os.Environ(), "API_MODE=true")
// 	out, err := cmd.Out put()
// 	if err != nil {
// 		fmt.Printf("ChifraResponse:cmd.Out put() failed: %s", err)
// 		var junk ChifraResponse
// 		return junk, err
// 	}
// 	var response ChifraResponse
// 	err = json.Unmarshal(out, &response)
// 	if err != nil {
// 		fmt.Printf("ChifraResponse:json.Unmarshal() failed: %s", err)
// 		var junk ChifraResponse
// 		return junk, err
// 	}
// 	return response, nil
// }

// func GetChifraData() (ChifraStatus, error) {
// 	response, err := GetChifraResponse()
// 	if err != nil {
// 		var junk ChifraStatus
// 		return junk, err
// 	}
// 	if len(response.Status) == 0 {
// 		fmt.Printf("GetChifraData.len(response.Status) == 0")
// 		var junk ChifraStatus
// 		return junk, err
// 	}
// 	return response.Status[0], nil
// }

// func GetChifraMeta() (ChifraMeta, error) {
// 	response, err := GetChifraResponse()
// 	if err != nil {
// 		fmt.Printf("GetChifraMeta.GetChifraResponse failed with: %s", err)
// 		var junk ChifraMeta
// 		return junk, err
// 	}
// 	return response.Meta, nil
// }
