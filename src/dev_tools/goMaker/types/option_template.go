package types

import (
	"bytes"
	"log"
	"strings"
	"text/template"
)

// executeTemplate executes the template with the given name and returns
// the result. It stores the parsed template in the templates map to avoid
// parsing it more than once.
func (op *Option) executeTemplate(name, tmplCode string) string {
	if op.templates == nil {
		op.templates = make(map[string]*template.Template)
	}

	if op.templates[name] == nil {
		toSnake := func(s string) string { return strings.ToLower(s[0:1]) + s[1:] }
		funcMap := template.FuncMap{"toSnake": toSnake}
		op.templates[name] = template.Must(template.New(name).Funcs(funcMap).Parse(tmplCode))
	}

	var tplBuffer bytes.Buffer
	if err := op.templates[name].Execute(&tplBuffer, op); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}
	return tplBuffer.String()
}
