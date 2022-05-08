// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"encoding/json"
	"io"
	"log"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
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
func (opts *GlobalOptions) RenderSlice(data []interface{}) error {
	if opts.Writer == nil {
		log.Fatal("opts.Writer is nil in RenderSlice")
	}

	if opts.ApiMode {
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

		err := output.OutputArray(data, opts.Writer, opts.Format, opts.Chain, opts.NoHeader, opts.ApiMode, opts.TestMode)
		if err != nil {
			opts.RespondWithError(hw, http.StatusInternalServerError, err)
		}

	} else {
		if opts.Format == "" || opts.Format == "none" {
			opts.Format = "txt"
		}
		err := output.OutputArray(data, opts.Writer, opts.Format, opts.Chain, opts.NoHeader, opts.ApiMode, opts.TestMode)
		if err != nil {
			logger.Log(logger.Error, err)
		}
	}
	return nil
}

func (opts *GlobalOptions) RenderObject(data interface{}) error {
	if opts.Writer == nil {
		log.Fatal("opts.Writer is nil in RenderSlice")
	}

	if opts.ApiMode {
		// We could check this, but if it's not empty, we know it's type
		var meta *rpcClient.MetaData = nil
		if opts.Format == "api" {
			meta = rpcClient.GetMetaData(opts.Chain, opts.TestMode)
		}
		err := output.OutputObject(data, opts.Writer, opts.Format, opts.NoHeader, opts.ApiMode, meta)
		if err != nil {
			hw, _ := opts.Writer.(http.ResponseWriter)
			opts.RespondWithError(hw, http.StatusInternalServerError, err)
		}

	} else {
		err := output.OutputObject(data, opts.Writer, opts.Format, opts.NoHeader, opts.ApiMode, nil)
		if err != nil {
			logger.Log(logger.Error, err)
		}
	}
	return nil
}

func (opts *GlobalOptions) RenderHeader(data interface{}, w *io.Writer, format string, apiMode, showHeader, first bool) error {
	if apiMode {
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
	}

	if !showHeader || !first {
		return nil
	}

	return output.OutputHeader(data, opts.Writer, format, apiMode)
}
