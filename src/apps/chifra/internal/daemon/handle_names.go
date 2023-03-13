// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package daemonPkg

import (
	"encoding/json"
	"fmt"
	"net/http"
	"os"
	"os/exec"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func EditName(w http.ResponseWriter, r *http.Request) {
	r.ParseForm()
	newName := types.SimpleName{}
	for k := range r.Form {
		json.Unmarshal([]byte(k), &newName)
	}
	logger.Print(colors.Yellow, "Adding name: ", newName, colors.Off)

	// Do the actual call
	cmd := exec.Command(config.GetPathToCommands("ethNames"), "--create")
	cmd.Env = append(os.Environ(), "TB_NAME_ADDRESS="+newName.Address.Hex())
	cmd.Env = append(cmd.Env, "TB_NAME_NAME="+newName.Name)
	cmd.Env = append(cmd.Env, "TB_NAME_TAG="+newName.Tags)
	cmd.Env = append(cmd.Env, "TB_NAME_SOURCE="+newName.Source)
	cmd.Env = append(cmd.Env, "TB_NAME_SYMBOL="+newName.Symbol)
	cmd.Env = append(cmd.Env, "TB_NAME_DECIMALS="+fmt.Sprintf("%d", newName.Decimals))
	out, err := cmd.Output()
	if err != nil {
		logger.Print("Error from server: ", err)
	}
	output := string(out[:])
	logger.Print(colors.Yellow, string(output), colors.Off)

	w.Header().Set("Content-Type", "application/json; charset=UTF-8")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")
	w.Header().Set("Access-Control-Allow-Methods", "PUT, POST, GET, DELETE, OPTIONS")
	if strings.Contains(output, "\"errors\":") {
		w.WriteHeader(http.StatusBadRequest)
	} else {
		w.WriteHeader(http.StatusOK)
	}
	bytes, _ := json.MarshalIndent(newName, "", "  ")
	fmt.Fprintf(w, "{ \"data\": [ "+string(bytes)+"\n  ]\n}")
}
