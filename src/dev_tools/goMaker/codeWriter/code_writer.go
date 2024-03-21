package codeWriter

import (
	"bufio"
	"fmt"
	"go/format"
	"log"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func WriteCode(existingFn, result string) error {
	destFn := existingFn + ".new"
	file.StringToAsciiFile(destFn, string(result))

	existingCode, err := extractExistingCode(existingFn)
	if err != nil {
		return fmt.Errorf("error extracting existing code: %v", err)
	}

	err = applyTemplate(destFn, existingCode)
	if err != nil {
		return fmt.Errorf("error applying template: %v", err)
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
		formatCode(dest)
		os.Remove(fileName)
	}()

	scanner := bufio.NewScanner(ff)
	codeSection := 0
	isOpen := false
	lineCnt := 0
	for scanner.Scan() {
		line := scanner.Text()
		lineCnt++
		if strings.Contains(line, "// EXISTING_CODE") {
			if isOpen {
				isOpen = false
				codeSection++
			} else {
				if code, ok := existingCode[codeSection]; ok {
					output.WriteString(code)
					isOpen = true
				} else {
					os.Remove(fileName)
					return fmt.Errorf("missing // EXISTING_CODE section %d line %d", codeSection, lineCnt)
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

func formatCode(fn string) {
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
