package utils

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

// Testing status: reviewed

func TestRemoveAny(t *testing.T) {
	tests := []struct {
		name     string
		A        string
		B        string
		expected string
	}{
		{"Remove characters", "hello world", "lo", "he wrd"},
		{"Empty B", "hello", "", "hello"},
		{"Empty A", "", "xyz", ""},
		{"Remove all", "abc", "abc", ""},
		{"No matching characters", "hello", "xyz", "hello"},
		{"Unicode characters", "你好世界", "界", "你好世"},
		{"Duplicate characters in B", "banana", "na", "b"},
		{"Case sensitivity", "Hello", "h", "Hello"},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			result := RemoveAny(tt.A, tt.B)
			assert.Equal(t, tt.expected, result)
		})
	}
}
