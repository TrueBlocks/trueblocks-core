package types

import (
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/goMaker/codeWriter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// ProcessFile processes a single file, applying the template to it and
// writing the result to the destination.
func (cb *CodeBase) ProcessFile(source string) error {
	cwd, _ := os.Getwd()
	source = filepath.Join(cwd, templateFolder, source)
	if ok, err := shouldProcess(source, "codebase"); err != nil {
		return err
	} else if !ok {
		return nil
	}

	tmpl := file.AsciiFileToString(source)
	result := cb.executeTemplate(source, tmpl)
	return codeWriter.WriteCode(convertToDestPath(source, "", "", ""), result)
}

// ProcessGroupFile processes a single file, applying the template to it and
// writing the result to the destination.
func (cb *CodeBase) ProcessGroupFile(source, group string) error {
	cwd, _ := os.Getwd()
	source = filepath.Join(cwd, templateFolder, source)
	if ok, err := shouldProcess(source, "codebase"); err != nil {
		return err
	} else if !ok {
		return nil
	}

	tmpl := file.AsciiFileToString(source)
	dest := convertToDestPath(source, "", "", group)
	tmpl = strings.Replace(tmpl, "[{GROUP}]", group, -1)
	result := cb.executeTemplate(source+group, tmpl)
	return codeWriter.WriteCode(dest, result)
}
