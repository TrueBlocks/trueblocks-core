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
	if s.DisableGo {
		return nil
	}

	cwd, _ := os.Getwd()
	source = filepath.Join(cwd, templateFolder, source)
	if ok, err := shouldProcess(source, s.Class); err != nil {
		return err
	} else if !ok {
		return nil
	}

	dest := ""
	sourceIn := strings.Contains(source, "internal")
	destIn := strings.Contains(s.GoOutput, "internal")
	if destIn && !sourceIn || sourceIn && !destIn {
		// fmt.Println("Mismatch", s.Class)
		return nil
	} else if destIn {
		s.Route = grabRoute(s.GoOutput)
		dest = convertToDestPath(source, s.Route, s.Name)
		dest = strings.Replace(dest, "/types/", "/types_", -1)
		// return nil
	} else {
		dest = convertToDestPath(source, "", s.Name)
	}

	result := s.executeTemplate(source, file.AsciiFileToString(source))
	dest = strings.Replace(dest, "//src/apps/chifra/pkg/types/", "/"+s.GoOutput+"/types_", -1)
	return codeWriter.WriteCode(dest, result)
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

func grabRoute(dest string) string {
	if !strings.Contains(dest, "/internal/") {
		return ""
	}
	parts := strings.Split(dest, "/")
	return parts[len(parts)-1]
}
