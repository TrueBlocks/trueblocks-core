// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package utils

import (
	"strings"
)

func FormattedCode(verbose bool, code string) string {
	codeLen := len(code)
	if verbose || codeLen <= 128 {
		return code
	}

	return strings.Join(
		[]string{
			code[:15],
			code[codeLen-15:],
		},
		"...",
	)
}

func FormattedHash(verbose bool, code string) string {
	codeLen := len(code)
	if verbose || codeLen <= 11 {
		return code
	}

	return strings.Join(
		[]string{
			code[:6],
			code[codeLen-4:],
		},
		"...",
	)
}
