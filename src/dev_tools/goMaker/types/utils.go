package types

import (
	"fmt"
	"os"
	"strings"
	"text/template"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

type TemplateMap map[string]*template.Template

func shouldProcess(source, tag string) (bool, error) {
	single := os.Getenv("TB_MAKER_SINGLE")
	if single != "" && !strings.Contains(source, single) {
		// logger.Warn("skipping ", source, " because of ", single)
		return false, nil
	}

	if tag == "" {
		return false, nil
	}

	if strings.Contains(source, "sdk_") {
		if tag == "explore" || tag == "daemon" || tag == "scrape" {
			return false, nil
		}
	}

	if strings.HasPrefix(source, "docs_") && tag == "codebase" {
		if tag == "explore" || tag == "daemon" {
			return false, nil
		}
	}

	if !file.FileExists(source) {
		return false, fmt.Errorf("file does not exist %s", source)
	}

	return true, nil
}

func convertToDestPath(source, routeTag, typeTag string) string {
	dest := strings.Replace(source, templateFolder, "", -1)
	dest = strings.Replace(dest, ".tmpl", "", -1)
	dest = strings.Replace(dest, "_type.go", "/"+typeTag+".go", -1)
	dest = strings.Replace(dest, "_route_", "/"+routeTag+"/", -1)
	dest = strings.Replace(dest, "__route", "_+route", -1)
	dest = strings.Replace(dest, "route.go", routeTag+".go", -1)
	dest = strings.Replace(dest, "route.py", routeTag+".py", -1)
	dest = strings.Replace(dest, "route.ts", routeTag+".ts", -1)
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
