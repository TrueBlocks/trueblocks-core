package types

import (
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (c *Command) HasNotes() bool {
	return len(c.Notes) > 0
}

func (c *Command) HelpNotes() string {
	readmePath := filepath.Join(GetTemplatePath(), "readme-intros", c.ReadmeName())
	readmePath = strings.ReplaceAll(readmePath, ".md", ".notes.md")
	if file.FileExists(readmePath) {
		tmplName := "Notes" + c.ReadmeName()
		tmpl := file.AsciiFileToString(readmePath)
		if tmpl == "" {
			logger.Fatal("Could not read template file: ", readmePath)
		}
		if err := ValidateTemplate(tmpl, readmePath); err != nil {
			logger.Fatal(err)
		}
		return "\n\n" + strings.Trim(c.executeTemplate(tmplName, tmpl), ws)
	}
	return ""
}
