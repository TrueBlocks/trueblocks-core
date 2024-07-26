package types

import (
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/goMaker/codeWriter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// ProcessGroupFile processes a single file, applying the template to it and
// writing the result to the destination.
func (cb *CodeBase) ProcessGroupFile(source, reason, group string) error {
	cwd, _ := os.Getwd()
	source = filepath.Join(cwd, GetTemplatePath(), source)
	if ok, err := shouldProcess(source, "codebase"); err != nil {
		return err
	} else if !ok {
		return nil
	}

	gSource := strings.ReplaceAll(source, "/templates/", "/templates/generators/")
	tmpl := file.AsciiFileToString(gSource)
	if tmpl == "" {
		logger.Fatal("Could not read template file: ", gSource)
	}
	dest := convertToDestPath(source, "", "", group, reason)
	tmpl = strings.ReplaceAll(tmpl, "[{GROUP}]", group)
	tmpl = strings.ReplaceAll(tmpl, "[{REASON}]", reason)
	result := cb.executeTemplate(source+group+reason, tmpl)
	_, err := codeWriter.WriteCode(dest, result)
	return err
}
