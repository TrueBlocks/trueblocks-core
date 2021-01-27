/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
package swagger

import (
	"log"
	"net/http"
	"time"
)

var cnt int

func Logger(inner http.Handler, name string) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		start := time.Now()

		inner.ServeHTTP(w, r)

		log.Printf(
			"%d %s %s %s %s",
			cnt,
			r.Method,
			r.RequestURI,
			name,
			time.Since(start),
		)
		cnt++
	})
}
