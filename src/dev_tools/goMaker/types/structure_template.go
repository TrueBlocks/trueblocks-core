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

func (s *Structure) ProcessFile(source string) error {
	cwd, _ := os.Getwd()
	source = filepath.Join(cwd, templateFolder, source)
	if ok, err := shouldProcess(source, s.Class); err != nil {
		return err
	} else if !ok {
		return nil
	}

	result := s.executeTemplate(source, file.AsciiFileToString(source))
	if s.DisableGo {
		// logger.Info(s.Class, "disabled", s.GoOutput)
		return nil
	} else if !strings.Contains(s.GoOutput, "/internal/") {
		dest := convertToDestPath(source, s.Name)
		dest = strings.Replace(dest, "//src/apps/chifra/pkg/types/", "/"+s.GoOutput+"/types_", -1)
		return codeWriter.WriteCode(dest, result)
	} else {
		return nil
	}
}

// executeTemplate executes the template with the given name and returns
// the result. It stores the parsed template in the templates map to avoid
// parsing it more than once.
func (s *Structure) executeTemplate(name, tmplCode string) string {
	if s.templates == nil {
		s.templates = make(map[string]*template.Template)
	}

	if s.templates[name] == nil {
		var err error
		s.templates[name], err = template.New(name).Parse(tmplCode)
		if err != nil {
			log.Fatalf("parsing template failed: %v", err)
		}
	}

	var tplBuffer bytes.Buffer
	if err := s.templates[name].Execute(&tplBuffer, s); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}

	return tplBuffer.String()
}
