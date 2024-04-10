package main

import (
	"bytes"
	"encoding/csv"
	"encoding/json"
	"fmt"
	"io"
	"net/url"
	"os"
	"path/filepath"
	"regexp"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

var interactiveTests = false

var pathToTests = []string{
	"../src/dev_tools/testRunner/testCases/apps",
	"../src/dev_tools/testRunner/testCases/tools",
}

var pathToTestsTesting = []string{
	"../testRunner/testCases/apps",
	"../testRunner/testCases/tools",
}

func main() {
	walkFunc := func(path string, info os.FileInfo, err error) error {
		if err == nil {
			if !info.IsDir() && strings.HasSuffix(path, ".csv") {
				processCSVFile(path)
			}
		}
		return err
	}

	thePaths := pathToTests
	if interactiveTests {
		thePaths = pathToTestsTesting
	}

	fmt.Println(os.Getwd())
	for _, rootPath := range thePaths {
		if err := filepath.Walk(rootPath, walkFunc); err != nil {
			fmt.Printf("error walking the path %q: %v\n", rootPath, err)
		}
	}
}

type Original struct {
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
	Enabled     bool      `json:"enabled"`
	Route       string    `json:"route"`
	PathTool    string    `json:"pathTool"`
	Options     []string  `json:"options"`
	GoldPath    string    `json:"goldPath"`
	WorkingPath string    `json:"workingPath"`
	Cannonical  string    `json:"cannonical"`
	Original    *Original `json:"original"`
}

var cm = map[string]string{
	"greenCheck":    "\033[32m✓\033[0m",
	"yellowCaution": "\033[33m!!\033[0m",
	"redX":          "\033[31mX\033[0m",
	"whiteStar":     "\033[37m*\033[0m",
}

func processCSVFile(filePath string) {
	ff, err := os.Open(filePath)
	if err != nil {
		fmt.Printf("Error opening file: %v\n", err)
		return
	}
	defer ff.Close()

	fmt.Println("Testing", filePath, strings.Repeat(" ", 120-len(filePath)))

	reader := csv.NewReader(ff)
	const requiredFields = 8
	lineNumber := 0

	testCases := make([]TestCase, 0)
	for {
		lineNumber++
		record, err := reader.Read()
		if err != nil {
			if err == io.EOF {
				break // End of file reached
			}
			continue
		}

		if len(record) == 0 || (len(record[0]) > 0 && record[0][0] == '#') {
			continue
		}
		if len(record) != requiredFields {
			continue
		}

		if !strings.HasPrefix(record[3], "route") {
			if strings.Contains(record[7], "@") {
				continue // skip these weird (hot-key only) single character test cases for the SDK
			}

			// fmt.Println(strings.Repeat("-", 80))
			// fmt.Println(record[5])
			// fmt.Println(strings.Repeat("-", 80))

			orig := Original{
				Enabled:  strings.Trim(record[0], " "),
				Mode:     strings.Trim(record[1], " "),
				Speed:    strings.Trim(record[2], " "),
				Route:    strings.Trim(record[3], " "),
				PathTool: strings.Trim(record[4], " "),
				Filename: strings.Trim(record[5], " "),
				Post:     strings.Trim(record[6], " "),
				Options:  strings.Trim(record[7], " "),
			}

			cannon := canonicalizeURL(orig.Options)
			testCase := TestCase{
				Enabled:     orig.Enabled == "on",
				Route:       orig.Route,
				PathTool:    orig.PathTool,
				GoldPath:    "../src/dev_tools/sdkTester/test/gold/" + orig.PathTool + "/",
				WorkingPath: "../src/dev_tools/sdkTester/test/working/" + orig.PathTool + "/",
				Cannonical:  cannon,
				Options:     strings.Split(strings.Replace(cannon, "%20", " ", -1), "&"),
				Original:    &orig,
			}

			// fmt.Println(strings.Repeat("-", 80))
			// fmt.Println(record[5])
			// fmt.Println(strings.Repeat("-", 80))

			testCases = append(testCases, testCase)
		}
	}

	nTested, nPassed := 0, 0
	for i, testCase := range testCases {
		tested, passed := testCase.RunTest(i, len(testCases))
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
	fmt.Println(colors.White, "    Passed", nPassed, "of", nTested, "tests.", msg, colors.Off, strings.Repeat(" ", utils.Max(0, 90)))
	fmt.Println()
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
	for _, option := range t.Options {
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

func (t *TestCase) RunTest(i, n int) (bool, bool) {
	if !t.Enabled {
		return false, false
	}

	testing := []string{
		"list",
		"export",
		"monitors",
		"config",
		"status",
		"daemon",
		"scrape",
		"chunks",
		"init",
		"explore",
		"names",
		// "slurp",
		"abis",
		"blocks",
		"transactions",
		"receipts",
		"logs",
		"state",
		"tokens",
		"traces",
		"when",
	}
	interesting := false
	for _, test := range testing {
		if test == t.Route && t.PathTool != "apps/chifra" {
			interesting = true
			break
		}
	}
	// interesting = t.Route == "init" && t.Original.Filename == "fmt_txt"
	if !interesting {
		return false, false
	}

	parts := strings.Split(t.PathTool, "/")
	if len(parts) != 2 {
		fmt.Fprintf(os.Stderr, "Invalid pathTool: %s\n", t.PathTool)
		return false, false
	}

	var ff *os.File
	folder := t.WorkingPath
	if !file.FolderExists(folder) {
		file.EstablishFolder(folder)
	}

	wasTested := false
	passedTest := false

	fn := filepath.Join(folder, parts[1]+"_"+t.Original.Filename+".txt")
	if interesting {
		os.Setenv("TEST_MODE", "true")
		logger.SetTestMode(true)
		if !interactiveTests {
			ff, _ = os.Create(fn)
			logger.SetLoggerWriter(ff)
			logger.ToggleDecoration()
			defer func() {
				logger.ToggleDecoration()
				logger.SetLoggerWriter(os.Stderr)
				msg := "[passed " + cm["greenCheck"] + "]"
				eol := "\r"
				if wasTested && !passedTest {
					msg = "[failed " + cm["redX"] + "]"
					eol = "\n"
				}
				skip := strings.Repeat(" ", utils.Max(0, 120-len(fn)))
				fmt.Printf("   Testing %d of %d %s %s%s%s", i, n, msg, fn, skip, eol)
			}()
		}
		logger.Info(t.Route + "?" + t.Cannonical)

	} else {
		logger.Info()
		logger.Info(strings.Repeat("=", 40), t.Original.Filename, strings.Repeat("=", 40))
		logger.Info(fmt.Sprintf("Route: %s, PathTool: %s, Enabled: %v, Options: %v", t.Route, t.PathTool, t.Enabled, t.Options))
		logger.Info("\t" + strings.Trim(fmt.Sprintf("chifra %s %s", t.Route, t.Clean()), " "))
		return false, false
	}

	var buff bytes.Buffer
	var results string
	wasTested = true
	if err := t.SdkTest(&buff); err != nil {
		type E struct {
			Errors []string `json:"errors"`
		}
		e := E{Errors: []string{err.Error()}}
		bytes, _ := json.MarshalIndent(e, "", "  ")
		results = string(bytes)
	} else {
		results = strings.Trim(buff.String(), "\n\r")
	}

	if len(results) > 0 {
		// because testRunner does this, we need to do it here
		results = strings.Replace(results, "3735928559", "\"0xdeadbeef\"", -1)
		logger.Info(results)
	}

	if ff != nil {
		ff.Close()
		newContents := file.AsciiFileToString(fn)
		oldContents := file.AsciiFileToString(strings.Replace(fn, "working", "gold", -1))
		passedTest = newContents == oldContents
	}
	return wasTested, passedTest
}
