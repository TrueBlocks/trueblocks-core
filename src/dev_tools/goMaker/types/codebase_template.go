package types

import (
	"bytes"
	"log"
	"os"
	"path/filepath"
	"strings"
	"text/template"

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

	result := cb.executeTemplate(source, file.AsciiFileToString(source))
	return codeWriter.WriteCode(convertToDestPath(source, "", ""), result)
}

// executeTemplate executes the template with the given name and returns
// the result. It stores the parsed template in the templates map to avoid
// parsing it more than once.
func (cb *CodeBase) executeTemplate(name, tmplCode string) string {
	if cb.templates == nil {
		cb.templates = make(map[string]*template.Template)
	}

	if cb.templates[name] == nil {
		toSnake := func(s string) string { return strings.ToLower(s[0:1]) + s[1:] }
		funcMap := template.FuncMap{"toSnake": toSnake}
		cb.templates[name] = template.Must(template.New(name).Funcs(funcMap).Parse(tmplCode))
	}

	var tplBuffer bytes.Buffer
	if err := cb.templates[name].Execute(&tplBuffer, cb); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}
	return tplBuffer.String()
}
