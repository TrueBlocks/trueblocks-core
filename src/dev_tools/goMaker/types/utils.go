package types

import (
	"errors"
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"golang.org/x/text/cases"
	"golang.org/x/text/language"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

var ErrNoTemplateFolder = errors.New("could not find the templates directory")

// TemplateMetadata holds metadata extracted from template files
type TemplateMetadata struct {
	Output string `yaml:"output"`
	Scope  string `yaml:"scope"`
}

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

// getGeneratorContentsAndDest processes a template file and returns both cleaned content and destination path
func getGeneratorContentsAndDest(fullPath, subPath, group, reason, routeTag, typeTag, groupTag string) (string, string) {
	gPath := strings.ReplaceAll(fullPath, "/templates/", "/templates/generators/"+subPath+"/")
	if !file.FileExists(gPath) {
		logger.Fatal("Could not find generator file: ", gPath)
	}

	tmpl := file.AsciiFileToString(gPath)
	if err := ValidateTemplate(tmpl, gPath); err != nil {
		logger.Fatal(err)
	}

	// Extract metadata first, before stripping it
	var dest string
	if metadata := parseMetadataBlock(tmpl, reason); metadata != nil {
		dest = processMetadataPath(metadata.Output, routeTag, typeTag, groupTag, reason)
	} else {
		logger.ShouldNotHappen("Old style templates should be gone.")
		// Fall back to filename-based conversion
		// dest = convertToDestPathLegacy(fullPath, routeTag, typeTag, groupTag, reason)
	}

	// Now strip metadata from template content
	tmpl = stripMetadata(tmpl)

	tmpl = strings.ReplaceAll(tmpl, "[{GROUP}]", group)
	tmpl = strings.ReplaceAll(tmpl, "[{REASON}]", reason)

	return tmpl, dest
}

// stripMetadata removes metadata block from template content and trims whitespace
func stripMetadata(content string) string {
	if !strings.HasPrefix(content, "/*\n") {
		return content
	}

	lines := strings.Split(content, "\n")
	if len(lines) < 3 {
		return content
	}

	// Find the end of the metadata block
	endIndex := -1
	for i := 1; i < len(lines); i++ {
		if strings.TrimSpace(lines[i]) == "*/" {
			endIndex = i
			break
		}
	}

	if endIndex == -1 {
		return content // No proper metadata block found
	}

	// Return content after metadata block, trimmed
	remaining := strings.Join(lines[endIndex+1:], "\n")
	return strings.TrimSpace(remaining) + "\n"
}

// parseMetadataBlock parses metadata from a comment block at the start of a template
func parseMetadataBlock(content, reason string) *TemplateMetadata {
	switch reason {
	case "readme":
		content = strings.ReplaceAll(content, "[[reason]]_", "chifra/")
	case "model":
		content = strings.ReplaceAll(content, "[[reason]]_", "data-model/")
	}

	lines := strings.Split(content, "\n")
	if len(lines) < 3 || lines[0] != "/*" {
		return nil
	}

	metadata := &TemplateMetadata{}
	for i := 1; i < len(lines); i++ {
		line := strings.TrimSpace(lines[i])
		if line == "*/" {
			break
		}

		if strings.HasPrefix(line, "output:") {
			metadata.Output = strings.TrimSpace(strings.TrimPrefix(line, "output:"))
		} else if strings.HasPrefix(line, "scope:") {
			metadata.Scope = strings.TrimSpace(strings.TrimPrefix(line, "scope:"))
		}
	}

	if metadata.Output == "" {
		return nil
	}
	return metadata
}

// processMetadataPath processes Go template variables in a metadata output path
func processMetadataPath(outputPath, routeTag, typeTag, groupTag, reason string) string {
	dest := outputPath

	dest = strings.ReplaceAll(dest, "[[Route]]", Proper(routeTag))
	dest = strings.ReplaceAll(dest, "[[Type]]", Proper(typeTag))
	dest = strings.ReplaceAll(dest, "[[Group]]", Proper(groupTag))
	dest = strings.ReplaceAll(dest, "[[Reason]]", Proper(reason))

	dest = strings.ReplaceAll(dest, "[[route]]", Lower(routeTag))
	dest = strings.ReplaceAll(dest, "[[type]]", Lower(typeTag))
	dest = strings.ReplaceAll(dest, "[[group]]", Lower(groupTag))
	dest = strings.ReplaceAll(dest, "[[reason]]", Lower(reason))

	return dest
}

// // convertToDestPathLegacy is the original filename-based conversion system
// func convertToDestPathLegacy(source, routeTag, typeTag, groupTag, reason string) string {
// 	singularWords := map[string]bool{
// 		"project": true,
// 		"history": true,
// 		"session": true,
// 		"config":  true,
// 		"wizard":  true,
// 	}
// 	viewName := func(s string) string {
// 		if singularWords[typeTag] {
// 			return Proper(s)
// 		}
// 		return Proper(Plural(typeTag))
// 	}

// 	dest := strings.ReplaceAll(source, GetTemplatePath(), "")
// 	dest = strings.ReplaceAll(dest, ".tmpl", "")
// 	dest = strings.ReplaceAll(dest, "_route_", "/"+routeTag+"/")
// 	dest = strings.ReplaceAll(dest, "route+internal", routeTag+"+internal")
// 	dest = strings.ReplaceAll(dest, "_Route_", "/"+Proper(routeTag)+"/")
// 	dest = strings.ReplaceAll(dest, "Route+internal", Proper(routeTag)+"+internal")
// 	reps := []string{".go", ".md", ".py", ".ts", ".test"}
// 	for _, rep := range reps {
// 		dest = strings.ReplaceAll(dest, "route"+rep, routeTag+rep)
// 		dest = strings.ReplaceAll(dest, "Route"+rep, Proper(routeTag)+rep)
// 	}
// 	dest = strings.ReplaceAll(dest, "+type+", "+"+typeTag+"+")
// 	dest = strings.ReplaceAll(dest, "_capType", "_"+viewName(typeTag))
// 	dest = strings.ReplaceAll(dest, "type+sort", typeTag+"+sort")
// 	dest = strings.ReplaceAll(dest, "type.go", typeTag+".go")
// 	dest = strings.ReplaceAll(dest, "type.md", typeTag+".md")
// 	dest = strings.ReplaceAll(dest, "type.ts", typeTag+".ts")
// 	dest = strings.ReplaceAll(dest, "group.md", groupTag+".md")
// 	switch reason {
// 	case "readme":
// 		dest = strings.ReplaceAll(dest, "_reason_", "_chifra_")
// 	case "model":
// 		dest = strings.ReplaceAll(dest, "_reason_", "_data-model_")
// 	}
// 	dest = strings.ReplaceAll(dest, "_", "/")
// 	dest = strings.ReplaceAll(dest, "+", "_")
// 	// Hack alert
// 	dest = strings.ReplaceAll(dest, "/src/apps/chifra/pkg/types/", "/src/apps/chifra/pkg/types/types_")
// 	return strings.ReplaceAll(dest, "//", "/")
// }

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
	content := file.AsciiFileToString(fn)
	if err := ValidateTemplate(content, fn); err != nil {
		logger.Fatal(err)
	}
	return content
}

// ValidateTemplate validates that EXISTING_CODE markers are properly formatted
func ValidateTemplate(content, templatePath string) error {
	lines := strings.Split(content, "\n")
	existingCodeCount := 0

	for _, line := range lines {
		if strings.Contains(line, "// EXISTING_CODE") {
			// Check rule 1: Line can only contain whitespace and // EXISTING_CODE
			// trimmed := strings.TrimSpace(line)
			// if trimmed != "// EXISTING_CODE" {
			// 	return fmt.Errorf("line %d in template %s contains '// EXISTING_CODE' but has other non-whitespace characters: %s",
			// 		i+1, templatePath, line)
			// }
			existingCodeCount++
		}
	}

	// Check rule 2: Must have even number of EXISTING_CODE markers (including zero)
	if existingCodeCount%2 != 0 {
		return fmt.Errorf("template %s has %d '// EXISTING_CODE' markers, but must have an even number",
			templatePath, existingCodeCount)
	}

	return nil
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
