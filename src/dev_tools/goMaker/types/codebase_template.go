package types

import (
	"bytes"
	"log"
	"os"
	"path/filepath"
	"sort"
	"strings"
	"text/template"

	"github.com/TrueBlocks/trueblocks-core/goMaker/codeWriter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

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

// Generate generates the code for the codebase using the given templates.
func (cb *CodeBase) Generate(cbTmpls, routeTmpls, typeTmpls []string) {
	// var wg sync.WaitGroup

	// wg.Add(len(cbTmpls))
	for _, source := range cbTmpls {
		// go func() {
		// 	defer wg.Done()
		if err := cb.ProcessFile(source); err != nil {
			logger.Fatal(err)
		}
		// }()
	}

	// wg.Add(len(routeTmpls) * len(cb.Commands))
	for _, source := range routeTmpls {
		for _, c := range cb.Commands {
			// go func() {
			// 	defer wg.Done()
			if err := c.ProcessFile(source); err != nil {
				logger.Fatal(err)
			}
			// }()
		}
	}

	// wg.Add(len(typeTmpls) * len(cb.Structures))
	for _, source := range typeTmpls {
		for _, s := range cb.Structures {
			// go func() {
			// 	defer wg.Done()
			sort.Slice(s.Members, func(i, j int) bool {
				return s.Members[i].SortName() < s.Members[j].SortName()
			})
			if err := s.ProcessFile(source); err != nil {
				logger.Fatal(err)
			}
			// }()
		}
	}

	logger.Info(strings.Repeat(" ", 120))
	// wg.Wait()
}
