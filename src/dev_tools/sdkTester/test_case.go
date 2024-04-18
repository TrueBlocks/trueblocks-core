package main

import (
	"fmt"
	"strings"
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
		return t.CmdTest(mode)
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
		if t.Mode == "cmd" || strings.Contains(t.Tool, "chifra") || t.Route == "monitors" {
			return false
		}
	case "cmd":
		if t.Mode == "api" {
			return false
		}
	case "sdk":
		if t.HasShorthand || strings.Contains(t.Tool, "chifra") || t.Route == "monitors" {
			return false
		}
	}

	return true
}
