// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

import (
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/utils"
)

// HandleExample populates a subfolder of the ./examples folder with a skeleton
// of the files needed to run an example. This is a convenience function for
// developers to quickly get started with the example.
func (opts *InitOptions) HandleExample(rCtx *output.RenderCtx) error {
	_ = rCtx
	template := "base/"                                  // will use opts.Template in the future
	tmplFolder := filepath.Join("./templates", template) // will later support opts.Template
	exampleDir := filepath.Join("./", opts.Example)

	// We already know that the folder does not exist since it passed validation...
	_ = os.MkdirAll(exampleDir, 0755)

	logger.Info("Example created in ", filepath.Join("./", opts.Example)+" from "+tmplFolder)

	for _, fn := range []string{"main.go", "main_test.go", "go.mod", "README.md"} {
		copyOne(fn, opts.Example, tmplFolder)
	}

	// tidy
	cmd := "cd " + exampleDir + " && "
	cmd += "go get github.com/btcsuite/btcd && "
	cmd += "go mod tidy"
	utils.System(cmd)

	// update the go.work file
	os.Remove(filepath.Join("..", "go.work"))
	utils.System("source ../scripts/go-work-sync.sh")

	return nil
}

func copyOne(fn, exName, tmplFolder string) {
	contents := file.AsciiFileToString(filepath.Join(tmplFolder, fn+".tmpl"))
	contents = strings.ReplaceAll(contents, "[{NAME}]", utils.MakeFirstUpperCase(exName))
	contents = strings.ReplaceAll(contents, "[{LOWER}]", exName)
	fn = filepath.Join("./", exName, fn)
	_ = file.StringToAsciiFile(fn, contents)
	logger.Info("\t==> " + fn)
}
