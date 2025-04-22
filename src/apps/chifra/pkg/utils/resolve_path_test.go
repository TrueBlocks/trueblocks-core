package utils

import (
	"os"
	"os/exec"
	"path/filepath"
	"testing"

	"github.com/stretchr/testify/assert"
)

// Testing status: reviewed

func TestResolvePath_Fatals(t *testing.T) {
	if os.Getenv("TEST_FATAL") == "1" {
		input := os.Getenv("TEST_INPUT")
		_ = ResolvePath(input)
		return
	}

	tests := []struct {
		input string
	}{
		{"~username"},
		{"$UNSET_ENV_VAR/test"},
	}

	for _, tt := range tests {
		t.Run(tt.input, func(t *testing.T) {
			cmd := exec.Command(os.Args[0], "-test.run=TestResolvePath_Fatals")
			cmd.Env = append(os.Environ(), "TEST_FATAL=1", "TEST_INPUT="+tt.input, "UNSET_ENV_VAR=")
			err := cmd.Run()
			if err == nil || err.Error() != "exit status 1" {
				t.Fatalf("expected Fatal to exit with status 1, got %v for input %s", err, tt.input)
			}
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
