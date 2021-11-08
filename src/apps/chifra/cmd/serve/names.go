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
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"os"
	"os/exec"
	"strings"
)

func EditName(w http.ResponseWriter, r *http.Request) {
	r.ParseForm()
	newName := NewNamedAddress()
	for k := range r.Form {
		json.Unmarshal([]byte(k), &newName)
	}
	log.Print(Yellow, "Adding name: ", newName.ToJson(), Off)

	// Do the actual call
	cmd := exec.Command(GetCommandPath("ethNames"), "--create")
	cmd.Env = append(os.Environ(), "TB_NAME_ADDRESS="+newName.Address)
	cmd.Env = append(cmd.Env, "TB_NAME_NAME="+newName.Name)
	cmd.Env = append(cmd.Env, "TB_NAME_TAG="+newName.Tags)
	cmd.Env = append(cmd.Env, "TB_NAME_SOURCE="+newName.Source)
	cmd.Env = append(cmd.Env, "TB_NAME_SYMBOL="+newName.Symbol)
	cmd.Env = append(cmd.Env, "TB_NAME_DECIMALS="+newName.Decimals)
	cmd.Env = append(cmd.Env, "TB_NAME_DESCR="+newName.Description)
	out, err := cmd.Output()
	if err != nil {
		log.Print("Error from server: ", err)
	}
	output := string(out[:])
	log.Print(Yellow, string(output), Off)

	w.Header().Set("Content-Type", "application/json; charset=UTF-8")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")
	w.Header().Set("Access-Control-Allow-Methods", "PUT, POST, GET, DELETE, OPTIONS")
	if strings.Contains(output, "\"errors\":") {
		w.WriteHeader(http.StatusBadRequest)
	} else {
		w.WriteHeader(http.StatusOK)
	}
	fmt.Fprintf(w, "{ \"data\": [ "+newName.ToJson()+" ]}")
}

type NamedAddress struct {
	Tags        string `json:"tags"`
	Address     string `json:"address"`
	Name        string `json:"name"`
	Symbol      string `json:"symbol,omitempty"`
	Source      string `json:"source"`
	Decimals    string `json:"decimals,omitempty"`
	Description string `json:"description,omitempty"`
	Deleted     bool   `json:"deleted,omitempty"`
	IsCustom    bool   `json:"is_custom,omitempty"`
	IsPrefund   bool   `json:"is_prefund,omitempty"`
	IsContract  bool   `json:"is_contract,omitempty"`
	IsErc20     bool   `json:"is_erc20,omitempty"`
	IsErc721    bool   `json:"is_erc721,omitempty"`
}

func NewNamedAddress() NamedAddress {
	ret := new(NamedAddress)
	ret.Name = "none"
	ret.Address = "none"
	ret.Tags = "19-Unknown"
	ret.Source = "EtherScan.io"
	// ret.Description = ""
	return *ret
}

func (name *NamedAddress) ToJson() string {
	e, err := json.Marshal(name)
	if err != nil {
		fmt.Printf("%s", err)
		return ""
	}
	return string(e)
}
