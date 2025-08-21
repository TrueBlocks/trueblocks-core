package types

import (
	"errors"
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"golang.org/x/text/cases"
	"golang.org/x/text/language"
)

var ErrNoTemplateFolder = errors.New("could not find the templates directory")

func shouldProcess(source, subPath, tag string) (bool, error) {
	single := os.Getenv("TB_MAKER_SINGLE")
	if single != "" && !strings.Contains(source, single) {
		// logger.Warn("skipping ", source, " because of ", single)
		return false, nil
	}

	if tag == "" {
		return false, nil
	}

	isSdk := strings.Contains(source, "sdk_")
	isExample := strings.Contains(source, "examples_")
	isPython := strings.Contains(source, "python")
	isTypeScript := strings.Contains(source, "typescript")
	isFuzzer := strings.Contains(source, "sdkFuzzer")
	switch tag {
	case "daemon":
		if isSdk || isFuzzer || isExample {
			return false, nil
		}
	case "scrape":
		if isFuzzer || isExample {
			return false, nil
		}
		fallthrough
	case "explore":
		if isSdk && (isPython || isTypeScript || isFuzzer) || isExample {
			return false, nil
		}
	}

	source = strings.ReplaceAll(source, "/templates/", "/templates/generators/"+subPath+"/")
	if !file.FileExists(source) {
		return false, fmt.Errorf("file does not exist %s", source)
	}

	return true, nil
}

func getGeneratorContents(fullPath, subPath, group, reason string) string {
	gPath := strings.ReplaceAll(fullPath, "/templates/", "/templates/generators/"+subPath+"/")
	if !file.FileExists(gPath) {
		logger.Fatal("Could not find generator file: ", gPath)
	}

	tmpl := file.AsciiFileToString(gPath)
	tmpl = strings.ReplaceAll(tmpl, "[{GROUP}]", group)
	tmpl = strings.ReplaceAll(tmpl, "[{REASON}]", reason)
	return tmpl
}

func convertToDestPath(source, routeTag, typeTag, groupTag, reason string) string {
	var singularWords = map[string]bool{
		"project": true,
		"history": true,
		"session": true,
		"config":  true,
		"wizard":  true,
	}
	viewName := func(s string) string {
		if singularWords[typeTag] {
			return Proper(s)
		}
		return Proper(Plural(typeTag))
	}

	dest := strings.ReplaceAll(source, GetTemplatePath(), "")
	dest = strings.ReplaceAll(dest, ".tmpl", "")
	dest = strings.ReplaceAll(dest, "_route_", "/"+routeTag+"/")
	dest = strings.ReplaceAll(dest, "route+internal", routeTag+"+internal")
	dest = strings.ReplaceAll(dest, "_Route_", "/"+Proper(routeTag)+"/")
	dest = strings.ReplaceAll(dest, "Route+internal", Proper(routeTag)+"+internal")
	reps := []string{".go", ".md", ".py", ".ts"}
	for _, rep := range reps {
		dest = strings.ReplaceAll(dest, "route"+rep, routeTag+rep)
		dest = strings.ReplaceAll(dest, "Route"+rep, Proper(routeTag)+rep)
	}
	dest = strings.ReplaceAll(dest, "+type+", "+"+typeTag+"+")
	dest = strings.ReplaceAll(dest, "_capType", "_"+viewName(typeTag))
	dest = strings.ReplaceAll(dest, "type+sort", typeTag+"+sort")
	dest = strings.ReplaceAll(dest, "type.go", typeTag+".go")
	dest = strings.ReplaceAll(dest, "type.md", typeTag+".md")
	dest = strings.ReplaceAll(dest, "type.ts", typeTag+".ts")
	dest = strings.ReplaceAll(dest, "group.md", groupTag+".md")
	switch reason {
	case "readme":
		dest = strings.ReplaceAll(dest, "_reason_", "_chifra_")
	case "model":
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

func getTemplatesPath() (string, error) {
	paths := []string{
		"src/dev_tools/goMaker/",
		"code_gen/",
	}

	attemptedPaths := []string{}

	for _, path := range paths {
		thePath := filepath.Join(path, "templates")
		if file.FolderExists(thePath) {
			setRootFolder(path)
			return thePath, nil
		}
		attemptedPaths = append(attemptedPaths, thePath)
	}

	return "", fmt.Errorf("%w [%s]", ErrNoTemplateFolder, strings.Join(attemptedPaths, ", "))
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
	if strings.HasSuffix(s, "s") || strings.HasSuffix(s, "ed") || strings.HasSuffix(s, "ing") {
		return s
	} else if strings.HasSuffix(s, "x") {
		return s + "es"
	} else if strings.HasSuffix(s, "y") {
		return s + "ies"
	} else if s == "config" || s == "session" || s == "publish" {
		return s
	}
	return s + "s"
}

func Proper(s string) string {
	titleCaser := cases.Title(language.English)
	return titleCaser.String(s)
}

func Singular(s string) string {
	sLower := strings.ToLower(s)
	if sLower == "addresses" {
		return s[:len(s)-2]
	}

	exclusions := []string{"baddress", "status", "stats", "series", "dalledress"}
	if !contains(exclusions, sLower) && strings.HasSuffix(sLower, "s") {
		return s[:len(s)-1]
	}

	return s
}

func Lower(s string) string {
	return strings.ToLower(s)
}

func Upper(s string) string {
	return strings.ToUpper(s)
}

// ws white space
var ws = "\n\r\t"

// wss white space with space
var wss = ws + " "
