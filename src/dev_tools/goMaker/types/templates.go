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
	tmplName := tmplPrefix + name

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
	toPlural := func(s string) string { return Plural(s) }
	toCamel := func(s string) string { return CamelCase(s) }
	return template.FuncMap{
		"toCamel":  toCamel,
		"toPlural": toPlural,
	}
}
