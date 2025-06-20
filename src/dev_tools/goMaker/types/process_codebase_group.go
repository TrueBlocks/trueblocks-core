package types

import (
	"os"
	"path/filepath"
)

// ProcessGroupFile processes a single file, applying the template to it and
// writing the result to the destination.
func (item *CodeBase) ProcessGroupFile(source, group, reason string) error {
	VerboseLog("Processing group file:", source, "for group:", group, "reason:", reason)

	cwd, _ := os.Getwd()
	fullPath := filepath.Join(cwd, GetTemplatePath(), source)
	subPath := "groups"
	if ok, err := shouldProcess(fullPath, subPath, "codebase"); err != nil {
		return err
	} else if !ok {
		VerboseLog("  Skipping", source, "as it should not be processed")
		return nil
	}

	VerboseLog("  Reading template from:", fullPath)
	tmpl := getGeneratorContents(fullPath, subPath, group, reason)
	dest := convertToDestPath(fullPath, "", "", group, reason)

	VerboseLog("  Generating file:", dest)
	tmplName := fullPath + group + reason
	result := item.executeTemplate(tmplName, tmpl)
	_, err := WriteCode(dest, result)

	return err
}
