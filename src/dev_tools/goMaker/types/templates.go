package types

import (
	"bytes"
	"log"
	"text/template"
)

var (
	codebaseCache map[string]*template.Template
)

func init() {
	codebaseCache = make(map[string]*template.Template)
}

func executeTemplate(receiver interface{}, tmplPrefix, name, tmplCode string) string {
	tmplName := tmplPrefix + " " + name

	if codebaseCache[tmplName] == nil {
		tmpl, err := template.New(tmplName).Funcs(getFuncMap()).Parse(tmplCode)
		if err != nil {
			log.Fatalf("parsing template failed: %v", err)
		}
		codebaseCache[tmplName] = template.Must(tmpl, nil)
	}

	var tplBuffer bytes.Buffer
	if err := codebaseCache[tmplName].Execute(&tplBuffer, receiver); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}
	return tplBuffer.String()
}

func getFuncMap() template.FuncMap {
	toProper := func(s string) string { return Proper(s) }
	toPlural := func(s string) string { return Plural(s) }
	toCamel := func(s string) string { return CamelCase(s) }
	toLower := func(s string) string { return Lower(s) }
	firstLower := func(s string) string { return FirstLower(s) }
	firstUpper := func(s string) string { return FirstUpper(s) }
	return template.FuncMap{
		"toProper":   toProper,
		"toCamel":    toCamel,
		"toPlural":   toPlural,
		"toLower":    toLower,
		"firstLower": firstLower,
		"firstUpper": firstUpper,
	}
}
