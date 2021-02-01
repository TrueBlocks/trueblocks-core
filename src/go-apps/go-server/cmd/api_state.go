/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
package trueblocks

import (
	"net/http"
)

func StateState(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getState")
}

func StateTokens(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getTokenInfo")
}
