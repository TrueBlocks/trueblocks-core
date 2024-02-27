package main

import (
	"encoding/csv"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strings"
)

type Original struct {
	Enabled  string
	Mode     string
	Speed    string
	Route    string
	PathTool string
	Filename string
	Post     string
	Options  string
}

type TestCase struct {
	Enabled  bool
	Route    string
	PathTool string
	Options  []string
	Original *Original
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
			// fmt.Printf("Error reading CSV data at line %d: %v\n", lineNumber, err)
			continue
		}

		if len(record) == 0 || (len(record[0]) > 0 && record[0][0] == '#') {
			// Skip empty lines or lines starting with '#'
			continue
		}
		if len(record) != requiredFields {
			// fmt.Printf("Skipping line %d: incorrect number of fields. Expected %d, got %d.\n", lineNumber, requiredFields, len(record))
			continue
		}

		if !strings.HasPrefix(record[3], "route") {
			testCase := Original{
				Enabled:  strings.Trim(record[0], " "),
				Mode:     strings.Trim(record[1], " "),
				Speed:    strings.Trim(record[2], " "),
				Route:    strings.Trim(record[3], " "),
				PathTool: strings.Trim(record[4], " "),
				Filename: strings.Trim(record[5], " "),
				Post:     strings.Trim(record[6], " "),
				Options:  strings.Trim(record[7], " "),
			}
			options := strings.Replace(strings.Replace(testCase.Options, "& ", "&", -1), " &", "&", -1)
			options = strings.Replace(strings.Replace(options, "= ", "=", -1), " =", "=", -1)
			testCases = append(testCases, TestCase{
				Enabled:  testCase.Enabled == "on",
				Route:    testCase.Route,
				PathTool: testCase.PathTool,
				Options:  strings.Split(options, "&"),
				Original: &testCase,
			})
		}
	}

	for _, testCase := range testCases {
		// fmt.Printf("Enabled: %v, Route: %s, PathTool: %s, Options: %v\n", testCase.Enabled, testCase.Route, testCase.PathTool, testCase.Options)

		parts := strings.Split(testCase.PathTool, "/")
		workingFolder := "../../../working/" + testCase.PathTool + "/"
		goldFolder := "../test/gold/" + testCase.PathTool + "/"
		destination := workingFolder + parts[1] + "_" + testCase.Original.Filename + ".txt"
		cmd := fmt.Sprintf("chifra %s  ", testCase.Route)
		for _, option := range testCase.Options {
			op := "--" + strings.Replace(option, "=", " ", -1)
			if strings.Contains(option, "@") {
				op = "-" + strings.Replace(option, "@", "", -1)
			}
			removes := []string{"--blocks", "--transactions", "--modes", "--terms"}
			for _, remove := range removes {
				op = strings.Replace(op, remove+" ", "", -1)
			}
			if len(option) > 0 {
				cmd += op + " "
			}
		}
		cmd = strings.Trim(cmd, " ")
		fmt.Printf("echo \"%s\"\n", cmd)
		fmt.Printf("cd %s ; echo \"%s\" >%s ; cd -\n", goldFolder, cmd, destination)
		fmt.Printf("cd %s ; TEST_MODE=true NO_COLOR=true %s >>%s 2>&1  ; cd -\n", goldFolder, cmd, destination)
	}
}

func main() {
	paths := []string{
		"../testRunner/testCases/tools",
		"../testRunner/testCases/apps",
		// "../testRunner/testCases/dev_tools",
	}

	for _, rootPath := range paths {
		err := filepath.Walk(rootPath, func(path string, info os.FileInfo, err error) error {
			if err != nil {
				fmt.Printf("prevent panic by handling failure accessing a path %q: %v\n", path, err)
				return err
			}
			if !info.IsDir() && strings.HasSuffix(path, ".csv") {
				// fmt.Printf(colors.Yellow+"Processing CSV file: %s\n", path+colors.Off)
				processCSVFile(path)
			}
			return nil
		})
		if err != nil {
			fmt.Printf("error walking the path %q: %v\n", rootPath, err)
		}
	}
}

/*
# Generate all possible pairs including the format requested
all_options = set()
for case in test_cases:
    all_options.update(case)

# Generate all possible pairs
all_pairs = list(combinations(sorted(all_options), 2))

# Count occurrences of each pair in test_cases
pair_counts = defaultdict(int)
for case in test_cases:
    for pair in combinations(case, 2):
        sorted_pair = tuple(sorted(pair))
        pair_counts[sorted_pair] += 1

# Prepare data for the table in the requested format
table_data = []
for option in sorted(all_options):
    table_data.append([option, "", "", ""])  # Option header row
    for other_option in sorted(all_options):
        if option == other_option:
            continue  # Skip same option pair
        pair_key = tuple(sorted([option, other_option]))
        count = pair_counts.get(pair_key, 0)  # Get count, defaulting to 0 if not found
        table_data.append(["", other_option, count, ""])

table_header = ["Option 1", "Option 2", "Count", ""]
table = [table_header] + table_data

table
*/
