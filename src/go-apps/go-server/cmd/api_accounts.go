/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
package trueblocks

import (
	"net/http"
)

func AccountsAbis(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "grabABI")
}

func AccountsCollections(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "ethNames")
}

func AccountsExport(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "export")
}

func AccountsList(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "list")
}

func AccountsNames(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "ethNames")
}

func AccountsRm(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "rm")
}

func AccountsTags(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "ethNames")
}
