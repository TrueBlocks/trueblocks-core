package pinning

import (
	"testing"
)

func TestIsValid(t *testing.T) {
	tests := []struct {
		needle  string
		isValid bool
	}{
		{
			needle:  "not-a-cid",
			isValid: false,
		},
		{
			needle:  "QmZ1",
			isValid: false,
		},
		{
			needle:  "QmUjPmAWTbvmdbWKivhamp9yuRCtQtVsEjnncojamNqFd4",
			isValid: true,
		},
	}
	for _, test := range tests {
		isValid := IsValid(test.needle)
		if isValid != test.isValid {
			t.Errorf("Expected %v, got %v", test.isValid, isValid)
		}
	}
}
