package types

import (
	"bytes"
	"log"
	"os"
	"path/filepath"
	"text/template"

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

	result := c.executeTemplate(source, file.AsciiFileToString(source))
	return codeWriter.WriteCode(convertToDestPath(source, c.Route, ""), result)
}

// executeTemplate executes the template with the given name and returns
// the result. It stores the parsed template in the templates map to avoid
// parsing it more than once.
func (c *Command) executeTemplate(name, tmplCode string) string {
	if c.templates == nil {
		c.templates = make(map[string]*template.Template)
	}

	if c.templates[name] == nil {
		var err error
		c.templates[name], err = template.New(name).Parse(tmplCode)
		if err != nil {
			log.Fatalf("parsing template failed: %v", err)
		}
	}

	var tplBuffer bytes.Buffer
	if err := c.templates[name].Execute(&tplBuffer, c); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}
	return tplBuffer.String()
}
