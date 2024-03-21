package main

import (
	"bytes"
	"log"
	"os"
	"path/filepath"
	"strings"
	"text/template"

	"github.com/TrueBlocks/trueblocks-core/goMaker/codeWriter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (c *Command) processFile(source string) error {
	cwd, _ := os.Getwd()
	source = filepath.Join(cwd, templateFolder, source)
	if ok, err := shouldProcess(source, c.Route); err != nil {
		return err
	} else if !ok {
		return nil
	}

	result := c.executeTemplate(source, file.AsciiFileToString(source))

	dest := convertToDestPath(source, c.Route)
	logger.Info("Writing to: ", dest)
	return codeWriter.WriteCode(dest, result)
}

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

func (c *Command) PyOptions() string {
	ret := []string{}
	for _, op := range c.Options {
		if op.OptionType != "positional" && !op.Hidden() {
			code := "    \"{{.SnakeCase}}\": {\"hotkey\": \"{{.PyHotKey}}\", \"type\": \"{{.OptionType}}\"},"
			ret = append(ret, op.executeTemplate("pyoption", code))
		}
	}
	return strings.Join(ret, "\n")
}
