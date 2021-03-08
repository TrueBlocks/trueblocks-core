package trueblocks

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

import (
	"bufio"
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"strings"
)

// CommandProgress shows progress for a long running request
type CommandProgress struct{
	Op string `json:"op"`
}

func (cp *CommandProgress) toJSON() []byte {
	json, err := json.Marshal(cp)
	if err != nil {
		log.Fatalf("Cannot marshal CommandProgress to JSON: %s", err.Error())
	}
	return json
}

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
func ScanForProgress(stderrPipe io.Reader, fn func(*CommandProgress)) {
	scanner := bufio.NewScanner(stderrPipe)
	scanner.Split(ScanProgressLine)
	for scanner.Scan() {
		text := scanner.Text()
		if len(text) > 0 {
			fmt.Println(text)
			if (strings.Contains(text, "<PROG>")) {
				substring := strings.SplitAfter(text, ":")
				fn(&CommandProgress{
					Op: substring[1],
				})
			}
		}
	}

	if err := scanner.Err(); err != nil {
		fmt.Println("Error while reading stderr:", err)
	}
}
