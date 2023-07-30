// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"fmt"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
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
		return validate.Usage("The {0} option is not available in Api Mode.", "--output")
	}

	// TODO: Can we re-enable this? It doesn't work in Sepolia under docker. Returns a really weird message.
	// tmpPath := filepath.Join(config.GetPathToCache(opts.Chain), "tmp", "checkProvider.txt")
	// if !file.FileExists(tmpPath) {
	// 	if version, err := rpcClient.GetClientVersion(opts.Chain); err != nil {
	// 		logger.Fatal("Cannot connect with the node software.", version, err)
	// 	} else {
	// 		file.StringToAsciiFile(tmpPath, version)
	// 	}
	// }

	err := validate.ValidateEnum("--fmt", opts.Format, "[json|txt|csv]")
	if err != nil {
		return err
	}

	// TODO: This hack is here to make test cases pass. It can be removed at some point
	if opts.Format == "json" && len(opts.OutputFn) > 0 && opts.TestMode {
		fmt.Println("{ \"outputFilename\": \"--output_filename--\" }")
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
