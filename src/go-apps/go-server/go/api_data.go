/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
package trueblocks

import (
	"net/http"
)

func DataBlocks(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getBlock")
}

func DataLogs(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getLogs")
}

func DataReceipts(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getReceipt")
}

func DataTraces(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getTrace")
}

func DataTransactions(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getTrans")
}

func DataWhen(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "whenBlock")
}
