// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"encoding/json"
	"io"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

// TODO: Fix export without arrays
func (opts *GlobalOptions) RenderObject(data interface{}, first bool) error {
	if opts.Writer == nil {
		logger.Fatal("opts.Writer is nil")
	}

	// TODO: We may move this line to InitGlobals when we merge json and api formats
	return output.OutputObject(data, opts.Writer, opts.Format, opts.NoHeader, first, nil)
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
	if opts.Format == "json" {
		opts.Writer.Write([]byte("\n  ]"))
		if opts.IsApiMode() {
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

func (opts *GlobalOptions) OutputOpts() output.OutputOptions {
	return output.OutputOptions{
		Writer:     opts.Writer,
		Chain:      opts.Chain,
		TestMode:   opts.TestMode,
		NoHeader:   opts.NoHeader,
		ShowRaw:    opts.ShowRaw,
		Verbose:    opts.Verbose,
		LogLevel:   opts.LogLevel,
		Format:     opts.Format,
		OutputFn:   opts.OutputFn,
		Append:     opts.Append,
		JsonIndent: "  ",
	}
}
