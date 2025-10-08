package types

import (
	"bufio"
	"bytes"
	"fmt"
	"go/format"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func WriteCode(existingFn, newCode string) (bool, error) {
	VerboseLog("Writing code to:", existingFn)

	if len(strings.Trim(newCode, wss)) == 0 {
		VerboseLog("  Skipping empty code")
		return false, nil
	}

	// For new files or files in /generated/, just write the new code directly
	if !file.FileExists(existingFn) || strings.Contains(existingFn, "/generated/") {
		if !strings.Contains(existingFn, "/generated/") {
			if !file.FolderExists(filepath.Dir(existingFn)) {
				_ = file.EstablishFolder(filepath.Dir(existingFn))
			}
			if !verbose {
				logger.Info(colors.Yellow+"Creating", existingFn, strings.Repeat(" ", 20)+colors.Off)
			} else {
				VerboseLog("  Creating new file:", existingFn)
			}
		}
		return updateFile(existingFn, newCode)
	}

	VerboseLog("  Updating existing file:", existingFn)

	tempFn := existingFn + ".new"
	defer func() {
		// delete the temp file when we're done
		os.Remove(tempFn)
	}()

	// save the new code in a temporary file
	_ = file.StringToAsciiFile(tempFn, newCode)

	// extract the EXISTING_CODE from the existing file
	existingParts, err := extractExistingCode(existingFn)
	if err != nil {
		return false, fmt.Errorf("error extracting existing code: %v", err)
	}

	// apply the EXISTING_CODE to the new code
	wasModified, err := applyTemplate(tempFn, existingParts)
	if err != nil {
		// If there's an error applying the template and this is a generated file,
		// fall back to just writing the new code
		if strings.Contains(existingFn, "/generated/") {
			VerboseLog("  Falling back to direct write for generated file")
			return updateFile(existingFn, newCode)
		}
		return false, fmt.Errorf("error applying template: %v %s", err, existingFn)
	}

	msg := LogMessage{
		MessageType: "Progress",
		Message:     existingFn,
	}
	if !strings.Contains(existingFn, "/generated/") && wasModified {
		msg.MessageType = "Info"
		msg.Message = fmt.Sprintf("Wrote %s", existingFn)
	}
	logChannel <- msg

	return wasModified, nil
}

func extractExistingCode(fileName string) (map[int]string, error) {
	file, err := os.Open(fileName)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	existingCode := make(map[int]string)
	scanner := bufio.NewScanner(file)

	codeSection := 0
	inExistingCode := false
	for scanner.Scan() {
		line := scanner.Text()
		if strings.Contains(line, "// EXISTING_CODE") {
			if inExistingCode {
				existingCode[codeSection] += line + "\n"
				codeSection++
				inExistingCode = false
			} else {
				existingCode[codeSection] = line + "\n"
				inExistingCode = true
			}
		} else if inExistingCode {
			existingCode[codeSection] += line + "\n"
		}
	}

	if err := scanner.Err(); err != nil {
		return nil, err
	}

	return existingCode, nil
}

func applyTemplate(tempFn string, existingCode map[int]string) (bool, error) {
	defer os.Remove(tempFn) // we always try to remove this file

	ff, err := os.Open(tempFn)
	if err != nil {
		return false, err
	}
	defer ff.Close()

	isOpen := false
	lineCnt := 0
	codeSection := 0
	var buffer bytes.Buffer
	scanner := bufio.NewScanner(ff)

	for scanner.Scan() {
		line := scanner.Text()
		lineCnt++
		if strings.Contains(line, "// EXISTING_CODE") {
			if isOpen {
				isOpen = false
				codeSection++
			} else {
				// If we have existing code for this section, use it
				if code, ok := existingCode[codeSection]; ok {
					buffer.WriteString(code)
				} else {
					// Otherwise just write the marker and continue
					buffer.WriteString(line + "\n")
				}
				isOpen = true
			}
		} else if !isOpen {
			buffer.WriteString(line + "\n")
		}
	}

	if err := scanner.Err(); err != nil {
		return false, err
	}

	return updateFile(tempFn, buffer.String())
}

func updateFile(tempFn, newCode string) (bool, error) {
	lines := []string{}
	for _, line := range strings.Split(newCode, "\n") {
		if !strings.Contains(line, "//-- remove line --") {
			lines = append(lines, line)
		}
	}
	codeToWrite := strings.Join(lines, "\n")
	fileExt := strings.TrimPrefix(filepath.Ext(strings.TrimSuffix(tempFn, ".new")), ".")

	origFn := strings.Replace(tempFn, ".new", "", 1)
	outFn := tempFn + ".output"
	errFn := tempFn + ".error"
	tmpSrcFn := tempFn + ".src"
	defer func() {
		os.Remove(outFn)
		os.Remove(errFn)
		os.Remove(tmpSrcFn)
	}()

	if fileExt == "go" {
		formattedBytes, err := format.Source([]byte(codeToWrite))
		if err != nil {
			_, _ = showErroredCode(origFn, codeToWrite, err)
		}
		codeToWrite = string(formattedBytes)
	} else {
		var parser string
		switch fileExt {
		case "md":
			// parser = "markdown"
		case "yaml", "yml":
			parser = "yaml"
		case "js":
			// parser = "babel"
		case "jsx":
			parser = "babel"
		case "ts":
			// parser = "typescript"
		case "tsx":
			parser = "typescript"
		default:
			// do nothing
		}
		if parser != "" {
			if hasPrettier() {
				_ = file.StringToAsciiFile(tmpSrcFn, codeToWrite)
				prettierPath := getPrettierPath()

				var cmd string

				// If prettier is in frontend directory, cd there and run it
				if strings.Contains(prettierPath, "frontend") {
					// Get absolute paths for files since we're changing directory
					absTmpSrcFn, _ := filepath.Abs(tmpSrcFn)
					absOutFn, _ := filepath.Abs(outFn)
					absErrFn, _ := filepath.Abs(errFn)

					// Change to frontend directory and run prettier
					cmd = fmt.Sprintf("cd frontend && %s --parser %s %s > %s 2> %s",
						strings.Replace(prettierPath, "./frontend/", "./", 1),
						parser, absTmpSrcFn, absOutFn, absErrFn)
				} else {
					// Check if there's a prettier config file that might already specify plugins
					prettierConfigPaths := []string{
						".prettierrc",
						".prettierrc.json",
						".prettierrc.js",
						".prettierrc.yaml",
						".prettierrc.yml",
						"prettier.config.js",
						"./frontend/.prettierrc",
						"./frontend/.prettierrc.json",
						"./frontend/.prettierrc.js",
						"./frontend/.prettierrc.yaml",
						"./frontend/.prettierrc.yml",
						"./frontend/prettier.config.js",
					}

					hasConfig := false
					configPath := ""
					for _, path := range prettierConfigPaths {
						if file.FileExists(path) {
							hasConfig = true
							configPath = path
							break
						}
					}

					if !hasConfig {
						// Only add plugin explicitly if no config file is found
						pluginPath := getPluginPath()
						if pluginPath != "" {
							cmd = fmt.Sprintf("%s --plugin %s --parser %s %s > %s 2> %s", prettierPath, pluginPath, parser, tmpSrcFn, outFn, errFn)
						} else {
							cmd = fmt.Sprintf("%s --parser %s %s > %s 2> %s", prettierPath, parser, tmpSrcFn, outFn, errFn)
						}
					} else {
						// Use prettier config file and specify its path explicitly
						cmd = fmt.Sprintf("%s --config %s --parser %s %s > %s 2> %s", prettierPath, configPath, parser, tmpSrcFn, outFn, errFn)
					}
				}

				utils.System(cmd)
				errors := file.AsciiFileToString(errFn)
				if len(errors) > 0 {
					return showErroredCode(tmpSrcFn, codeToWrite, fmt.Errorf("prettier errors: %s", errors))
				}
				codeToWrite = file.AsciiFileToString(outFn)
			}
		}
	}

	// Compare the new formatted code to the existing file and only write if different
	if string(codeToWrite) == file.AsciiFileToString(origFn) {
		return false, nil
	} else {
		_ = file.StringToAsciiFile(origFn, string(codeToWrite)) // modified code is in the original file
		return true, nil
	}
}

type LogMessage struct {
	MessageType string
	Message     string
}

var logChannel = make(chan LogMessage, 100) // Buffered channel

// syncLogger goroutine
func syncLogger() {
	rep := strings.Repeat(" ", 30)
	for logMsg := range logChannel {
		switch logMsg.MessageType {
		case "Progress":
			logger.Progress(true, colors.Green+logMsg.Message+colors.Off+rep)
		case "Info":
			logger.Info(colors.Yellow + logMsg.Message + colors.Off + rep)
		default:
			logger.Error("Unknown message type:", logMsg.MessageType)
		}
	}
}

func init() {
	go syncLogger()
}

func hasPrettier() bool {
	return getPrettierPath() != ""
}

func getPrettierPath() string {
	// Search for prettier in common locations
	searchPaths := []string{
		"./node_modules/.bin/prettier",                // Local install in current directory
		"./sdk/typescript/node_modules/.bin/prettier", // TrueBlocks SDK location
		"./frontend/node_modules/.bin/prettier",       // Common frontend directory
		"./web/node_modules/.bin/prettier",            // Common web directory
	}

	for _, path := range searchPaths {
		if file.FileExists(path) {
			return path
		}
	}

	// Fall back to global prettier if available
	utils.System("which prettier >./found 2>/dev/null")
	defer os.Remove("./found")
	if file.FileExists("./found") {
		contents := file.AsciiFileToString("./found")
		if len(contents) > 0 {
			return "prettier"
		}
	}

	return ""
}

func getPluginPath() string {
	// Search for the sort-imports plugin in common locations
	searchPaths := []string{
		"./node_modules/@trivago/prettier-plugin-sort-imports/lib/src/index.js",                // Local install in current directory
		"./sdk/typescript/node_modules/@trivago/prettier-plugin-sort-imports/lib/src/index.js", // TrueBlocks SDK location
		"./frontend/node_modules/@trivago/prettier-plugin-sort-imports/lib/src/index.js",       // Common frontend directory
		"./web/node_modules/@trivago/prettier-plugin-sort-imports/lib/src/index.js",            // Common web directory
	}

	for _, path := range searchPaths {
		if file.FileExists(path) {
			return path
		}
	}

	return ""
}

func showErroredCode(fn, newCode string, err error) (bool, error) {
	logger.Error("Error formatting code at", fn, colors.Red, err, colors.Off)
	logger.Info("Code that caused the error:")
	for i, line := range strings.Split(newCode, "\n") {
		logger.Info(fmt.Sprintf("%s%4d%s: %s", colors.Yellow, i+1, colors.Off, line))
	}
	return false, err
}
