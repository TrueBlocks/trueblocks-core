package types

import (
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/goMaker/codeWriter"
)

// IAMONE
// ProcessGroupFile processes a single file, applying the template to it and
// writing the result to the destination.
func (item *CodeBase) ProcessGroupFile(sourceIn, group, reason string) error {
	cwd, _ := os.Getwd()
	fullPath := filepath.Join(cwd, GetTemplatePath(), sourceIn)
	subPath := "groups"
	if ok, err := shouldProcess(fullPath, subPath, "codebase"); err != nil {
		return err
	} else if !ok {
		return nil
	}

	tmpl := getGeneratorContents(fullPath, subPath, group, reason)
	dest := convertToDestPath(fullPath, "", "", group, reason)

	tmplName := fullPath + group + reason
	result := item.executeTemplate(tmplName, tmpl)
	_, err := codeWriter.WriteCode(dest, result)

	return err
}
