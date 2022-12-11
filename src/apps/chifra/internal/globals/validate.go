// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"fmt"
	"path/filepath"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/bykof/gostradamus"
)

func (opts *GlobalOptions) Validate() error {
	if len(opts.File) > 0 {
		if opts.IsApiMode() {
			return validate.Usage("The {0} option is not available in {1} mode", "--file", "Api")
		}
		if !file.FileExists(opts.File) {
			return validate.Usage("The {0} option ({1}) must {2}", "file", opts.File, "exist")
		}
	}

	if len(opts.File) > 0 && !file.FileExists(opts.File) {
		return validate.Usage("The {0} option ({1}) must {2}", "file", opts.File, "exist")
	}

	if len(opts.OutputFn) > 0 && opts.IsApiMode() {
		return validate.Usage("The {0} option is not available in Api Mode. Use {1} instead", "--output", "--to_file")
	}

	if len(opts.OutputFn) > 0 && opts.ToFile {
		return validate.Usage("Choose either the {0} option or the {1} option. Not both.", "--output", "--to_file")
	}

	if len(opts.OutputFn) == 0 && opts.ToFile {
		fn := fmt.Sprintf("/tmp/%s.%s", gostradamus.Now().Format("YYYY_MM_DD_HH_mm_ss"), opts.Format)
		opts.OutputFn = fn
		opts.ToFile = false
	}

	tmpPath := filepath.Join(config.GetPathToCache(opts.Chain), "tmp", "checkProvider.txt")
	if !file.FileExists(tmpPath) {
		if version, err := rpcClient.GetVersion(opts.Chain); err != nil {
			logger.Fatal("Cannot connect with the node software.", err)
		} else {
			file.StringToAsciiFile(tmpPath, version)
		}
	}

	err := validate.ValidateEnum("--fmt", opts.Format, "[json|txt|csv]")
	if err != nil {
		return err
	}
	return nil
}

// TODO: This could be removed by changing the code generator
func ToFloat64(val string) float64 {
	f, _ := strconv.ParseFloat(val, 64)
	return f
}

// TODO: This could be removed by changing the code generator
func ToUint64(val string) uint64 {
	f, _ := strconv.ParseUint(val, 10, 64)
	return f
}
