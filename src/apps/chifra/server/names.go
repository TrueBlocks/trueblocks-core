// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package servePkg

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"os"
	"os/exec"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

func EditName(w http.ResponseWriter, r *http.Request) {
	r.ParseForm()
	newName := NewNamedAddress()
	for k := range r.Form {
		json.Unmarshal([]byte(k), &newName)
	}
	log.Print(colors.Yellow, "Adding name: ", newName.ToJson(), colors.Off)

	// Do the actual call
	cmd := exec.Command(config.GetPathToCommands("ethNames"), "--create")
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
	log.Print(colors.Yellow, string(output), colors.Off)

	w.Header().Set("Content-Type", "application/json; charset=UTF-8")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")
	w.Header().Set("Access-Control-Allow-Methods", "PUT, POST, GET, DELETE, OPTIONS")
	if strings.Contains(output, "\"errors\":") {
		w.WriteHeader(http.StatusBadRequest)
	} else {
		w.WriteHeader(http.StatusOK)
	}
	fmt.Fprintf(w, "{ \"data\": [ "+newName.ToJson()+"\n  ]\n}")
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
	IsCustom    bool   `json:"isCustom,omitempty"`
	IsPrefund   bool   `json:"isPrefund,omitempty"`
	IsContract  bool   `json:"isContract,omitempty"`
	IsErc20     bool   `json:"isErc20,omitempty"`
	IsErc721    bool   `json:"isErc721,omitempty"`
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
