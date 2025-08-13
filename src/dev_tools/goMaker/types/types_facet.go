package types

import (
	"fmt"
	"strings"
)

type Facet struct {
	Name       string   `json:"name"`
	Store      string   `json:"store"`
	Actions    []string `toml:"actions" json:"actions"`
	ViewType   string   `toml:"viewType" json:"viewType"`
	Attributes string   `json:"attributes"`
}

var allowedActions = map[string]bool{
	"delete":   true,
	"undelete": true,
	"remove":   true,
	"autoname": true,
	"update":   true,
	"clean":    true,
	"cleanOne": true,
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

func (f *Facet) ValidateActions() error {
	for _, action := range f.Actions {
		if !allowedActions[action] {
			return fmt.Errorf("invalid action value: %s", action)
		}
	}
	return nil
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

func (f *Facet) ValidateAll() error {
	if err := f.ValidateActions(); err != nil {
		return err
	}
	if err := f.ValidateViewType(); err != nil {
		return err
	}
	return nil
}

func (f *Facet) HasActions() bool {
	if f == nil {
		return false
	}
	return len(f.Actions) > 0
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
	return f.ViewType == "form"
}

func (f *Facet) IsDashboard() bool {
	return f.ViewType == "dashboard"
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
