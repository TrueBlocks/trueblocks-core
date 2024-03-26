package types

import (
	"bytes"
	"log"
	"os"
	"path/filepath"
	"sync"
	"text/template"

	"github.com/TrueBlocks/trueblocks-core/goMaker/codeWriter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

var m sync.Mutex

// ProcessFile processes a single file, applying the template to it and
// writing the result to the destination.
func (cb *CodeBase) ProcessFile(source string) error {
	cwd, _ := os.Getwd()
	source = filepath.Join(cwd, templateFolder, source)
	if ok, err := shouldProcess(source, ""); err != nil {
		return err
	} else if !ok {
		return nil
	}

	result := cb.executeTemplate(source, file.AsciiFileToString(source))

	dest := convertToDestPath(source, "")

	err := codeWriter.WriteCode(dest, result)
	defer func() {
		m.Unlock()
	}()
	m.Lock()
	logger.Info("Writing to: ", dest)
	return err
}

// executeTemplate executes the template with the given name and returns
// the result. It stores the parsed template in the templates map to avoid
// parsing it more than once.
func (cb *CodeBase) executeTemplate(name, tmplCode string) string {
	if cb.templates == nil {
		cb.templates = make(map[string]*template.Template)
	}

	if cb.templates[name] == nil {
		var err error
		cb.templates[name], err = template.New(name).Parse(tmplCode)
		if err != nil {
			log.Fatalf("parsing template failed: %v", err)
		}
	}

	var tplBuffer bytes.Buffer
	if err := cb.templates[name].Execute(&tplBuffer, cb); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}
	return tplBuffer.String()
}
