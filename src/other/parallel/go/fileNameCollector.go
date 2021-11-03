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
