/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
package swagger

import (
	"net/http"
)

func OtherDive(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "turboDive")
}

func OtherQuotes(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "ethQuote")
}

func OtherSlurp(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "ethslurp")
}

func OtherWhere(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "whereBlock")
}
