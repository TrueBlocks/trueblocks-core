package types

import (
	"testing"
)

func TestFacetValidation(t *testing.T) {
	// Valid facet
	validFacet := Facet{
		Name:     "TestFacet",
		Store:    "TestStore",
		Cruds:    []string{"delete", "update"},
		ViewType: "table",
	}
	if err := validFacet.ValidateAll(); err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	// Invalid CRUD
	invalidCrudFacet := Facet{
		Name:     "InvalidCrudFacet",
		Store:    "TestStore",
		Cruds:    []string{"invalid"},
		ViewType: "form",
	}
	if err := invalidCrudFacet.ValidateAll(); err == nil {
		t.Error("Expected error for invalid CRUD, got none")
	}

	// Invalid viewType
	invalidViewTypeFacet := Facet{
		Name:     "InvalidViewTypeFacet",
		Store:    "TestStore",
		Cruds:    []string{"delete"},
		ViewType: "invalid",
	}
	if err := invalidViewTypeFacet.ValidateAll(); err == nil {
		t.Error("Expected error for invalid viewType, got none")
	}

	// Empty viewType (valid case)
	emptyViewTypeFacet := Facet{
		Name:     "EmptyViewTypeFacet",
		Store:    "TestStore",
		Cruds:    []string{"delete"},
		ViewType: "",
	}
	if err := emptyViewTypeFacet.ValidateAll(); err != nil {
		t.Errorf("Expected no error for empty viewType, got %v", err)
	}
}
