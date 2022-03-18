// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package file

import (
	"bufio"
	"os"
)

func AsciiFileToLines(filename string) []string {
	file, err := os.Open(filename)
	if err != nil {
		return []string{}
	}
	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanLines)
	var ret []string
	for scanner.Scan() {
		ret = append(ret, scanner.Text())
	}
	file.Close()
	return ret
}
