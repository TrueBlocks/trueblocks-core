package types

import (
	"testing"
)

func TestFacetValidation(t *testing.T) {
	// Valid facet
	validFacet := Facet{
		Name:     "TestFacet",
		Store:    "TestStore",
		Actions:  []string{"delete", "update"},
		ViewType: "table",
	}
	if err := validFacet.ValidateAll(); err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	// Invalid Action
	invalidActionFacet := Facet{
		Name:     "InvalidActionFacet",
		Store:    "TestStore",
		Actions:  []string{"invalid"},
		ViewType: "form",
	}
	if err := invalidActionFacet.ValidateAll(); err == nil {
		t.Error("Expected error for invalid Action, got none")
	}

	// Invalid viewType
	invalidViewTypeFacet := Facet{
		Name:     "InvalidViewTypeFacet",
		Store:    "TestStore",
		Actions:  []string{"delete"},
		ViewType: "invalid",
	}
	if err := invalidViewTypeFacet.ValidateAll(); err == nil {
		t.Error("Expected error for invalid viewType, got none")
	}

	// Empty viewType (valid case)
	emptyViewTypeFacet := Facet{
		Name:     "EmptyViewTypeFacet",
		Store:    "TestStore",
		Actions:  []string{"delete"},
		ViewType: "",
	}
	if err := emptyViewTypeFacet.ValidateAll(); err != nil {
		t.Errorf("Expected no error for empty viewType, got %v", err)
	}
}
