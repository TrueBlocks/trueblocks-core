package types

import (
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/goMaker/codeWriter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// ProcessFile processes a single file, applying the template to it and
// writing the result to the destination.
func (c *Command) ProcessFile(source string) error {
	cwd, _ := os.Getwd()
	source = filepath.Join(cwd, templateFolder, source)
	if ok, err := shouldProcess(source, c.Route); err != nil {
		return err
	} else if !ok {
		return nil
	}

	tmpl := file.AsciiFileToString(source)
	result := c.executeTemplate(source, tmpl)
	return codeWriter.WriteCode(convertToDestPath(source, c.Route, "", ""), result)
}
