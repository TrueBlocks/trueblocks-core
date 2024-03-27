package codeWriter

import (
	"bufio"
	"bytes"
	"fmt"
	"go/format"
	"os"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

var m sync.Mutex

func WriteCode(existingFn, newCode string) error {
	tempFn := existingFn + ".new"
	defer func() {
		// delete the temp file when we're done
		os.Remove(tempFn)
	}()

	// save the new code in a temporary file
	file.StringToAsciiFile(tempFn, newCode)

	// extract the EXISTING_CODE from the existing file
	existingParts, err := extractExistingCode(existingFn)
	if err != nil {
		return fmt.Errorf("error extracting existing code: %v", err)
	}

	// apply the EXISTING_CODE to the new code, format the new code and
	// write it back to the original file (potentially destroying it)
	wasModified, err := applyTemplate(tempFn, existingParts)
	if err != nil {
		return fmt.Errorf("error applying template: %v %s", err, existingFn)
	}

	defer func() {
		m.Unlock()
	}()
	m.Lock()
	if !wasModified {
		logger.Progress(true, colors.Green+"No changes to", existingFn+colors.Off, strings.Repeat(" ", 20))
	} else {
		logger.Info(colors.Yellow+"Wrote changes to", existingFn, strings.Repeat(" ", 20)+colors.Off)
	}

	return nil
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
				if code, ok := existingCode[codeSection]; ok {
					buffer.WriteString(code)
					isOpen = true
				} else {
					return false, fmt.Errorf("missing // EXISTING_CODE section %d line %d", codeSection, lineCnt)
				}
			}
		} else {
			buffer.WriteString(line + "\n")
		}
	}

	if err := scanner.Err(); err != nil {
		return false, err
	}

	origFn := strings.Replace(tempFn, ".new", "", 1)
	newCode := buffer.String()
	formatted := newCode
	if strings.HasSuffix(origFn, ".go") {
		formattedBytes, err := format.Source([]byte(newCode))
		if err != nil {
			return false, fmt.Errorf("format.Source failed: %v %s", err, file.AsciiFileToString(newCode))
		}
		formatted = string(formattedBytes)
	}
	if string(formatted) == file.AsciiFileToString(origFn) {
		return false, nil
	} else {
		file.StringToAsciiFile(origFn, string(formatted)) // modified code is in the original file
		return true, nil
	}
}
