package types

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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

	skip := tag == "explore" || tag == "scrape" || tag == "daemon"
	isSdk := strings.Contains(source, "sdk_")
	isFuzzer := strings.Contains(source, "sdkFuzzer")
	if skip && (isSdk || isFuzzer) {
		if tag == "scrape" {
			isPython := strings.Contains(source, "python")
			isTypeScript := strings.Contains(source, "typescript")
			if isPython || isTypeScript || isFuzzer {
				// do not generate code for scrape for Python or TypeScript SDKs
				return false, nil
			}
		} else {
			// do not generate code for daemon or explore for SDK
			return false, nil
		}
	}

	source = strings.ReplaceAll(source, "/templates/", "/templates/generators/")
	if !file.FileExists(source) {
		return false, fmt.Errorf("file does not exist %s", source)
	}

	return true, nil
}

func getGeneratorContents(fullPath, group, reason string) string {
	gPath := strings.ReplaceAll(fullPath, "/templates/", "/templates/generators/")
	if !file.FileExists(gPath) {
		logger.Fatal("Could not find generator file: ", gPath)
	}

	tmpl := file.AsciiFileToString(gPath)
	tmpl = strings.ReplaceAll(tmpl, "[{GROUP}]", group)
	tmpl = strings.ReplaceAll(tmpl, "[{REASON}]", reason)
	return tmpl
}

func convertToDestPath(source, routeTag, typeTag, groupTag, reason string) string {
	dest := strings.ReplaceAll(source, GetTemplatePath(), "")
	dest = strings.ReplaceAll(dest, ".tmpl", "")
	dest = strings.ReplaceAll(dest, "_route_", "/"+routeTag+"/")
	dest = strings.ReplaceAll(dest, "route+internal", routeTag+"+internal")
	dest = strings.ReplaceAll(dest, "route.go", routeTag+".go")
	dest = strings.ReplaceAll(dest, "route.md", routeTag+".md")
	dest = strings.ReplaceAll(dest, "route.py", routeTag+".py")
	dest = strings.ReplaceAll(dest, "route.ts", routeTag+".ts")
	dest = strings.ReplaceAll(dest, "type.go", typeTag+".go")
	dest = strings.ReplaceAll(dest, "type.md", typeTag+".md")
	dest = strings.ReplaceAll(dest, "type.ts", typeTag+".ts")
	dest = strings.ReplaceAll(dest, "group.md", groupTag+".md")
	if reason == "readme" {
		dest = strings.ReplaceAll(dest, "_reason_", "_chifra_")
	} else if reason == "model" {
		dest = strings.ReplaceAll(dest, "_reason_", "_data-model_")
	}
	dest = strings.ReplaceAll(dest, "_", "/")
	dest = strings.ReplaceAll(dest, "+", "_")
	// Hack alert
	dest = strings.ReplaceAll(dest, "/src/apps/chifra/pkg/types/", "/src/apps/chifra/pkg/types/types_")
	return strings.ReplaceAll(dest, "//", "/")
}

var rootFolder = "src/dev_tools/goMaker/"

func getRootFolder() string {
	return filepath.Join(rootFolder)
}

func setRootFolder(folder string) {
	rootFolder = folder
}

func GetTemplatePath() string {
	return filepath.Join(getRootFolder(), "templates/")
}

func getTemplateContents(fnIn string) string {
	fn := filepath.Join(GetTemplatePath(), fnIn+".md")
	return file.AsciiFileToString(fn)
}

func GetGeneratedPath() string {
	return filepath.Join(getRootFolder(), "generated/")
}

func getGeneratedContents(fnIn string) string {
	fn := filepath.Join(GetGeneratedPath(), fnIn+".md")
	return file.AsciiFileToString(fn)
}

func LowerNoSpaces(s string) string {
	return strings.ToLower(strings.ReplaceAll(s, " ", ""))
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
	return strings.ReplaceAll(strings.ToLower(result[0:1])+result[1:], " ", "")
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

func Singular(s string) string {
	if s == "Addresses" {
		return "Address"
	}

	if s != "Status" && s != "Stats" && strings.HasSuffix(s, "s") {
		return s[:len(s)-1]
	}

	return s
}

func Lower(s string) string {
	return strings.ToLower(s)
}

// ws white space
var ws = "\n\r\t"

// wss white space with space
var wss = ws + " "
