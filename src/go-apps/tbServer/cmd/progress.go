package trueblocks

import (
	"bufio"
	"fmt"
	"log"
	"io"
	"regexp"
	"strconv"
	"encoding/json"
)

type CommandProgress struct{
	Op string `json:"op"`
	Done int `json:"done"`
	Total int `json:"total"`
	Finished bool `json:"finished"`
}

func (cp *CommandProgress) toJSON() []byte {
	json, err := json.Marshal(cp)

	if err != nil {
		log.Fatalf("Cannot marshal CommandProgress to JSON: %s", err.Error())
	}

	return json
}

var progressLine = regexp.MustCompile(`:([A-Za-z|\s]+)[\s]+([\d]+) of ([\d]+)`)

func MatchProgress(outputLine string) []string {
	return progressLine.FindStringSubmatch(outputLine)
}

type SendFunc func(*CommandProgress)

func ScanForProgress(stderrPipe io.Reader, fn SendFunc) {
	scanner := bufio.NewScanner(stderrPipe)

	for scanner.Scan() {
		matches := MatchProgress(scanner.Text())

		if len(matches) != 0 {
			done, err := strconv.Atoi(matches[2])

			if err != nil {
				fmt.Println("Cannot get 'done' from stderr");
				continue;
			}

			total, err := strconv.Atoi(matches[3])

			if err != nil {
				fmt.Println("Cannot get 'total' from stderr");
				continue;
			}

			fn(&CommandProgress{
				Op: matches[1],
				Done: done,
				Total: total,
				Finished: done == total,
			})
		}
	}

	if err := scanner.Err(); err != nil {
		fmt.Println("Error while reading stderr:", err)
	}
}
