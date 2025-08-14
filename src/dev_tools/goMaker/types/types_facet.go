package types

import (
	"fmt"
	"strings"
)

type Facet struct {
	Name       string          `json:"name"`
	Store      string          `json:"store"`
	Actions    []string        `toml:"actions" json:"actions"`
	Confirms   map[string]bool `json:"-" toml:"-"` // actions requiring confirmation (parsed from -confirm suffix)
	ViewType   string          `toml:"viewType" json:"viewType"`
	Attributes string          `json:"attributes"`
}

var allowedViewTypes = map[string]bool{
	"table":     true,
	"form":      true,
	"dashboard": true,
}

func (f *Facet) SingleStore() string {
	if f == nil {
		return ""
	}
	return Singular(f.Store)
}

func (f *Facet) ValidateViewType() error {
	if f == nil {
		return nil
	}
	if f.ViewType == "" {
		f.ViewType = "table" // Default to table view if not specified
	}

	if !allowedViewTypes[f.ViewType] {
		return fmt.Errorf("invalid viewType value: %s", f.ViewType)
	}

	return nil
}

func (f *Facet) ValidateActions() error {
	for _, action := range f.Actions {
		if !rowActions[action] && !headerActions[action] {
			return fmt.Errorf("invalid action value: %s", action)
		}
	}
	return nil
}

// NormalizeActions processes any "-confirm" suffix, storing the base action name and a parallel confirmation flag.
func (f *Facet) NormalizeActions() {
	if f == nil || len(f.Actions) == 0 {
		return
	}
	acts := make([]string, 0, len(f.Actions))
	confirms := make(map[string]bool)
	for _, a := range f.Actions {
		need := false
		if strings.HasSuffix(a, "-confirm") {
			need = true
			a = strings.TrimSuffix(a, "-confirm")
		}
		a = strings.TrimSpace(a)
		if a == "" { // skip empties
			continue
		}
		acts = append(acts, a)
		if need {
			confirms[a] = true
		}
	}
	f.Actions = acts
	f.Confirms = confirms
}

func (f *Facet) ValidateAll() error {
	if err := f.ValidateActions(); err != nil {
		return err
	}
	if err := f.ValidateViewType(); err != nil {
		return err
	}
	return nil
}

func (f *Facet) HasViewType() bool {
	if f == nil {
		return false
	}
	return f.ViewType != ""
}

func (f *Facet) IsTable() bool {
	return f.ViewType == "table"
}

func (f *Facet) IsForm() bool {
	return f.ViewType == "form" || f.ViewType == "dashboard"
}

func (f *Facet) HasDivider() bool {
	return strings.Contains(f.Attributes, "dividerBefore") || strings.Contains(f.Attributes, "dividerAfter")
}

func (f *Facet) Divider() string {
	if strings.Contains(f.Attributes, "dividerBefore") {
		return "dividerBefore"
	} else if strings.Contains(f.Attributes, "dividerAfter") {
		return "dividerAfter"
	}
	return ""
}
