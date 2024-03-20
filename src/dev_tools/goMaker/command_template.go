package main

import (
	"bytes"
	"log"
	"strings"
	"text/template"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (c *Command) processFile(source string) {
	if !shouldProcess(source, c.Route) {
		return
	}

	result := c.executeTemplate(source, file.AsciiFileToString(source))

	dest := strings.Replace(source, "templates/", "/Users/jrush/Development/trueblocks-core/", -1)
	dest = strings.Replace(dest, "_route_", "/"+c.Route+"/", -1)
	dest = strings.Replace(dest, "__route", "_+route", -1)
	dest = strings.Replace(dest, "route.go", c.Route+".go", -1)
	dest = strings.Replace(dest, "route.py", c.Route+".py", -1)
	dest = strings.Replace(dest, "route.ts", c.Route+".ts", -1)
	dest = strings.Replace(dest, "_", "/", -1)
	dest = strings.Replace(dest, "+", "_", -1)

	logger.Info("Writing", dest)
	file.StringToAsciiFile(dest+".new", string(result))
	WriteOut(dest+".new", dest)
}

func shouldProcess(source, route string) bool {
	if strings.Contains(source, "templates/sdk") {
		if route == "explore" || route == "daemon" || route == "scrape" {
			return false
		}
	}
	return true
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
