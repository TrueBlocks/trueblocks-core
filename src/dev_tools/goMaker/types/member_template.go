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
func (m *Member) executeTemplate(name, tmplCode string) string {
	if m.templates == nil {
		m.templates = make(map[string]*template.Template)
	}

	if m.templates[name] == nil {
		toSnake := func(s string) string { return strings.ToLower(s[0:1]) + s[1:] }
		funcMap := template.FuncMap{"toSnake": toSnake}
		m.templates[name] = template.Must(template.New(name).Funcs(funcMap).Parse(tmplCode))
	}

	var tplBuffer bytes.Buffer
	if err := m.templates[name].Execute(&tplBuffer, m); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}

	return tplBuffer.String()
}
