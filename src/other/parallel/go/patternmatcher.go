/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
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
