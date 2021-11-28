// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package main

import (
	"fmt"
	"io"
	"os"
)

// PatternMatcher struct used for encapsulating matching meta
type PatternMatcher struct {
	fileToMatch string
}

func (matcher *PatternMatcher) match(bytePattern byte) bool {

	// open file
	// read whole content
	// search for bytepattern
	// decide true or false

	file, err := os.OpenFile(matcher.fileToMatch, os.O_RDONLY, 0755)
	defer file.Close()
	if err != nil {
		fmt.Println("Error occured while opening file", matcher.fileToMatch, err)
		return false
	}

	// read first two bytes and try to match for the pattern
	const bytesToRead int = 2
	bytes := make([]byte, bytesToRead)

	if size, err := file.Read(bytes); err != nil {
		if err != io.EOF || size > 0 {
			fmt.Println("Error occured while reading file.Size =", size, matcher.fileToMatch, err)
		}
		return false
	} else if size != bytesToRead {
		fmt.Printf("Not enough data available to read. Required = %v, Available %v", bytesToRead, size)
		return false
	}

	// last byte is required byte comparision for this case
	return bytes[bytesToRead-1] == bytePattern
}
