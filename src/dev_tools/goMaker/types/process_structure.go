package types

import (
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/goMaker/codeWriter"
)

// IAMONE
// ProcessFile processes a single file, applying the template to it and
// writing the result to the destination.
func (item *Structure) ProcessFile(sourceIn, group, reason string) error {
	cwd, _ := os.Getwd()
	fullPath := filepath.Join(cwd, GetTemplatePath(), sourceIn)
	if ok, err := shouldProcess(fullPath, item.Class); err != nil {
		return err
	} else if !ok {
		return nil
	}

	tmpl := getGeneratorContents(fullPath, group, reason)
	dest := convertToDestPath(fullPath, "", item.Name(), "", "")
	dest = strings.ReplaceAll(dest, "/src/apps/chifra/pkg/types/", "/src/apps/chifra/pkg/types/types_")

	tmplName := fullPath + group + reason
	result := item.executeTemplate(tmplName, tmpl)
	_, err := codeWriter.WriteCode(dest, result)

	return err
}
