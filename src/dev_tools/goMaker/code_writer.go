package main

import (
	"bufio"
	"fmt"
	"go/format"
	"log"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func WriteOut(source, dest string) {
	existingCode, err := extractExistingCode(dest)
	if err != nil {
		fmt.Println("Error extracting existing code:", err)
		return
	}

	err = applyTemplate(source, existingCode)
	if err != nil {
		fmt.Println("Error applying template:", err)
	}
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

func applyTemplate(fileName string, existingCode map[int]string) error {
	ff, err := os.Open(fileName)
	if err != nil {
		return err
	}
	defer ff.Close()

	dest := strings.Replace(fileName, ".new", "", -1)
	output, err := os.Create(dest)
	if err != nil {
		return err
	}
	defer func() {
		output.Close()
		FormatCode(dest)
		os.Remove(fileName)
	}()

	scanner := bufio.NewScanner(ff)
	codeSection := 0
	isOpen := false
	for scanner.Scan() {
		line := scanner.Text()
		if strings.Contains(line, "// EXISTING_CODE") {
			if isOpen {
				isOpen = false
				codeSection++
			} else {
				if code, ok := existingCode[codeSection]; ok {
					output.WriteString(code)
					isOpen = true
				} else {
					logger.Fatal("No existing code for section", codeSection)
				}
			}
		} else {
			output.WriteString(line + "\n")
		}
	}

	if err := scanner.Err(); err != nil {
		return err
	}

	return nil
}

func FormatCode(fn string) {
	if !strings.HasSuffix(fn, ".go") {
		return
	}
	raw := file.AsciiFileToString(fn)
	formattedSrc, err := format.Source([]byte(raw))
	if err != nil {
		log.Fatalf("format.Source failed: %v %s", err, string(raw))
	}
	file.StringToAsciiFile(fn, string(formattedSrc))
}
