// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"net/http"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

var formatToMimeType = map[string]string{
	"api":  "application/json",
	"json": "application/json",
	"csv":  "text/csv",
	"txt":  "text/tab-separated-values",
}

// RespondWithError marshals the given error err into JSON
// that can be returned to the client and sets httpStatus HTTP error status code
func (opts *GlobalOptions) RespondWithError(w http.ResponseWriter, httpStatus int, err error) {
	formatted := output.JsonFormatted{Errors: []string{err.Error()}}
	data := []string{}
	marshalled, err := output.AsJsonBytes1(&formatted, data, opts.Format, opts.Chain, opts.TestMode)
	if err != nil {
		panic(err)
	}
	w.WriteHeader(httpStatus)
	w.Write(marshalled)
}

// TODO: Fix export without arrays
func (opts *GlobalOptions) OutputArray(data interface{}) error {
	formatNotEmpty := opts.Format
	if formatNotEmpty == "" {
		formatNotEmpty = "api"
	}

	if opts.ApiMode && opts.Writer != nil {
		// Decides which format should be used, calls the right Responder, sets HTTP
		// status code and writes a response
		opts.Writer.Header().Set("Content-Type", formatToMimeType[formatNotEmpty])
		opts.Format = formatNotEmpty
		err := output.Output2(opts.Writer, data, opts.Format, opts.Chain, opts.NoHeader, opts.TestMode)
		if err != nil {
			opts.RespondWithError(opts.Writer, http.StatusInternalServerError, err)
		}

	} else {
		err := output.Output2(os.Stdout, data, opts.Format, opts.Chain, opts.NoHeader, opts.TestMode)
		if err != nil {
			logger.Log(logger.Error, err)
		}
	}

	return nil
}
