package utils

import "regexp"

func StripColors(input string) string {
	re := regexp.MustCompile(`\033\[[0-9;]*[mK]`)
	return re.ReplaceAllString(input, "")
}
