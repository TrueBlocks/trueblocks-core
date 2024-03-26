package types

import (
	"bytes"
	"log"
	"text/template"
)

// executeTemplate executes the template with the given name and returns
// the result. It stores the parsed template in the templates map to avoid
// parsing it more than once.
func (s *Member) executeTemplate(name, tmplCode string) string {
	if s.templates == nil {
		s.templates = make(map[string]*template.Template)
	}

	if s.templates[name] == nil {
		var err error
		s.templates[name], err = template.New(name).Parse(tmplCode)
		if err != nil {
			log.Fatalf("parsing template failed: %v", err)
		}
	}

	var tplBuffer bytes.Buffer
	if err := s.templates[name].Execute(&tplBuffer, s); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}

	return tplBuffer.String()
}
