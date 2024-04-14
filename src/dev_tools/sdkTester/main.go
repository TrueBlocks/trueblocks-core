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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

var debugging = false

func main() {
	testMap := make(map[string][]TestCase, 100)
	walkFunc := func(path string, info os.FileInfo, err error) error {
		if err == nil {
			if !info.IsDir() && strings.HasSuffix(path, ".csv") {
				testCases := collectCsvFiles(path)
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

	thePath := "../src/dev_tools/testRunner/testCases"
	if debugging {
		thePath = "../testRunner/testCases"
	}

	if err := filepath.Walk(thePath, walkFunc); err != nil {
		fmt.Printf("error walking the path %q: %v\n", thePath, err)
	}

	bytes, err := json.MarshalIndent(testMap, "", "  ")
	if err != nil {
		fmt.Printf("Error marshalling testCases: %v\n", err)
	}
	file.StringToAsciiFile("../src/dev_tools/sdkTester/generated/testCases.json", string(bytes))

	DoSdkTests(testMap)
}

type record struct {
	Enabled  string `json:"enabled"`
	Mode     string `json:"mode"`
	Speed    string `json:"speed"`
	Route    string `json:"route"`
	PathTool string `json:"pathTool"`
	Filename string `json:"filename"`
	Post     string `json:"post"`
	Options  string `json:"options"`
}

type TestCase struct {
	record
	IsEnabled    bool     `json:"isEnabled"`
	HasShorthand bool     `json:"hasShorthand"`
	GoldPath     string   `json:"goldPath"`
	WorkingPath  string   `json:"workingPath"`
	Cannonical   string   `json:"cannonical"`
	OptionArray  []string `json:"options"`
	SourceFile   string   `json:"sourceFile"`
}

var cm = map[string]string{
	"greenCheck":    "\033[32m✓\033[0m",
	"yellowCaution": "\033[33m!!\033[0m",
	"redX":          "\033[31mX\033[0m",
	"whiteStar":     "\033[37m*\033[0m",
}

func collectCsvFiles(filePath string) []TestCase {
	ff, err := os.Open(filePath)
	if err != nil {
		fmt.Printf("Error opening file: %v\n", err)
		return []TestCase{}
	}
	defer ff.Close()

	reader := csv.NewReader(ff)
	const requiredFields = 8
	lineNumber := 0

	testCases := make([]TestCase, 0, 200)
	for {
		lineNumber++
		csvRecord, err := reader.Read()
		if err != nil {
			if err == io.EOF {
				break // End of file reached
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
				PathTool: strings.Trim(csvRecord[4], " "),
				Filename: strings.Trim(csvRecord[5], " "),
				Post:     strings.Trim(csvRecord[6], " "),
				Options:  strings.Trim(csvRecord[7], " "),
			}

			cannon := canonicalizeURL(rec.Options)
			testCase := TestCase{
				record:       rec,
				IsEnabled:    rec.Enabled == "on",
				HasShorthand: strings.Contains(rec.Options, "@"),
				GoldPath:     "../test/gold/" + rec.PathTool + "/sdk_tests/",
				WorkingPath:  "../test/working/" + rec.PathTool + "/sdk_tests/",
				Cannonical:   cannon,
				OptionArray:  strings.Split(strings.Replace(cannon, "%20", " ", -1), "&"),
				SourceFile:   filePath,
			}
			testCases = append(testCases, testCase)
		}
	}
	return testCases
}

type TestRun struct {
	Route  string `json:"route"`
	Mode   string `json:"mode"`
	Passed int    `json:"passed"`
	Total  int    `json:"total"`
	Msg    string `json:"msg"`
}

func (r *TestRun) PaddedRoute() string {
	return padRight(r.Route, 20, false, '.')
}

var order = []string{
	"slurp",
	"names",
	"blocks",
	"logs",
	"receipts",
	"state",
	"tokens",
	"traces",
	"transactions",
	"abis",
	"when",
	"list",
	"monitors",
	"export",
	"scrape",
	"status",
	"chunks",
	"chifra",
	"config",
	"explore",
	"init",
	"daemon",
}

func DoSdkTests(testMap map[string][]TestCase) {
	for _, item := range order {
		source := "../src/dev_tools/testRunner/testCases/" + item + ".csv"
		nTested, nPassed := 0, 0
		filtered := []TestCase{}
		for _, testCase := range testMap[source] {
			if !strings.HasSuffix(testCase.SourceFile, item+".csv") || testCase.HasShorthand {
				continue
			}
			filtered = append(filtered, testCase)
		}

		for i, testCase := range filtered {
			tested, passed := testCase.RunSdkTest(i, len(filtered))
			if tested {
				nTested++
			}
			if passed {
				nPassed++
			}
		}

		msg := cm["greenCheck"]
		if nTested != nPassed {
			msg = cm["redX"]
		}

		run := TestRun{
			item,
			"sdk",
			nTested,
			nPassed,
			msg,
		}

		tmplCode := "  {{.PaddedRoute}} {{.Mode}} {{.Passed}} of {{.Total}} tests. {{.Msg}}"
		if run.Total == 0 {
			tmplCode = "  {{.PaddedRoute}} {{.Mode}} No tests found."
		}
		tmplParsed, err := template.New("sdk").Parse(tmplCode)
		if err != nil {
			log.Fatalf("parsing template failed: %v", err)
		}
		tmpl := template.Must(tmplParsed, nil)
		var tplBuffer bytes.Buffer
		if err := tmpl.Execute(&tplBuffer, &run); err != nil {
			log.Fatalf("executing template failed: %v", err)
		}
		fmt.Println(colors.White, tplBuffer.String(), colors.Off, strings.Repeat(" ", utils.Max(0, 90)))
	}
}

func preClean(rawURL string) string {
	rawURL = regexp.MustCompile(`\s*&\s*`).ReplaceAllString(rawURL, "&")
	rawURL = regexp.MustCompile(`\s*=\s*`).ReplaceAllString(rawURL, "=")
	rawURL = regexp.MustCompile(`\s+`).ReplaceAllString(rawURL, "%20")
	return rawURL
}

var removes = "help,wei,fmt,version,noop,nocolor,no_header,file"

func canonicalizeURL(rawURL string) string {
	rawURL = preClean(strings.Replace(rawURL, ":", "%3A", -1))
	cleanURL := strings.Join(strings.Fields(rawURL), "")
	if parsedURL, err := url.Parse("http://localhost?" + cleanURL); err != nil {
		logger.Error(err)
		return ""
	} else {
		rawQuery := parsedURL.RawQuery
		var newQuery string
		for _, part := range strings.Split(rawQuery, "&") {
			kv := strings.SplitN(part, "=", 2)
			key := kv[0]
			value := ""
			if len(kv) > 1 {
				value = kv[1]
			}
			if strings.Contains(removes, key) {
				continue
			} else if strings.Contains(key, "_") {
				key = camelCase(key)
			}
			if newQuery != "" {
				newQuery += "&"
			}
			if value == "" {
				newQuery += key
			} else {
				newQuery += key + "=" + value
			}
		}
		canonicalURL := newQuery
		canonicalURL = strings.Replace(canonicalURL, "%3A", ":", -1)
		canonicalURL = strings.Replace(canonicalURL, "+", "%20", -1)
		return canonicalURL
	}
}

func camelCase(s string) string {
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

func (t *TestCase) Clean() string {
	ret := []string{}
	for _, option := range t.OptionArray {
		op := "--" + strings.Replace(option, "=", " ", -1)
		if strings.Contains(option, "@") {
			op = "-" + strings.Replace(option, "@", "", -1)
		}
		removes := []string{"--blocks", "--transactions", "--modes", "--terms", "--addrs"}
		for _, remove := range removes {
			op = strings.Replace(op, remove+" ", "", -1)
		}
		if len(option) > 0 {
			op = strings.Replace(op, "*", "\\*", -1)
			ret = append(ret, op)
		}
	}
	return strings.Join(ret, " ")
}

func init() {
	os.Setenv("NO_USERQUERY", "true")
	os.Setenv("TEST_MODE", "true")
	os.Setenv("NO_COLOR", "true")
	colors.ColorsOff()
}

func padRight(str string, length int, bumpPad bool, pad rune) string {
	if len(str) < length {
		str += strings.Repeat(string(pad), length-len(str))
	} else {
		str = str[:length]
	}

	if bumpPad && length >= 3 {
		str = str[:length-3] + strings.Repeat(string(pad), 3)
	}
	return str
}
