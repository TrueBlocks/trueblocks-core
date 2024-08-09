package main

import (
	"fmt"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

type record struct {
	Enabled  string `json:"enabled"`
	Mode     string `json:"mode"`
	Speed    string `json:"speed"`
	Route    string `json:"route"`
	Path     string `json:"path"`
	Tool     string `json:"tool"`
	Filename string `json:"filename"`
	Post     string `json:"post"`
	Options  string `json:"options"`
}

type TestCase struct {
	record
	IsEnabled       bool     `json:"isEnabled,omitempty"`
	HasShorthand    bool     `json:"hasShorthand,omitempty"`
	WorkingPath     string   `json:"workingPath,omitempty"`
	OrigOptions     string   `json:"origOptions,omitempty"`
	SourceFile      string   `json:"sourceFile,omitempty"`
	SdkOptions      string   `json:"sdkOptions,omitempty"`
	CmdOptions      string   `json:"cmdOptions,omitempty"`
	ApiOptions      string   `json:"apiOptions,omitempty"`
	SdkOptionsArray []string `json:"sdkOptionArray,omitempty"`
	OptionArray     []string `json:"optionArray,omitempty"`
}

func (t *TestCase) OptionsForMode(mode string) string {
	if mode == "api" {
		return t.ApiOptions
	} else if mode == "cmd" {
		return t.CmdOptions
	} else if mode == "sdk" {
		return t.SdkOptions
	}
	return t.OrigOptions
}

func (t *TestCase) InnerTest(mode string) (string, error) {
	if mode == "api" {
		return t.ApiTest()
	} else if mode == "cmd" {
		return t.CmdTest()
	} else if mode == "sdk" {
		return t.SdkTest()
	}
	return "", fmt.Errorf("Invalid mode:" + mode)
}

func (t *TestCase) ShouldRun(mode string) bool {
	if !t.IsEnabled {
		return false
	}

	switch mode {
	case "api":
		if t.Mode == "cmd" || t.Mode == "sdk" || strings.Contains(t.Tool, "chifra") || t.Route == "monitors" {
			return false
		}
	case "cmd":
		if t.Mode == "api" || t.Mode == "sdk" {
			return false
		}
	case "sdk":
		if t.Mode == "cmd" || t.Mode == "api" || t.HasShorthand || strings.Contains(t.Tool, "chifra") || t.Route == "monitors" {
			return false
		}
	}

	return true
}

func (t *TestCase) GetOutputPaths(mode string) (string, string, string, string) {
	working := t.WorkingPath
	if mode != "cmd" {
		working = filepath.Join(t.WorkingPath, mode+"_tests")
	}
	gold := strings.ReplaceAll(working, "working", "gold")

	workFn := filepath.Join(working, t.Tool+"_"+t.Filename+".txt")
	goldFn := filepath.Join(gold, t.Tool+"_"+t.Filename+".txt")
	testRoot := func(s string) string {
		return strings.ReplaceAll(strings.ReplaceAll(s, "sdk_tests", ""), "api_tests", "")
	}
	envFn := filepath.Join(testRoot(gold), t.Filename+".env")
	if !file.FileExists(envFn) {
		envFn = ""
	}
	outputFn := ""
	if mode == "cmd" {
		redirFn := filepath.Join(gold, t.Filename+".redir")
		if file.FileExists(redirFn) {
			t.OptionArray = append(t.OptionArray, "output="+t.Filename+"_out.file")
		}
		for _, option := range t.OptionArray {
			if strings.HasPrefix(option, "output") {
				parts := strings.Split(option, "=")
				if len(parts) < 2 {
					continue
				}
				outputFn = parts[1]
				break
			}
		}
		if len(outputFn) > 0 {
			outputFn = filepath.Join(gold, outputFn)
		}
	}

	return workFn, goldFn, envFn, outputFn
}

func (t *TestCase) ProcessRedirFile() {
	gold := strings.ReplaceAll(t.WorkingPath, "working", "gold")
	redirFn := filepath.Join(gold, t.Filename+".redir")
	if file.FileExists(redirFn) {
		outFn := t.Filename + "_out.file"
		t.OptionArray = append(t.OptionArray, "output="+outFn)
		t.CmdOptions += " --output " + outFn
	}
}
