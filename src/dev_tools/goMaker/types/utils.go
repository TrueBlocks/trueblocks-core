package types

import (
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"golang.org/x/text/cases"
	"golang.org/x/text/language"
)

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
	dest = strings.Replace(dest, "_type.md", "/"+typeTag+".md", -1)
	dest = strings.Replace(dest, "_route_", "/"+routeTag+"/", -1)
	dest = strings.Replace(dest, "route.go", routeTag+".go", -1)
	dest = strings.Replace(dest, "route.md", routeTag+".md", -1)
	dest = strings.Replace(dest, "route.py", routeTag+".py", -1)
	dest = strings.Replace(dest, "route.ts", routeTag+".ts", -1)
	dest = strings.Replace(dest, "_", "/", -1)
	dest = strings.Replace(dest, "+", "_", -1)
	return dest
}

var templateFolder = "src/dev_tools/goMaker/templates"

func SnakeCase(s string) string {
	if len(s) < 2 {
		return s
	}

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
	return strings.Replace(strings.ToLower(result[0:1])+result[1:], " ", "", -1)
}

func Pad(s string, width int) string {
	return s + strings.Repeat(" ", width-len(s))
}

func FirstUpper(s string) string {
	if len(s) == 0 {
		return s
	}
	return strings.ToUpper(s[0:1]) + s[1:]
}

func FirstLower(s string) string {
	if len(s) == 0 {
		return s
	}
	return strings.ToLower(s[0:1]) + s[1:]
}

func Plural(s string) string {
	if strings.HasSuffix(s, "s") {
		return s
	}
	return s + "s"
}

func Proper(s string) string {
	titleCaser := cases.Title(language.English)
	return titleCaser.String(s)
}

func Lower(s string) string {
	return strings.ToLower(s)
}
