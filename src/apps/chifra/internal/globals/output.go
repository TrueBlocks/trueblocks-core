// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"net/http"
	"os"
	"strings"

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

	var hw http.ResponseWriter
	var ok bool
	if opts.Writer != nil {
		hw, ok = opts.Writer.(http.ResponseWriter)
	}
	if opts.ApiMode && ok {
		// Decides which format should be used, calls the right Responder, sets HTTP
		// status code and writes a response
		hw.Header().Set("Content-Type", formatToMimeType[formatNotEmpty])
		opts.Format = formatNotEmpty
		err := output.Output2(hw, data, opts.Format, opts.Chain, opts.NoHeader, opts.TestMode)
		if err != nil {
			opts.RespondWithError(hw, http.StatusInternalServerError, err)
		}

	} else {
		err := output.Output2(os.Stdout, data, opts.Format, opts.Chain, opts.NoHeader, opts.TestMode)
		if err != nil {
			logger.Log(logger.Error, err)
		}
	}

	return nil
}

func (opts *GlobalOptions) OutputObject(data interface{}) error {
	return opts.OutputArray(data)
}

func (opts *GlobalOptions) GetExportFormat(cmd string) string {
	if strings.Contains(cmd, "json") {
		return "json"
	} else if strings.Contains(cmd, "txt") {
		return "txt"
	} else if strings.Contains(cmd, "csv") {
		return "csv"
	}
	if len(opts.Format) > 0 {
		return opts.Format
	}
	return "csv"
}
