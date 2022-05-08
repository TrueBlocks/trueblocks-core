// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"encoding/json"
	"log"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

// RespondWithError marshals the given error err into JSON
// that can be returned to the client and sets httpStatus HTTP error status code
func (opts *GlobalOptions) RespondWithError(w http.ResponseWriter, httpStatus int, err error) {
	type ErrorResponse struct {
		Errors []string `json:"errors,omitempty"`
	}
	marshalled, _ := json.MarshalIndent(ErrorResponse{Errors: []string{err.Error()}}, "", "  ")
	w.WriteHeader(httpStatus)
	w.Write(marshalled)
}

// TODO: Fix export without arrays
func (opts *GlobalOptions) OutputArray(data interface{}) error {
	if opts.ApiMode {
		if opts.Writer == nil {
			log.Fatal("opts.Writer is nil in OutputArray")
		}

		// We could check this, but if it's not empty, we know it's type
		hw, _ := opts.Writer.(http.ResponseWriter)
		switch opts.Format {
		case "txt":
			hw.Header().Set("Content-Type", "text/tab-separated-values")
		case "csv":
			hw.Header().Set("Content-Type", "text/csv")
		default:
			hw.Header().Set("Content-Type", "application/json")
		}

		err := output.Output(opts.Writer, data, opts.Format, opts.Chain, opts.NoHeader, opts.TestMode)
		if err != nil {
			opts.RespondWithError(hw, http.StatusInternalServerError, err)
		}

	} else {
		if opts.Format == "" || opts.Format == "none" {
			opts.Format = "txt"
		}
		err := output.Output(opts.Writer, data, opts.Format, opts.Chain, opts.NoHeader, opts.TestMode)
		if err != nil {
			logger.Log(logger.Error, err)
		}
	}
	return nil
}

func (opts *GlobalOptions) OutputObject(data interface{}) error {
	return opts.OutputArray(data)
}
