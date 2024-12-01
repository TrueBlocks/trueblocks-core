package types

import (
	"os"
	"path/filepath"
)

// ProcessFile processes a single file, applying the template to it and
// writing the result to the destination.
func (item *CodeBase) ProcessFile(source, group, reason string) error {
	cwd, _ := os.Getwd()
	fullPath := filepath.Join(cwd, GetTemplatePath(), source)
	subPath := "codebase"
	if ok, err := shouldProcess(fullPath, subPath, "codebase"); err != nil {
		return err
	} else if !ok {
		return nil
	}

	tmpl := getGeneratorContents(fullPath, subPath, group, reason)
	dest := convertToDestPath(fullPath, "", "", group, reason)

	tmplName := fullPath + group + reason
	result := item.executeTemplate(tmplName, tmpl)
	_, err := WriteCode(dest, result)

	return err
}
