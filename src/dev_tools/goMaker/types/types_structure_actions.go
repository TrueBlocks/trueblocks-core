package types

import (
	"fmt"
	"sort"
	"strings"
)

func (s *Structure) HasDelete() bool {
	return s.Class != "Dresses" && strings.Contains(strings.Join(s.UniqueActions(), ","), "delete")
}

func (s *Structure) HasAutoname() bool {
	return strings.Contains(strings.Join(s.UniqueActions(), ","), "autoname")
}

func (s *Structure) HasUpdate() bool {
	return s.Class == "Names" && strings.Contains(strings.Join(s.UniqueActions(), ","), "update")
}

func (s *Structure) HasCrud() bool {
	crudActions := []string{"create", "update", "delete", "remove", "undelete", "autoname", "remove-confirm"}
	for _, facet := range s.Facets {
		if facet.Renderer == "facet" {
			continue
		}
		for _, action := range facet.Actions {
			for _, crudAction := range crudActions {
				if action == crudAction {
					return true
				}
			}
		}
	}
	return false
}

func (s *Structure) RowActionsFe() string {
	ret := []string{}
	for _, action := range s.UniqueActions() {
		if action != "undelete" && rowActions[action] {
			ret = append(ret, action)
		}
	}
	return "'" + strings.Join(ret, "', '") + "'"
}

func (f *Facet) RowActionsBe(facet string) string {
	_ = facet
	ret := []string{}
	for _, a := range f.Actions {
		if a != "undelete" && rowActions[a] {
			if !contains(ret, a) {
				ret = append(ret, a)
			}
		}
	}
	r := strings.Join(ret, "\", \"")
	if r == "" {
		return ""
	}
	return "\"" + r + "\""
}

func (s *Structure) RowActionsBe(facet string) string {
	f := s.FacetByName(facet)
	if f == nil {
		return ""
	}
	return f.RowActionsBe(facet)
}

func (f *Facet) HeaderActionsBe(facet string) string {
	_ = facet
	ret := []string{}
	for _, a := range f.Actions {
		if headerActions[a] {
			if !contains(ret, a) {
				ret = append(ret, a)
			}
		}
	}
	r := strings.Join(ret, "\", \"")
	if r == "" {
		return ""
	}
	return "\"" + r + "\""
}

func (s *Structure) HeaderActionsBe(facet string) string {
	f := s.FacetByName(facet)
	if f == nil {
		return ""
	}
	return f.HeaderActionsBe(facet)
}

func (s *Structure) AllActions() []string {
	// Build confirmation lookup from facets' Confirm maps
	confirmMap := make(map[string]bool)
	for _, f := range s.Facets {
		for a := range f.Confirms {
			confirmMap[a] = true
		}
	}
	ret := []string{}
	for _, action := range s.UniqueActions() {
		if confirmMap[action] {
			ret = append(ret, fmt.Sprintf("\"%s\": {Name: \"%s\", Label: \"%s\", Icon: \"%s\", Confirmation: true},", action, action, FirstUpper(action), FirstUpper(action)))
		} else {
			ret = append(ret, fmt.Sprintf("\"%s\": {Name: \"%s\", Label: \"%s\", Icon: \"%s\"},", action, action, FirstUpper(action), FirstUpper(action)))
		}
	}
	return ret
}

// returns a list of unique actions from all the facets sorted alphabetically
func (s *Structure) UniqueActions() []string {
	actionSet := make(map[string]struct{})
	for _, f := range s.Facets {
		for _, a := range f.Actions {
			if a != "" {
				actionSet[a] = struct{}{}
			}
		}
	}
	actions := make([]string, 0, len(actionSet))
	for action := range actionSet {
		actions = append(actions, action)
	}
	sort.Strings(actions)
	return actions
}

func contains(sl []string, v string) bool {
	for _, s := range sl {
		if s == v {
			return true
		}
	}
	return false
}

func (s *Structure) Handlers_inner() string {
	ret := []string{}
	for _, action := range s.UniqueActions() {
		if action == "undelete" || !rowActions[action] {
			continue
		}
		name := action
		if action == "delete" {
			name = "toggle"
		}
		ret = append(ret, "handle"+FirstUpper(name))
	}
	sort.Strings(ret)
	return strings.Join(ret, ",")
}

func (s *Structure) Handlers() string {
	ret := s.Handlers_inner()
	if s.HasRowActions() && s.Class != "Dresses" {
		ret = ret + ", handleRowAction"
	} else if s.Class == "Dresses" {
		ret = "handleRowAction"
	}
	return ret
}

func (s *Structure) HandlerStrs() string {
	handlers := strings.Split(s.Handlers_inner(), ",")
	ret := []string{}
	for _, handler := range handlers {
		handler = strings.TrimSpace(handler)
		if handler == "handleUpdate" {
			continue
		}
		ret = append(ret, "      "+handler+",")
	}
	return strings.Join(ret, "\n")
}

var rowActions = map[string]bool{
	"delete":   true,
	"undelete": true,
	"remove":   true,
	"autoname": true,
	"update":   true,
	"revoke":   true,
	"cleanRow": true,
}

var headerActions = map[string]bool{
	"create":  true,
	"clean":   true,
	"publish": true,
	"pin":     true,
	"export":  true,
}

func (s *Store) HasActions(facets []Facet) bool {
	for _, f := range facets {
		if f.StoreName == s.Name && len(f.Actions) > 0 {
			return true
		}
	}
	return false
}
func (s *Structure) HasCrudActions() bool {
	for _, f := range s.Facets {
		for _, action := range f.Actions {
			if action == "create" || action == "update" || action == "delete" || action == "remove" {
				return true
			}
		}
	}
	return false
}

func (s *Structure) FacetByName(facet string) *Facet {
	for _, f := range s.Facets {
		if f.Name == facet {
			return &f
		}
	}
	return nil
}

func (s *Structure) HasRowActions() bool {
	for _, f := range s.Facets {
		if len(f.Navigate) > 0 {
			return true
		}
	}
	return false
}
