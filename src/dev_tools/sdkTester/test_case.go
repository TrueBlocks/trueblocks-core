package main

import "strings"

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
	GoldPath        string   `json:"goldPath,omitempty"`
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

func (t *TestCase) ShouldTest(mode string) bool {
	if !t.IsEnabled {
		return false
	}

	isChifra := strings.Contains(t.PathTool, "chifra")
	switch mode {
	case "api":
		if t.Mode == "cmd" {
			return false
		}
	case "cmd":
		if t.Mode == "api" || isChifra {
			return false
		}
	case "sdk":
		if t.HasShorthand || isChifra {
			return false
		}
	}

	return true
}
