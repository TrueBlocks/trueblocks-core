package types

import (
	"fmt"
	"strings"
	"text/template"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

type TemplateMap map[string]*template.Template

func shouldProcess(source, route string) (bool, error) {
	if strings.Contains(source, "sdk_") {
		if route == "explore" || route == "daemon" || route == "scrape" {
			return false, nil
		}
	}

	if !file.FileExists(source) {
		return false, fmt.Errorf("file does not exist %s", source)
	}

	return true, nil
}

func convertToDestPath(source, route string) string {
	dest := strings.Replace(source, templateFolder, "", -1)
	dest = strings.Replace(dest, ".tmpl", "", -1)
	dest = strings.Replace(dest, "_route_", "/"+route+"/", -1)
	dest = strings.Replace(dest, "__route", "_+route", -1)
	dest = strings.Replace(dest, "route.go", route+".go", -1)
	dest = strings.Replace(dest, "route.py", route+".py", -1)
	dest = strings.Replace(dest, "route.ts", route+".ts", -1)
	dest = strings.Replace(dest, "_", "/", -1)
	dest = strings.Replace(dest, "+", "_", -1)
	return dest
}

var templateFolder = "src/dev_tools/goMaker/templates"

func snakeCase(s string) string {
	result := ""
	toUpper := false
	for _, c := range s {
		if c == '_' {
			toUpper = true
			continue
		}
		if toUpper {
			result += strings.ToUpper(string(c))
			toUpper = false
		} else {
			result += string(c)
		}
	}
	return result
}
