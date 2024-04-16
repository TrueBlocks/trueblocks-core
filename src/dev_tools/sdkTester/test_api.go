package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (tr *Runner) RunApiTest(t *TestCase) (bool, bool, error) {
	tr.AppendLog(t)

	working := strings.ReplaceAll(t.WorkingPath, "sdk_tests", "api_tests")
	if !file.FolderExists(working) {
		file.EstablishFolder(working)
	}

	wasTested := false
	passedTest := false

	os.Setenv("TEST_MODE", "true")
	logger.SetTestMode(true)

	parts := strings.Split(t.PathTool, "/")
	workFn := filepath.Join(working, parts[1]+"_"+t.Filename+".txt")

	workFile, _ := os.Create(workFn)
	logger.SetLoggerWriter(workFile)
	logger.ToggleDecoration()

	defer func() {
		logger.ToggleDecoration()
		logger.SetLoggerWriter(os.Stderr)
		eol := "\r"
		if wasTested && !passedTest {
			eol = "\n"
		}

		msg := "[passed " + cm["greenCheck"] + "]"
		if wasTested && !passedTest {
			msg = "[failed " + cm["redX"] + "]"
		}

		skip := strings.Repeat(" ", utils.Max(0, 120-len(workFn)))
		colors.ColorsOn()
		fmt.Printf("   %sTesting %d of %d %s %s%s%s%s", colors.Green, tr.NTested, tr.NFiltered, msg, workFn, skip, colors.Off, eol)
		colors.ColorsOff()
	}()

	logger.Info(t.Route + "?" + t.ApiOptions)

	wasTested = true
	if results, err := t.ApiTest(); err != nil {
		type E struct {
			Errors []string `json:"errors"`
		}
		e := E{Errors: []string{err.Error()}}
		bytes, _ := json.MarshalIndent(e, "", "  ")
		results = string(bytes)
		logger.Info(results)
	} else {
		results = strings.Trim(results, "\n\r")
		if len(results) > 0 {
			results = strings.Replace(results, "3735928559", "\"0xdeadbeef\"", -1)
			logger.Info(results)
		}
	}

	if workFile != nil {
		workFile.Close()
		newContents := file.AsciiFileToString(workFn)
		goldFn := strings.Replace(workFn, "working", "gold", -1)
		oldContents := file.AsciiFileToString(goldFn)
		passedTest = newContents == oldContents
	}

	return wasTested, passedTest, nil
}

func (t *TestCase) ApiTest() (string, error) {
	url := "http://localhost:8080/" + t.Route + "?" + t.ApiOptions
	if response, err := http.Get(url); err != nil {
		log.Printf("Failed to query URL %s: %v", url, err)
	} else {
		defer response.Body.Close()
		if body, err := ioutil.ReadAll(response.Body); err != nil {
			log.Printf("Failed to read response from URL %s: %v", url, err)
		} else {
			results := string(body)
			results = strings.Replace(results, "3735928559", "\"0xdeadbeef\"", -1) + "\n"
			return results, nil
		}
	}
	return "", nil
}
