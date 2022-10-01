// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"encoding/json"
	"fmt"
	"io"
	"log"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: Fix export without arrays
func RenderSlice[
	T types.SimpleNamedBlock |
		types.ReportClean |
		types.ReportCheck |
		types.SimpleFunction |
		types.SimpleMonitor |
		types.SimpleChunkRecord |
		types.SimpleAppearance |
		types.VerboseAppearance](opts *GlobalOptions, arr []T) error {

	data := make([]interface{}, len(arr))
	for i := range arr {
		data[i] = arr[i]
	}

	if opts.Writer == nil {
		log.Fatal("opts.Writer is nil")
	}

	var meta *rpcClient.MetaData = nil
	if opts.Format == "api" {
		var err error
		meta, err = rpcClient.GetMetaData(opts.Chain, opts.TestMode)
		if err != nil {
			return err
		}
	}

	if opts.IsApiMode() {
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

	switch opts.Format {
	case "api":
		fallthrough
	case "json":
		return output.OutputSlice(data, opts.Writer, opts.Format, opts.NoHeader, true, meta)
	case "csv":
		fallthrough
	case "txt":
		for i, item := range data {
			if i == 0 {
				if !opts.NoHeader {
					err := output.OutputHeader(item, opts.Writer, opts.Format)
					if err != nil {
						return err
					}
				}
			}
			err := output.OutputObject(item, opts.Writer, opts.Format, opts.NoHeader, false, meta)
			if err != nil {
				return err
			}
		}
		return nil
	}

	return fmt.Errorf("unsupported format %s", opts.Format)
}

// TODO: Fix export without arrays
func (opts *GlobalOptions) RenderObject(data interface{}, first bool) error {
	if opts.Writer == nil {
		log.Fatal("opts.Writer is nil")
	}

	// TODO: We may move this line to InitGlobals when we merge json and api formats
	format := opts.Format
	if opts.ShowRaw {
		// If users wants raw output, we will most probably print JSON
		format = "json"
	}
	return output.OutputObject(data, opts.Writer, format, opts.NoHeader, first, nil)
}

// TODO: Fix export without arrays
func (opts *GlobalOptions) RenderHeader(data interface{}, w *io.Writer, format string, hideHeader, first bool) error {
	if opts.IsApiMode() {
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

	if hideHeader || !first {
		return nil
	}

	return output.OutputHeader(data, *w, format)
}

// TODO: Fix export without arrays
func (opts *GlobalOptions) RenderFooter() error {
	if opts.Format == "api" || opts.Format == "json" {
		opts.Writer.Write([]byte("\n  ]"))
		showMeta := opts.IsApiMode() || opts.Format == "api"
		if showMeta {
			meta, err := rpcClient.GetMetaData(opts.Chain, opts.TestMode)
			if err != nil {
				return err
			}
			opts.Writer.Write([]byte(",\n  \"meta\": "))
			b, err := json.MarshalIndent(meta, "", "  ")
			if err != nil {
				return err
			}
			opts.Writer.Write(b)
		}
		opts.Writer.Write([]byte("\n}\n"))
	}
	return nil
}
