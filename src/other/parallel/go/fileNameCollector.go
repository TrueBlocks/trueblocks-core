// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package main

import (
	"fmt"
	"sort"
	"strings"
	"sync"
)

type fileNameCollector struct {
	sortedList              []string
	patternMatchedChannel   chan string
	patternMatchedWaitGroup sync.WaitGroup
}

func (fc *fileNameCollector) init() {
	fc.sortedList = make([]string, 0)
}

func (fc *fileNameCollector) collect() {
	fc.patternMatchedWaitGroup.Add(1)
	go func() {
		defer fc.patternMatchedWaitGroup.Done()
		for matchedFile := range fc.patternMatchedChannel {
			fc.sortedList = append(fc.sortedList, matchedFile)
		}
	}()
}

func (fc *fileNameCollector) buildSortedList() {
	fc.patternMatchedWaitGroup.Wait()

	sort.Slice(fc.sortedList, func(i int, j int) bool {
		// sort in descending order for testing
		if strings.Compare(fc.sortedList[i], fc.sortedList[j]) > 0 {
			return true
		}
		return false
	})
}

func (fc *fileNameCollector) print() {
	fmt.Println("Sorted list of files: ")
	for _, item := range fc.sortedList {
		fmt.Println(item)
	}
}
