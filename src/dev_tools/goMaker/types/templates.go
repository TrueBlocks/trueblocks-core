package types

import (
	"bytes"
	"fmt"
	"regexp"
	"strings"
	"text/template"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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
			logger.Fatalf("parsing template failed: %v", err)
		}
		codebaseCache[tmplName] = template.Must(tmpl, nil)
	}

	var tplBuffer bytes.Buffer
	if err := codebaseCache[tmplName].Execute(&tplBuffer, receiver); err != nil {
		logger.Fatalf("executing template failed: %v", err)
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
	split := func(s string, k string) []string { return strings.Split(s, k) }
	hasPrefix := func(s, prefix string) bool { return strings.HasPrefix(s, prefix) }
	contains := func(s, substr string) bool { return strings.Contains(s, substr) }
	trim := func(s string) string { return strings.TrimSpace(s) }
	or := func(a, b bool) bool { return a || b }
	add := func(a, b int) int { return a + b }
	sub := func(a, b int) int { return a - b }
	min := func(a, b int) int { return min(a, b) }
	max := func(a, b int) int { return max(a, b) }
	append := func(existing []string, add string) []string { return append(existing, add) }
	replace := func(str, find, rep string) string { return strings.ReplaceAll(str, find, rep) }
	hotkey := func(n int) string {
		var key string
		if n == 10 {
			key = "0"
		} else {
			key = fmt.Sprintf("%d", n%10)
		}
		var hotkey, altHotkey string
		if n > 10 {
			hotkey = fmt.Sprintf("mod+shift+%s", key)
			altHotkey = fmt.Sprintf("alt+shift+%s", key)
		} else {
			hotkey = fmt.Sprintf("mod+%s", key)
			altHotkey = fmt.Sprintf("alt+%s", key)
		}
		return fmt.Sprintf("hotkey: '%s',\n    altHotkey: '%s',", hotkey, altHotkey)
	}
	toHeader := func(s string) string {
		var words []string
		start := 0
		for i, r := range s {
			if i > 0 && r >= 'A' && r <= 'Z' {
				words = append(words, s[start:i])
				start = i
			}
		}
		if start < len(s) {
			words = append(words, s[start:])
		}
		for i, word := range words {
			if len(word) > 0 {
				words[i] = strings.ToUpper(word[:1]) + strings.ToLower(word[1:])
			}
		}
		if len(words) > 0 && (words[0] == "N" || words[0] == "Is") {
			words = words[1:]
		}
		return strings.Join(words, " ")
	}
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
	regexCompile := func(pattern string) *regexp.Regexp {
		re, err := regexp.Compile(pattern)
		if err != nil {
			logger.Fatal(err)
		}
		return re
	}

	regexReplace := func(re *regexp.Regexp, input, replacement string) string {
		return re.ReplaceAllString(input, replacement)
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
		"add":           add,
		"sub":           sub,
		"replace":       replace,
		"cond":          cond,
		"apply":         apply,
		"split":         split,
		"trim":          trim,
		"hasPrefix":     hasPrefix,
		"contains":      contains,
		"regexCompile":  regexCompile,
		"regexReplace":  regexReplace,
		"toHeader":      toHeader,
		"append":        append,
		"min":           min,
		"max":           max,
		"hotkey":        hotkey,
	}
}
