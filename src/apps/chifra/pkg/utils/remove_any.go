package utils

import "strings"

// RemoveAny returns a new string with all characters from string A that are present in
// string B removed. The function uses a map for efficient lookups and preserves the
// order of characters in A.
func RemoveAny(A, B string) string {
	result := strings.Builder{}
	toRemove := make(map[rune]struct{})
	for _, char := range B {
		toRemove[char] = struct{}{}
	}
	for _, char := range A {
		if _, exists := toRemove[char]; !exists {
			result.WriteRune(char)
		}
	}
	return result.String()
}
