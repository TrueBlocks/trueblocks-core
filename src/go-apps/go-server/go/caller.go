/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
package swagger

import (
	"fmt"
	"net/http"
	"os/exec"
)

func callOne(w http.ResponseWriter, r *http.Request, cmd string) {
	callOneExtra(w, r, cmd, "")
}

func callOneExtra(w http.ResponseWriter, r *http.Request, cmd, extra string) {
	w.Header().Set("Content-Type", "application/json; charset=UTF-8")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")
	w.Header().Set("Access-Control-Allow-Methods", "PUT, POST, GET, DELETE, OPTIONS")
	w.WriteHeader(http.StatusOK)
	allDogs := []string{}
	if extra != "" {
		allDogs = append(allDogs, extra)
	}
	for key, values := range r.URL.Query() {
		if key != "addrs" &&
			key != "terms" &&
			key != "modes" &&
			key != "blocks" &&
			key != "transactions" &&
			key != "block_list" &&
			key != "mode" &&
			key != "names" &&
			key != "addrs2" {
			allDogs = append(allDogs, "--"+key)
		}
		for _, value := range values {
			allDogs = append(allDogs, value)
		}
	}
	out, err := exec.Command(cmd, allDogs...).Output()
	if err != nil {
		fmt.Printf("%s", err)
	}
	output := string(out[:])
	fmt.Fprint(w, output)
}
