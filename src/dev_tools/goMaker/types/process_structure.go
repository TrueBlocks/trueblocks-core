package types

import (
	"os"
	"path/filepath"
	"strings"
)

// ProcessFile processes a single file, applying the template to it and
// writing the result to the destination.
func (item *Structure) ProcessFile(sourceIn, group, reason string) error {
	VerboseLog("Processing structure file:", sourceIn, "for type:", item.Name())

	cwd, _ := os.Getwd()
	fullPath := filepath.Join(cwd, GetTemplatePath(), sourceIn)
	subPath := "types"
	if ok, err := shouldProcess(fullPath, subPath, item.Class); err != nil {
		return err
	} else if !ok {
		VerboseLog("  Skipping", sourceIn, "as it should not be processed")
		return nil
	}

	VerboseLog("  Reading template from:", fullPath)
	tmpl := getGeneratorContents(fullPath, subPath, group, reason)
	// For types in pkg/types/<route>/<type>.go format, pass the route
	route := item.Route
	if route == "" {
		route = strings.ToLower(item.Class)
	}
	dest := convertToDestPath(fullPath, route, item.Name(), group, reason)

	VerboseLog("  Generating file:", dest)
	tmplName := fullPath + group + reason
	result := item.executeTemplate(tmplName, tmpl)
	_, err := WriteCode(dest, result)

	return err
}
