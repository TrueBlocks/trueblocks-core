package types

import (
	"bytes"
	"log"
	"text/template"
)

// executeTemplate executes the template with the given name and returns
// the result. It stores the parsed template in the templates map to avoid
// parsing it more than once.
func (op *CmdLineOption) executeTemplate(name, tmplCode string) string {
	if op.templates == nil {
		op.templates = make(map[string]*template.Template)
	}

	if op.templates[name] == nil {
		var err error
		op.templates[name], err = template.New(name).Parse(tmplCode)
		if err != nil {
			log.Fatalf("parsing template failed: %v", err)
		}
	}

	var tplBuffer bytes.Buffer
	if err := op.templates[name].Execute(&tplBuffer, op); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}
	return tplBuffer.String()
}
