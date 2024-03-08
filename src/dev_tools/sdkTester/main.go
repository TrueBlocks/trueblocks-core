package main

import (
	"encoding/csv"
	"encoding/json"
	"fmt"
	"io"
	"net/url"
	"os"
<<<<<<< HEAD
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
=======
	"path/filepath"
	"regexp"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
>>>>>>> ec00f9d435535d8bad8b2049a4d6980dd03473db
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

var pathToTests = []string{
	"../src/dev_tools/testRunner/testCases/tools",
	"../src/dev_tools/testRunner/testCases/apps",
	// "../src/dev_tools/testRunner/testCases/dev_tools",
}

// var pathToTests = []string{
// 	"../testRunner/testCases/tools",
// 	"../testRunner/testCases/apps",
// }

func main() {
	walkFunc := func(path string, info os.FileInfo, err error) error {
		if err == nil {
			if !info.IsDir() && strings.HasSuffix(path, ".csv") {
				processCSVFile(path)
			}
		}
		return err
	}

	fmt.Println(os.Getwd())
	for _, rootPath := range pathToTests {
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

func processCSVFile(filePath string) {
	ff, err := os.Open(filePath)
	if err != nil {
		fmt.Printf("Error opening file: %v\n", err)
		return
	}
	defer ff.Close()

	fmt.Println("Processing", filePath)

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

	for _, testCase := range testCases {
<<<<<<< HEAD
		if testCase.Enabled {
			cmd := fmt.Sprintf("chifra %s %s", testCase.Route, testCase.Clean())
			cmd = strings.Trim(cmd, " ")

			// logger.Info(fmt.Sprintf(colors.Yellow+"Enabled: %v, Route: %s, PathTool: %s, Options: %v"+colors.Off, testCase.Enabled, testCase.Route, testCase.PathTool, testCase.Options))
			// logger.Info("\t" + colors.Green + cmd + colors.Off)
			logger.Info(fmt.Sprintf("Enabled: %v, Route: %s, PathTool: %s, Options: %v", testCase.Enabled, testCase.Route, testCase.PathTool, testCase.Options))
			logger.Info("\t" + cmd)
			// var buff bytes.Buffer
			// testCase.SdkTest(&buff)

			// // 	fmt.Printf("echo \"%s\"\n", cmd)
			// // 	fmt.Printf("cd %s ; echo \"%s\" >%s ; cd -\n", testCase.GoldPath, cmd, testCase.Destination)
			// // 	out := fmt.Sprintf("[ -f ./output_test_file ] && echo '----' >>%s && echo 'Results in ./output_test_file' >>%s && cat ./output_test_file >>%s && echo >>%s && rm -f ./output_test_file", testCase.Destination, testCase.Destination, testCase.Destination, testCase.Destination)
			// // 	fmt.Printf("cd %s ; TEST_MODE=true NO_COLOR=true %s >>%s 2>&1 ; %s ; cd -\n", testCase.GoldPath, cmd, testCase.Destination, out)
			// // fmt.Println(colors.White + testCase.Original.Options + colors.Off)
			// // fmt.Println(colors.BrightBlue + testCase.Cannonical + colors.Off)
			// fmt.Println("curl -H \"User-Agent: testRunner\" \"http://localhost:8080/" + testCase.Route + "?" + testCase.Cannonical + "\"")
		}
	}
}

// func main() {
// 	paths := []string{
// 		"../testRunner/testCases/tools",
// 		"../testRunner/testCases/apps",
// 		// "../testRunner/testCases/dev_tools",
// 	}

// 	for _, rootPath := range paths {
// 		err := filepath.Walk(rootPath, func(path string, info os.FileInfo, err error) error {
// 			if err != nil {
// 				fmt.Printf("prevent panic by handling failure accessing a path %q: %v\n", path, err)
// 				return err
// 			}
// 			if !info.IsDir() && strings.HasSuffix(path, ".csv") {
// 				// fmt.Printf(colors.Yellow+"Processing CSV file: %s\n", path+colors.Off)
// 				processCSVFile(path)
// 			}
// 			return nil
// 		})
// 		if err != nil {
// 			fmt.Printf("error walking the path %q: %v\n", rootPath, err)
// 		}
// 	}
// 	// canonicalizeURL(os.Args[1])
// }

func main() {
	raw := "addrs = 0x05a56e2d52c817161883f50c441c3228cfe54d9f & fmt = txt & first_block = 90 & first_record = 3 & max_records = 10"
	url := canonicalizeURL(raw)
	fmt.Println(colors.Green + url + colors.Off)
	cmd := canonicalizeCmd(url)
	fmt.Println(colors.Green + cmd + colors.Off)
	args := strings.Split(cmd, " ") // strings.Split("--count --first_record 100 --fmt json 0x123 0x456", " ")

	var err error
	var opts sdk.ListOptions
	opts.Positionals, err = sdk.ParseOptions[sdk.ListOptions](args, &opts)
	if err != nil {
		fmt.Println("Error parsing list options:", err)
		os.Exit(1)
	}

	// Print the parsed options and addresses to verify
	fmt.Printf("Options: %+v\n", opts)
	fmt.Printf("Positionals: %v\n", opts.Positionals)
}

func canonicalizeCmd(rawURL string) string {
	ret := "--" + rawURL
	ret = strings.Replace(ret, "&", " --", -1)
	ret = strings.Replace(ret, "=", " ", -1)
	ret = strings.Replace(ret, "%20", " ", -1)
	ret = strings.Replace(ret, "--addrs", " ", -1)
	ret = strings.Replace(ret, "--blocks", " ", -1)
	ret = strings.Replace(ret, "--transactions", " ", -1)
	ret = strings.Replace(ret, "--terms", " ", -1)
	return strings.Trim(ret, " ")
=======
		testCase.RunTest()
	}
}

func preClean(rawURL string) string {
	rawURL = regexp.MustCompile(`\s*&\s*`).ReplaceAllString(rawURL, "&")
	rawURL = regexp.MustCompile(`\s*=\s*`).ReplaceAllString(rawURL, "=")
	rawURL = regexp.MustCompile(`\s+`).ReplaceAllString(rawURL, "%20")
	return rawURL
>>>>>>> ec00f9d435535d8bad8b2049a4d6980dd03473db
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
				key = snakeCase(key)
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
	os.Setenv("TEST_MODE", "true")
	os.Setenv("NO_COLOR", "true")
	colors.ColorsOff()
}

func (t *TestCase) RunTest() {
	if !t.Enabled {
		return
	}

	// interesting := t.PathTool == "tools/getLogs" && t.Original.Filename == "bad_blkhash_msg_raw" // (t.Route == "list" || t.Route == "receipts" || t.Route == "when") && t.PathTool != "apps/chifra"
	interesting := (t.Route == "list" || t.Route == "receipts" || t.Route == "when") && t.PathTool != "apps/chifra"
	if !interesting {
		return
	}

	if interesting {
		parts := strings.Split(t.PathTool, "/")
		if len(parts) != 2 {
			fmt.Fprintf(os.Stderr, "Invalid pathTool: %s\n", t.PathTool)
			return
		}

		folder := t.WorkingPath
		if !file.FolderExists(folder) {
			// fmt.Println("Creating folder", folder)
			// fmt.Println()
			file.EstablishFolder(folder)
			file.StringToAsciiFile(filepath.Join(folder, ".gitignore"), "# place holder - do not remove")
		}
		fn := filepath.Join(folder, parts[1]+"_"+t.Original.Filename+".txt")
		fmt.Printf("Testing %s...", fn)

		ff, _ := os.Create(fn)
		logger.SetLoggerWriter(ff)
		logger.ToggleDecoration()
		logger.SetTestMode(true)
		os.Setenv("TEST_MODE", "true")
		defer func() {
			logger.ToggleDecoration()
			logger.SetLoggerWriter(os.Stderr)
			ff.Close()
			fmt.Println("Done.")
		}()
		logger.Info(t.Route + "?" + t.Cannonical)

	} else {
		logger.Info()
		logger.Info(strings.Repeat("=", 40), t.Original.Filename, strings.Repeat("=", 40))
		logger.Info(fmt.Sprintf("Route: %s, PathTool: %s, Enabled: %v, Options: %v", t.Route, t.PathTool, t.Enabled, t.Options))
		logger.Info("\t" + strings.Trim(fmt.Sprintf("chifra %s %s", t.Route, t.Clean()), " "))
	}

	var buff bytes.Buffer
	var results string
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
}
