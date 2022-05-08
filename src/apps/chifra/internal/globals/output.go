// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"io"
	"log"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: Fix export without arrays
func RenderSlice[
	T types.NamedBlock |
		types.Function |
		types.CleanReport |
		types.SimpleMonitor |
		types.SimplePinList |
		types.SimpleAppearance](opts *GlobalOptions, arr []T) error {
	data := make([]interface{}, len(arr))
	for i := range arr {
		data[i] = arr[i]
	}

	if opts.Writer == nil {
		log.Fatal("opts.Writer is nil")
	}

	var meta *rpcClient.MetaData = nil
	if opts.Format == "api" {
		meta = rpcClient.GetMetaData(opts.Chain, opts.TestMode)
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

		err := output.OutputArray(data, opts.Writer, opts.Format, opts.NoHeader, opts.ApiMode, meta)
		if err != nil {
			opts.RespondWithError(hw, http.StatusInternalServerError, err)
		}

	} else {
		err := output.OutputArray(data, opts.Writer, opts.Format, opts.NoHeader, opts.ApiMode, meta)
		if err != nil {
			logger.Log(logger.Error, err)
		}
	}
	return nil
}

func (opts *GlobalOptions) RenderObject(data interface{}) error {
	if opts.Writer == nil {
		log.Fatal("opts.Writer is nil")
	}

	var meta *rpcClient.MetaData = nil
	if opts.Format == "api" {
		meta = rpcClient.GetMetaData(opts.Chain, opts.TestMode)
	}

	if opts.ApiMode {
		// We could check this, but if it's not empty, we know it's type
		err := output.OutputObject(data, opts.Writer, opts.Format, opts.NoHeader, opts.ApiMode, meta)
		if err != nil {
			hw, _ := opts.Writer.(http.ResponseWriter)
			opts.RespondWithError(hw, http.StatusInternalServerError, err)
		}

	} else {
		err := output.OutputObject(data, opts.Writer, opts.Format, opts.NoHeader, opts.ApiMode, meta)
		if err != nil {
			logger.Log(logger.Error, err)
		}
	}
	return nil
}

func (opts *GlobalOptions) RenderHeader(data interface{}, w *io.Writer, format string, apiMode, showHeader, first bool) error {
	if apiMode {
		// We could check this, but if it's not empty, we know it's type
		hw, _ := (*w).(http.ResponseWriter)
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

	return output.OutputHeader(data, *w, format, apiMode)
}
