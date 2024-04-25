// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

import (
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleExample populates a subfolder of the ./examples folder with a skeleton
// of the files needed to run an example. This is a convenience function for
// developers to quickly get started with the example.
func (opts *InitOptions) HandleExample() error {
	_ = os.MkdirAll("./"+opts.Example, 0755)
	tmplFolder := "./templates/" // will later support a template name
	logger.Info("Example created in ./" + opts.Example + " from " + tmplFolder)
	for _, fn := range []string{"main.go", "main_test.go", "go.mod", "README.md"} {
		contents := file.AsciiFileToString(tmplFolder + fn + ".tmpl")
		contents = strings.ReplaceAll(contents, "[{NAME}]", utils.MakeFirstUpperCase(opts.Example))
		contents = strings.ReplaceAll(contents, "[{LOWER}]", opts.Example)
		fn = "./" + opts.Example + "/" + fn
		file.StringToAsciiFile(fn, contents)
		logger.Info("\t==> " + fn)
	}
	os.Remove("../go.work")
	utils.System("source ../scripts/make-go-work.sh")

	return nil
}
