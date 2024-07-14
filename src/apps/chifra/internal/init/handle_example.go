// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

import (
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleExample populates a subfolder of the ./examples folder with a skeleton
// of the files needed to run an example. This is a convenience function for
// developers to quickly get started with the example.
func (opts *InitOptions) HandleExample(rCtx output.RenderCtx) error {
	template := "base/"                     // will use opts.Template in the future
	tmplFolder := "./templates/" + template // will later support opts.Template

	// We already know that the folder does not exist since it passed validation...
	_ = os.MkdirAll("./"+opts.Example, 0755)

	logger.Info("Example created in ./" + opts.Example + " from " + tmplFolder)

	for _, fn := range []string{"main.go", "main_test.go", "go.mod", "README.md"} {
		copyOne(fn, opts.Example, tmplFolder)
	}

	// update the go.work file
	os.Remove("../go.work")
	utils.System("source ../scripts/go-work-sync.sh")

	return nil
}

func copyOne(fn, exName, tmplFolder string) {
	contents := file.AsciiFileToString(tmplFolder + fn + ".tmpl")
	contents = strings.ReplaceAll(contents, "[{NAME}]", utils.MakeFirstUpperCase(exName))
	contents = strings.ReplaceAll(contents, "[{LOWER}]", exName)
	fn = "./" + exName + "/" + fn
	_ = file.StringToAsciiFile(fn, contents)
	logger.Info("\t==> " + fn)
}
