package types

import (
	"bytes"
	"log"
	"strings"
	"text/template"
)

var codebaseCache map[string]*template.Template

func (cb *CodeBase) executeTemplate(name, tmplCode string) string {
	if codebaseCache == nil {
		codebaseCache = make(map[string]*template.Template)
	}

	tmplName := "codebase" + name
	if codebaseCache[tmplName] == nil {
		toSnake := func(s string) string { return strings.ToLower(s[0:1]) + s[1:] }
		funcMap := template.FuncMap{"toSnake": toSnake}
		codebaseCache[tmplName] = template.Must(template.New(tmplName).Funcs(funcMap).Parse(tmplCode))
	}

	var tplBuffer bytes.Buffer
	if err := codebaseCache[tmplName].Execute(&tplBuffer, cb); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}
	return tplBuffer.String()
}

func (s *Structure) executeTemplate(name, tmplCode string) string {
	if codebaseCache == nil {
		codebaseCache = make(map[string]*template.Template)
	}

	tmplName := "structure" + name
	if codebaseCache[tmplName] == nil {
		toSnake := func(s string) string { return strings.ToLower(s[0:1]) + s[1:] }
		funcMap := template.FuncMap{"toSnake": toSnake}
		codebaseCache[tmplName] = template.Must(template.New(tmplName).Funcs(funcMap).Parse(tmplCode))
	}

	var tplBuffer bytes.Buffer
	if err := codebaseCache[tmplName].Execute(&tplBuffer, s); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}

	return tplBuffer.String()
}

func (m *Member) executeTemplate(name, tmplCode string) string {
	if codebaseCache == nil {
		codebaseCache = make(map[string]*template.Template)
	}

	tmplName := "member" + name
	if codebaseCache[tmplName] == nil {
		toSnake := func(s string) string { return strings.ToLower(s[0:1]) + s[1:] }
		funcMap := template.FuncMap{"toSnake": toSnake}
		codebaseCache[tmplName] = template.Must(template.New(tmplName).Funcs(funcMap).Parse(tmplCode))
	}

	var tplBuffer bytes.Buffer
	if err := codebaseCache[tmplName].Execute(&tplBuffer, m); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}

	return tplBuffer.String()
}

func (c *Command) executeTemplate(name, tmplCode string) string {
	if codebaseCache == nil {
		codebaseCache = make(map[string]*template.Template)
	}

	tmplName := "command" + name
	if codebaseCache[tmplName] == nil {
		toSnake := func(s string) string { return strings.ToLower(s[0:1]) + s[1:] }
		funcMap := template.FuncMap{"toSnake": toSnake}
		codebaseCache[tmplName] = template.Must(template.New(tmplName).Funcs(funcMap).Parse(tmplCode))
	}

	var tplBuffer bytes.Buffer
	if err := codebaseCache[tmplName].Execute(&tplBuffer, c); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}
	return tplBuffer.String()
}

func (op *Option) executeTemplate(name, tmplCode string) string {
	tmplName := "option" + name
	if codebaseCache == nil {
		codebaseCache = make(map[string]*template.Template, 50)
	}

	if codebaseCache[tmplName] == nil {
		toSnake := func(s string) string { return strings.ToLower(s[0:1]) + s[1:] }
		funcMap := template.FuncMap{"toSnake": toSnake}
		codebaseCache[tmplName] = template.Must(template.New(tmplName).Funcs(funcMap).Parse(tmplCode))
	}

	var tplBuffer bytes.Buffer
	if err := codebaseCache[tmplName].Execute(&tplBuffer, op); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}
	return tplBuffer.String()
}
