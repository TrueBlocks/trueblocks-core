package utils

import (
	"os"
	"path/filepath"
	"testing"

	"github.com/stretchr/testify/assert"
)

// Testing status: reviewed

func TestResolvePath_Panics(t *testing.T) {
	tests := []struct {
		input string
	}{
		{"~username"},
		{"$UNSET_ENV_VAR/test"},
	}

	for _, tt := range tests {
		t.Run(tt.input, func(t *testing.T) {
			defer func() {
				if r := recover(); r == nil {
					t.Fatalf("expected panic for input %s, but did not panic", tt.input)
				}
			}()
			_ = ResolvePath(tt.input)
		})
	}
}

func TestResolvePath_NonFatals(t *testing.T) {
	homeDir, _ := os.UserHomeDir()
	currentDir, _ := os.Getwd()

	tests := []struct {
		input    string
		expected string
	}{
		{"~", homeDir},
		{"~/test", filepath.Join(homeDir, "test")},
		{"$HOME/test", filepath.Join(homeDir, "test")},
		{"./test", filepath.Join(currentDir, "test")},
		{"/usr/local/test", "/usr/local/test"},
		{"", ""},
	}

	for _, tt := range tests {
		t.Run(tt.input, func(t *testing.T) {
			result := ResolvePath(tt.input)
			assert.Equal(t, tt.expected, result)
		})
	}
}
