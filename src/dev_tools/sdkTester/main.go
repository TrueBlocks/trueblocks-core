package main

import (
	"encoding/csv"
	"fmt"
	"io"
	"net/url"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

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
	Destination string    `json:"destination"`
	Cannonical  string    `json:"cannonical"`
	Original    *Original `json:"original"`
}

func processCSVFile(filePath string) {
	file, err := os.Open(filePath)
	if err != nil {
		fmt.Printf("Error opening file: %v\n", err)
		return
	}
	defer file.Close()

	reader := csv.NewReader(file)
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
			parts := strings.Split(orig.PathTool, "/")
			testCase := TestCase{
				Enabled:     orig.Enabled == "on",
				Route:       orig.Route,
				PathTool:    orig.PathTool,
				Options:     strings.Split(clean(orig.Options), "&"),
				GoldPath:    "../test/gold/" + orig.PathTool + "/",
				WorkingPath: "../../../working/" + orig.PathTool + "/",
				Destination: "../../../working/" + orig.PathTool + "/" + parts[1] + "_" + orig.Filename + ".txt",
				Cannonical:  canonicalizeURL(orig.Options),
				Original:    &orig,
			}
			testCases = append(testCases, testCase)
		}
	}

	os.Setenv("TEST_MODE", "true")
	for _, testCase := range testCases {
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
}

// parses the URL, and returns a canonicalized version of the URL.
func canonicalizeURL(rawURL string) string {
	// Trim spaces around the URL and between tokens
	rawURL = strings.Replace(rawURL, ":", "[COLON]", -1)
	cleanURL := strings.Join(strings.Fields(rawURL), "")

	// Parse the cleaned URL
	parsedURL, err := url.Parse(cleanURL)
	if err != nil {
		logger.Error(err)
		return ""
	}

	// Rebuild the URL to ensure it's in a canonical form
	// Here, you might want to further manipulate parsedURL to adjust scheme, host, etc., as needed
	canonicalURL := strings.Replace(parsedURL.String(), "[COLON]", ":", -1)
	// fmt.Println(colors.Yellow+rawURL, canonicalURL+colors.Off)
	return canonicalURL
}

func clean(s string) string {
	options := strings.Replace(strings.Replace(s, "& ", "&", -1), " &", "&", -1)
	options = strings.Replace(strings.Replace(options, "= ", "=", -1), " =", "=", -1)
	options = strings.Replace(options, "%20", " ", -1)
	return options
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
