package main

import (
	"bytes"
	"encoding/csv"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"net/url"
	"os"
	"path/filepath"
	"regexp"
	"strings"
	"text/template"
	"unicode"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func init() {
	os.Setenv("NO_USERQUERY", "true")
}

func main() {
	if err := isValidEnvironment(); err != nil {
		logger.Fatal(err)
		os.Exit(1)
	}

	if err := startApiServer(); err != nil {
		logger.Fatal(err)
	}

	if testMap, casesPath, err := loadTestCases(); err != nil {
		logger.Fatal(err)
	} else {
		if err := downloadAbis(); err != nil {
			logger.Fatal(err)
		}

		routeList, modeList := getRoutesAndModes()
		for _, mode := range modeList {
			// clean up old log files, they will be replaced
			os.Remove(getLogFile(mode))
		}

		summary := NewSummary()
		for _, item := range routeList {
			source := casesPath + item + ".csv"
			for _, mode := range modeList {
				tr := NewRunner(testMap, item, mode, source)
				for _, testCase := range testMap[source] {
					if err := tr.Run(&testCase); err != nil {
						logger.Error(err) // continue even on goLang error
					}
				}
				tr.ReportOneMode()
				summary.NTested += tr.NTested
				summary.NPassed += tr.NPassed
				summary.Fails = append(summary.Fails, tr.Fails...)
			}
		}

		summary.ReportFinal()
		if summary.NPassed != summary.NTested {
			os.Exit(1)
		}
	}
}

func parseCsv(filePath string) ([]TestCase, error) {
	ff, err := os.Open(filePath)
	if err != nil {
		return []TestCase{}, err
	}
	defer ff.Close()

	reader := csv.NewReader(ff)
	const requiredFields = 9
	lineNumber := 0
	testCases := make([]TestCase, 0, 200)
	for {
		lineNumber++
		csvRecord, err := reader.Read()
		if err != nil {
			if err == io.EOF {
				break // End of file reached
			}
			if !strings.Contains(err.Error(), "wrong number of fields") {
				fmt.Printf("Error at %s:%d %v\n", filePath, lineNumber, err)
				return []TestCase{}, err
			}
			continue
		}

		if len(csvRecord) == 0 || (len(csvRecord[0]) > 0 && csvRecord[0][0] == '#') {
			continue
		}
		if len(csvRecord) != requiredFields {
			continue
		}

		if !strings.HasPrefix(csvRecord[3], "route") {
			rec := record{
				Enabled:  strings.Trim(csvRecord[0], " "),
				Mode:     strings.Trim(csvRecord[1], " "),
				Speed:    strings.Trim(csvRecord[2], " "),
				Route:    strings.Trim(csvRecord[3], " "),
				Path:     strings.Trim(csvRecord[4], " "),
				Tool:     strings.Trim(csvRecord[5], " "),
				Filename: strings.Trim(csvRecord[6], " "),
				Post:     strings.Trim(csvRecord[7], " "),
				Options:  strings.Trim(csvRecord[8], " "),
			}

			testCase := TestCase{
				record:       rec,
				IsEnabled:    rec.Enabled == "on",
				HasShorthand: strings.Contains(rec.Options, "@"),
				WorkingPath:  filepath.Join(getWorkingPath(), rec.Path, rec.Tool) + "/",
				OrigOptions:  rec.Options,
				SourceFile:   filePath,
			}

			// order matters
			testCase.ApiOptions = testCase.cleanForApi()
			testCase.CmdOptions = testCase.cleanForCmd()
			testCase.SdkOptions = testCase.cleanForSdk()
			testCase.OptionArray = strings.Split(testCase.ApiOptions, "&")
			testCase.SdkOptionsArray = strings.Split(strings.Replace(testCase.SdkOptions, "%20", " ", -1), "&")
			testCase.ProcessRedirFile()
			// order matters

			testCases = append(testCases, testCase)
		}
	}
	return testCases, nil
}

func executeTemplate(color, tmplName, tmplCode string, data interface{}) string {
	padRight := func(str string, len int, pad string) string { return padRight(str, len, false, pad) }
	funcMap := template.FuncMap{
		"padRight": padRight,
	}
	parsed, err := template.New(tmplName).Funcs(funcMap).Parse(tmplCode)
	if err != nil {
		log.Fatalf("parsing template failed: %v", err)
	}
	var tplBuffer bytes.Buffer
	if err := parsed.Execute(&tplBuffer, &data); err != nil {
		log.Fatalf("executing template failed: %v", err)
	}

	// defer func() {
	// 	colors.ColorsOff()
	// }()
	// colors.ColorsOn()
	return color + tplBuffer.String() + colors.Off + strings.Repeat(" ", 135-len(tplBuffer.String()))
}

func (t *TestCase) clean(removes string) string {
	ret := t.OrigOptions
	ret = regexp.MustCompile(`\s*@\s*`).ReplaceAllString(ret, "&")
	ret = regexp.MustCompile(`\s*&\s*`).ReplaceAllString(ret, "&")
	ret = regexp.MustCompile(`\s*=\s*`).ReplaceAllString(ret, "=")
	ret = regexp.MustCompile(`\s+`).ReplaceAllString(ret, "%20")
	ret = strings.ReplaceAll(ret, ":", "%3A")
	cleanURL := strings.Join(strings.Fields(ret), "")
	if parsedURL, err := url.Parse("http://localhost?" + cleanURL); err != nil {
		logger.Error(err)
		return ""

	} else {
		phrases := []string{}
		for _, part := range strings.Split(parsedURL.RawQuery, "&") {
			kv := strings.SplitN(part, "=", 2)
			key := kv[0]
			value := ""
			if len(kv) > 1 {
				value = kv[1]
			}
			if strings.Contains(removes, key) {
				continue
			} else if strings.Contains(key, "_") {
				key = toCamelCase(key)
			}
			if value == "" {
				phrases = append(phrases, key)
			} else {
				phrases = append(phrases, key+"="+value)
			}
		}
		return strings.ReplaceAll(strings.Join(phrases, "&"), "%3A", ":")
	}
}

func (t *TestCase) cleanForSdk() string {
	return t.clean("help,wei,fmt,version,noop,nocolor,no_header,file")
}

func (t *TestCase) cleanForApi() string {
	return t.clean("")
}

func (t *TestCase) cleanForCmd() string {
	apiOptions := t.clean("")
	options := strings.Split(apiOptions, "&")
	removes := []string{
		"addrs", "blocks", "transactions", "modes", "terms", "addrs", "mode", "topics", "fourbytes",
	}

	ret := []string{}
	for _, option := range options {
		op := parseAndConvert(option)
		if len(op) > 0 {
			op = strings.ReplaceAll(op, "%20", " ")
			hack := "vbe,avt,"
			if len(op) < 3 || strings.Contains(hack, op+",") {
				op = "-" + op
			} else {
				op = "--" + strings.ReplaceAll(op, "=", " ")
			}
			for _, remove := range removes {
				op = strings.ReplaceAll(op, "--"+remove+" ", "")
				op = strings.ReplaceAll(op, "--"+remove, "")
			}
			op = strings.ReplaceAll(op, "*", "\\*")
		}
		ret = append(ret, op)
	}
	return strings.Join(ret, " ")
}

func toCamelCase(s string) string {
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

func toSnakeCase(str string) string {
	var result []rune
	for i, r := range str {
		if unicode.IsUpper(r) && i > 0 {
			result = append(result, '_')
		}
		result = append(result, unicode.ToLower(r))
	}
	return string(result)
}

func parseAndConvert(input string) string {
	result := []string{}
	pairs := strings.Split(input, "&")
	for _, pair := range pairs {
		kv := strings.SplitN(pair, "=", 2)
		key := strings.TrimSpace(kv[0])
		convertedKey := toSnakeCase(key)
		if len(kv) == 1 {
			result = append(result, convertedKey)
		} else if len(kv) == 2 {
			value := strings.TrimSpace(kv[1])
			result = append(result, convertedKey+"="+value)
		}
	}
	return strings.Join(result, "&")
}

func padRight(str string, length int, bumpPad bool, pad string) string {
	if len(str) < length {
		str += strings.Repeat(pad, length-len(str))
	} else {
		str = str[:length]
	}

	if bumpPad && length >= 3 {
		str = str[:length-3] + strings.Repeat(string(pad), 3)
	}
	return str
}

func toJson(structure interface{}) string {
	var buf bytes.Buffer
	encoder := json.NewEncoder(&buf)
	encoder.SetEscapeHTML(false)
	encoder.SetIndent("", "  ")
	err := encoder.Encode(structure)
	if err != nil {
		fmt.Println("Error encoding JSON:", err)
		return ""
	}
	return buf.String()
}

func getRepoRoot() string {
	wd, _ := os.Getwd()
	if !strings.HasSuffix(wd, "/build") {
		logger.Fatal("testRunner must be run from the build folder, not from", wd)
	}
	return filepath.Join(wd, "../")
}

func getCasesPath() string {
	return filepath.Join(getRepoRoot(), "src/dev_tools/testRunner/testCases") + "/"
}

func getGeneratedPath() string {
	return filepath.Join(getRepoRoot(), "src/dev_tools/testRunner/generated") + "/"
}

func getWorkingPath() string {
	return filepath.Join(getRepoRoot(), "tests/working") + "/"
}

func getTempFilePath(goldFn string) string {
	return filepath.Join(getRepoRoot(), "build", filepath.Base(goldFn)) + ".tmp"
}

func getLogFile(mode string) string {
	return getGeneratedPath() + "test_" + mode + ".log"
}

func loadTestCases() (map[string][]TestCase, string, error) {
	testMap := make(map[string][]TestCase, 100)
	walkFunc := func(path string, info os.FileInfo, err error) error {
		if err == nil {
			if !info.IsDir() && strings.HasSuffix(path, ".csv") {
				testCases, err := parseCsv(path)
				if err != nil {
					return err
				}
				for _, testCase := range testCases {
					source := testCase.SourceFile
					if _, ok := testMap[source]; !ok {
						testMap[source] = []TestCase{}
					}
					testMap[source] = append(testMap[source], testCase)
				}
			}
		}
		return err
	}

	casesPath := getCasesPath()
	if err := filepath.Walk(casesPath, walkFunc); err != nil {
		return testMap, casesPath, fmt.Errorf("error walking the path %q: %v", casesPath, err)
	}

	file.StringToAsciiFile(getGeneratedPath()+"testCases.json", toJson(testMap))
	return testMap, casesPath, nil
}
