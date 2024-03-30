// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --openapi. Edit only those parts of
 * the code outside of 'BEG_ROUTE/END_ROUTE' tags.
 */

package daemonPkg

import (
	"encoding/json"
	"fmt"
	"net/http"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/gorilla/mux"
	"golang.org/x/time/rate"
)

// RespondWithError marshals an err into JSON and returns the bytes
// back to the caller httpStatus HTTP error status code
func RespondWithError(w http.ResponseWriter, httpStatus int, err error) {
	type ErrorResponse struct {
		Errors []string `json:"errors,omitempty"`
	}
	marshalled, _ := json.MarshalIndent(ErrorResponse{Errors: []string{err.Error()}}, "", "  ")
	w.WriteHeader(httpStatus)
	_, _ = w.Write(marshalled)
}

// NewRouter Creates a new router given the routes array
func NewRouter() *mux.Router {
	router := mux.NewRouter().StrictSlash(true)
	router.Use(CorsHandler)
	router.
		Methods("OPTIONS").
		Handler(OptionsHandler)
	router.Use(ContentTypeHandler)

	for _, route := range routes {
		var handler http.Handler
		handler = route.HandlerFunc
		handler = Logger(handler, route.Name)
		router.
			Methods(route.Method).
			Path(route.Pattern).
			Name(route.Name).
			Handler(handler)
	}

	return router
}

func addCorsHeaders(w http.ResponseWriter) {
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")
	w.Header().Set("Access-Control-Allow-Methods", "PUT, POST, GET, DELETE, OPTIONS")
}

var OptionsHandler = http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
	addCorsHeaders(w)
})

// CorsHandler handles CORS requests
func CorsHandler(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		addCorsHeaders(w)
		next.ServeHTTP(w, r)
	})
}

// ContentTypeHandler sets correct Content-Type header on response
func ContentTypeHandler(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		requestedFormat := r.URL.Query().Get("fmt")

		var contentType string
		switch requestedFormat {
		case "txt":
			contentType = "text/plain"
		case "csv":
			contentType = "text/csv"
		default:
			contentType = "application/json"
		}

		w.Header().Set("Content-Type", contentType)
		next.ServeHTTP(w, r)
	})
}

var nProcessed int

// Logger sends information to the server's console
func Logger(inner http.Handler, name string) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		var limiter = rate.NewLimiter(1, 3)
		if !limiter.Allow() {
			http.Error(w, http.StatusText(http.StatusTooManyRequests), http.StatusTooManyRequests)
			return
		}
		start := time.Now()
		inner.ServeHTTP(w, r)
		t := ""
		if isTestModeServer(r) {
			t = "-test"
		}
		msg := fmt.Sprintf("%d %s%s %s %s %s", nProcessed, r.Method, t, r.RequestURI, name, time.Since(start))
		logger.Info(msg)
		nProcessed++
	})
}

// isTestModeServer return true if we are running from the testing harness
func isTestModeServer(r *http.Request) bool {
	return r.Header.Get("User-Agent") == "testRunner"
}
