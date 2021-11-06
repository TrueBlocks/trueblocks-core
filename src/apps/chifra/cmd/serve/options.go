package serve

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

import (
	"fmt"
)

// ChifraStatus
type ChifraStatus struct {
	Client     string `json:"client_version"`
	TrueBlocks string `json:"trueblocks_version"`
	RPC        string `json:"rpc_provider"`
	ConfigPath string `json:"config_path"`
	CachePath  string `json:"cache_path"`
	IndexPath  string `json:"index_path"`
	IsTesting  bool   `json:"is_testing"`
	IsDocker   bool   `json:"is_docker"`
	IsArchive  bool   `json:"is_archive"`
	IsTracing  bool   `json:"is_tracing"`
	HasEsKey   bool   `json:"has_eskey"`
	HasPinKey  bool   `json:"has_pinkey"`
	Date       string `json:"date"`
}

// ChifraMeta
type ChifraMeta struct {
	Client    int64
	Finalized int64
	Staging   int64
	Ripe      int64
	Unripe    int64
}

// ChifraResponse is sent by the command line chifra
type ChifraResponse struct {
	Status []ChifraStatus `json:"data"`
	Meta   ChifraMeta     `json:"meta"`
}

func GetChifraResponse() (ChifraResponse, error) {
	// TODO: fix
	// cmd := exec.Command(GetCommandPath("cacheStatus"), "--terse")
	// cmd.Env = append(os.Environ(), "API_MODE=true")
	// out, err := cmd.Output()
	// if err != nil {
	// 	fmt.Printf("ChifraResponse:cmd.Output() failed: %s", err)
	// 	var junk ChifraResponse
	// 	return junk, err
	// }
	// var response ChifraResponse
	// err = json.Unmarshal(out, &response)
	// if err != nil {
	// 	fmt.Printf("ChifraResponse:json.Unmarshal() failed: %s", err)
	// 	var junk ChifraResponse
	// 	return junk, err
	// }
	// return response, nil
	return ChifraResponse{}, nil
}

func GetChifraData() (ChifraStatus, error) {
	response, err := GetChifraResponse()
	if err != nil {
		var junk ChifraStatus
		return junk, err
	}
	if len(response.Status) == 0 {
		fmt.Printf("GetChifraData.len(response.Status) == 0")
		var junk ChifraStatus
		return junk, err
	}
	return response.Status[0], nil
}

func GetChifraMeta() (ChifraMeta, error) {
	response, err := GetChifraResponse()
	if err != nil {
		fmt.Printf("GetChifraMeta.GetChifraResponse failed with: %s", err)
		var junk ChifraMeta
		return junk, err
	}
	return response.Meta, nil
}

// OptionsType makes command line options available to the package
type OptionsType struct {
	Scrape  bool
	Monitor bool
	Port    string
	Verbose int
	Sleep   int
	Pin     bool
	Status  ChifraStatus
	Meta    ChifraMeta
}

// Options carries command line options
var Options OptionsType
