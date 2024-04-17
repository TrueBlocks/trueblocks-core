package main

import (
	"fmt"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type record struct {
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
		return t.CmdTest(mode)
	} else if mode == "sdk" {
		return t.SdkTest()
	}
	return "", fmt.Errorf("Invalid mode:" + mode)
}

func (t *TestCase) ShouldTest(mode string) bool {
	if !t.IsEnabled {
		return false
	}

	switch mode {
	case "api":
		if t.Mode == "cmd" || strings.Contains(t.PathTool, "chifra") || t.Route == "monitors" {
			return false
		}
	case "cmd":
		if t.Mode == "api" {
			return false
		}
	case "sdk":
		if t.HasShorthand || strings.Contains(t.PathTool, "chifra") || t.Route == "monitors" {
			return false
		}
	}

	return true
}

func (t *TestCase) GetOutputPaths(mode string) (string, string, string) {
	working := t.WorkingPath
	if mode != "cmd" {
		working = filepath.Join(t.WorkingPath, mode+"_tests") + "/"
	}
	file.EstablishFolder(working)
	parts := strings.Split(t.PathTool, "/")
	if len(parts) < 2 {
		logger.Fatal(fmt.Sprintf("Invalid pathTool: %s. Need two parts.", t.PathTool))
	}

	workFn := filepath.Join(working, parts[1]+"_"+t.Filename+".txt")
	goldFn := strings.Replace(workFn, "working", "gold", -1)
	envFn := filepath.Join(working, t.Filename+".env")
	if !file.FileExists(envFn) {
		envFn = ""
	}

	return workFn, goldFn, envFn
}
