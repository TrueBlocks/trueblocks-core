// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"encoding/json"
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
	var hw http.ResponseWriter
	var ok bool
	if opts.Writer != nil {
		hw, ok = opts.Writer.(http.ResponseWriter)
	}

	if opts.ApiMode && ok {
		// Decides which format should be used, calls the right Responder, sets HTTP
		// status code and writes a response
		switch opts.Format {
		case "txt":
			hw.Header().Set("Content-Type", "text/tab-separated-values")
		case "csv":
			hw.Header().Set("Content-Type", "text/csv")
		default:
			hw.Header().Set("Content-Type", "application/json")
		}

		err := output.Output3(hw, data, opts.Format, opts.Chain, opts.NoHeader, opts.TestMode)
		if err != nil {
			opts.RespondWithError(hw, http.StatusInternalServerError, err)
		}

	} else {
		err := output.Output2(opts.Writer, data, opts.Format, opts.Chain, opts.NoHeader, opts.TestMode)
		if err != nil {
			logger.Log(logger.Error, err)
		}
	}

	return nil
}

func (opts *GlobalOptions) OutputObject(data interface{}) error {
	return opts.OutputArray(data)
}
