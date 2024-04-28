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
		if tag == "explore" || tag == "scrape" {
			return false, nil
		}
		if tag == "daemon" && strings.Contains(source, "python") {
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

func convertToDestPath(source, routeTag, typeTag, groupTag, reason string) string {
	dest := strings.Replace(source, templateFolder, "", -1)
	dest = strings.Replace(dest, ".tmpl", "", -1)
	dest = strings.Replace(dest, "_route_", "/"+routeTag+"/", -1)
	dest = strings.Replace(dest, "route+internal", routeTag+"+internal", -1)
	dest = strings.Replace(dest, "route.go", routeTag+".go", -1)
	dest = strings.Replace(dest, "route.md", routeTag+".md", -1)
	dest = strings.Replace(dest, "route.py", routeTag+".py", -1)
	dest = strings.Replace(dest, "route.ts", routeTag+".ts", -1)
	dest = strings.Replace(dest, "type.go", typeTag+".go", -1)
	dest = strings.Replace(dest, "type.md", typeTag+".md", -1)
	dest = strings.Replace(dest, "group.md", groupTag+".md", -1)
	if reason == "readme" {
		dest = strings.Replace(dest, "_reason_", "_chifra_", -1)
	} else if reason == "model" {
		dest = strings.Replace(dest, "_reason_", "_data-model_", -1)
	}
	dest = strings.ReplaceAll(dest, "_", "/")
	dest = strings.ReplaceAll(dest, "+", "_")
	return strings.Replace(dest, "//", "/", -1)
}

var templateFolder = "src/dev_tools/goMaker/templates"

func LowerNoSpaces(s string) string {
	return strings.ToLower(strings.Replace(s, " ", "", -1))
}

func GoName(s string) string {
	return FirstUpper(CamelCase(s))
}

func CamelCase(s string) string {
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

// ws white space
var ws = "\n\r\t"

// wss white space with space
var wss = ws + " "
