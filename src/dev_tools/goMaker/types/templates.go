package types

import (
	"bytes"
	"log"
	"strings"
	"text/template"
)

var (
	codebaseCache map[string]*template.Template
)

func init() {
	codebaseCache = make(map[string]*template.Template)
}

func executeTemplate(receiver any, tmplPrefix, name, tmplCode string) string {
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
	toSingular := func(s string) string { return Singular(s) }
	toProper := func(s string) string { return Proper(s) }
	toPlural := func(s string) string { return Plural(s) }
	toCamel := func(s string) string { return CamelCase(s) }
	toLower := func(s string) string { return Lower(s) }
	toUpper := func(s string) string { return Upper(s) }
	firstLower := func(s string) string { return FirstLower(s) }
	firstUpper := func(s string) string { return FirstUpper(s) }
	toLowerPlural := func(s string) string { return Lower(Plural(s)) }
	or := func(a, b bool) bool { return a || b }
	sub := func(a, b int) int { return a - b }
	replace := func(str, find, rep string) string { return strings.ReplaceAll(str, find, rep) }
	cond := func(t bool, a, b any) any {
		if t {
			return a
		} else {
			return b
		}
	}
	apply := func(array []string, tmplStr, sep string) string {
		var ret string
		for i, item := range array {
			ret += executeTemplate(item, "apply", tmplStr, tmplStr)
			if i < len(array)-1 {
				ret += sep
			}
		}
		return ret
	}

	return template.FuncMap{
		"toSingular":    toSingular,
		"toProper":      toProper,
		"toCamel":       toCamel,
		"toPlural":      toPlural,
		"toLowerPlural": toLowerPlural,
		"toLower":       toLower,
		"toUpper":       toUpper,
		"firstLower":    firstLower,
		"firstUpper":    firstUpper,
		"or":            or,
		"sub":           sub,
		"replace":       replace,
		"cond":          cond,
		"apply":         apply,
	}
}
