package types

import (
	"bytes"
	"log"
	"strings"
	"text/template"
)

func (cb *CodeBase) executeTemplate(name, tmplCode string) string {
	if cb.templates == nil {
		cb.templates = make(map[string]*template.Template)
	}

	if cb.templates[name] == nil {
		toSnake := func(s string) string { return strings.ToLower(s[0:1]) + s[1:] }
		funcMap := template.FuncMap{"toSnake": toSnake}
		cb.templates[name] = template.Must(template.New(name).Funcs(funcMap).Parse(tmplCode))
	}

	var tplBuffer bytes.Buffer
	if err := cb.templates[name].Execute(&tplBuffer, cb); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}
	return tplBuffer.String()
}

func (s *Structure) executeTemplate(name, tmplCode string) string {
	if s.templates == nil {
		s.templates = make(map[string]*template.Template)
	}

	if s.templates[name] == nil {
		toSnake := func(s string) string { return strings.ToLower(s[0:1]) + s[1:] }
		funcMap := template.FuncMap{"toSnake": toSnake}
		s.templates[name] = template.Must(template.New(name).Funcs(funcMap).Parse(tmplCode))
	}

	var tplBuffer bytes.Buffer
	if err := s.templates[name].Execute(&tplBuffer, s); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}

	return tplBuffer.String()
}

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

func (c *Command) executeTemplate(name, tmplCode string) string {
	if c.templates == nil {
		c.templates = make(map[string]*template.Template)
	}

	if c.templates[name] == nil {
		toSnake := func(s string) string { return strings.ToLower(s[0:1]) + s[1:] }
		funcMap := template.FuncMap{"toSnake": toSnake}
		c.templates[name] = template.Must(template.New(name).Funcs(funcMap).Parse(tmplCode))
	}

	var tplBuffer bytes.Buffer
	if err := c.templates[name].Execute(&tplBuffer, c); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}
	return tplBuffer.String()
}

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
