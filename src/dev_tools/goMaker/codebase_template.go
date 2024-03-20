package main

import (
	"bytes"
	"log"
	"strings"
	"text/template"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (cb *CodeBase) processFile(source string) {
	source = strings.Replace(source, ".tmpl", "", -1)
	result := cb.executeTemplate(source, file.AsciiFileToString(source))

	dest := strings.Replace(source, "templates/", "/Users/jrush/Development/trueblocks-core/", -1)
	dest = strings.Replace(dest, "_", "/", -1)

	logger.Info("Writing", dest)
	file.StringToAsciiFile(dest+".new", string(result))
	WriteOut(dest+".new", dest)
}

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
