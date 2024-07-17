package file

import "strings"

// ValidFilename returns a valid filename from the input string
func ValidFilename(in string, noSpaces bool) string {
	invalidChars := []string{
		"\n", "\r", "\t", "/", "\\", ":", "*", "?", "'", "\"",
		"<", ">", "|", "(", ")", "[", "]", "{", "}",
	}
	if noSpaces {
		invalidChars = append(invalidChars, " ")
	}

	for _, char := range invalidChars {
		in = strings.ReplaceAll(in, char, "_")
	}
	in = strings.TrimSpace(in)
	in = strings.Trim(in, "_")
	in = strings.ReplaceAll(in, "__", "_")
	return in
}
