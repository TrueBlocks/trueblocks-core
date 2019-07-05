package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strconv"
)

func readLines(path string) ([]string, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var lines []string
	// TODO: get file size, then make the slice that number of lines

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		text := scanner.Text()
		lines = append(lines, text)
	}
	return lines, scanner.Err()
}

func writeLines(lines []string, path string) error {
	file, err := os.Create(path)
	if err != nil {
		return err
	}
	defer file.Close()

	w := bufio.NewWriter(file)
	for _, line := range lines {
		fmt.Fprintln(w, line)
	}
	return w.Flush()
}

func consolidate(startBlock int, endBlock int, numRecords int) {
	currRecords := make([]string, 0)
	numCurrRecords := 0
	currStartBlock := startBlock
	for i := startBlock; i < endBlock; i++ {
		// read in file, and figure out number of records
		addressSightings, err := readLines("data/" + PadLeft(strconv.Itoa(i), 9) + ".txt")
		if err != nil {
			fmt.Println("ERROR:", err)
		}
		currRecords = append(currRecords, addressSightings...)
		numCurrRecords += len(addressSightings)
		if numCurrRecords > numRecords {
			// sort, and then write file
			sort.Strings(currRecords)
			// TODO: can sort in linear time, as compared to log linear (b/c already sorted)
			writeLines(currRecords, "data/"+PadLeft("a"+strconv.Itoa(currStartBlock), 9)+"-"+PadLeft(strconv.Itoa(i), 9)+".txt")

			currRecords = make([]string, 0)
			numCurrRecords = 0
			currStartBlock = i + 1
		}
	}
	sort.Strings(currRecords)
	writeLines(currRecords, "data/"+PadLeft("a"+strconv.Itoa(currStartBlock), 9)+"-"+PadLeft(strconv.Itoa(endBlock), 9)+".txt")
}
