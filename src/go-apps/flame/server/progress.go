package server

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"strings"
)

// dropNL drops new line characters (\n) from the progress stream
func dropNL(data []byte) []byte {
	if len(data) > 0 && data[len(data)-1] == '\n' {
		return data[0 : len(data)-1]
	}
	return data
}

// ScanProgressLine looks for "lines" that end with `\r` not `\n` like usual
func ScanProgressLine(data []byte, atEOF bool) (advance int, token []byte, err error) {
	if atEOF && len(data) == 0 {
		return 0, nil, nil
	}
	if i := bytes.IndexByte(data, '\r'); i >= 0 {
		return i + 1, dropNL(data[0:i]), nil
	}
	return bufio.ScanLines(data, atEOF);
}

// ScanForProgress watches stderr and picks of progress messages
func ScanForProgress(stderrPipe io.Reader, fn func(string)) {
	scanner := bufio.NewScanner(stderrPipe)
	scanner.Split(ScanProgressLine)
	for scanner.Scan() {
		text := scanner.Text()
		if len(text) > 0 {
			fmt.Println(text)
			if (strings.Contains(text, "<PROG>")) {
				fn(strings.SplitAfter(text, ":")[1])
			}
		}
	}

	if err := scanner.Err(); err != nil {
		fmt.Println("Error while reading stderr:", err)
	}
}
