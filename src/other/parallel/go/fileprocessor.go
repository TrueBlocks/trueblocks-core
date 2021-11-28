// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package main

import (
	"fmt"
	"os"
	"path"
	"sync"
	"sync/atomic"
)

type fileProcessor struct {
	workerCount         int
	totalFileCount      uint64
	totalDirectoryCount uint64

	directoriesChannel    <-chan string
	patternMatchedChannel chan<- string
	fileScanWaitGroup     sync.WaitGroup
	fileProcessWaitGroup  sync.WaitGroup
}

// func getFileProcessor(directoryChannel chan string, workerCount int) *fileProcessor {
// 	return &fileProcessor{
// 		workerCount:        workerCount,
// 		directoriesChannel: directoryChannel,
// 	}
// }

func (processor *fileProcessor) waitForCompletion() {

	processor.fileScanWaitGroup.Wait()
	processor.fileProcessWaitGroup.Wait()
	close(processor.patternMatchedChannel)
}

func (processor *fileProcessor) printStatus() {
	fmt.Println("Total files scanned:", processor.totalFileCount,
		"Total Dir scanned:", processor.totalDirectoryCount)
}

func (processor *fileProcessor) processInBackground(patternToMatch byte) {
	for i := 0; i < processor.workerCount; i++ {
		processor.fileScanWaitGroup.Add(1)
		go func() {
			defer processor.fileScanWaitGroup.Done()
			for dirPath := range processor.directoriesChannel {

				atomic.AddUint64(&processor.totalDirectoryCount, 1)

				f, err := os.Open(dirPath)
				if err != nil {
					fmt.Println("Error occured while opening directory", dirPath, err)
					return
				}

				files, err := f.Readdir(-1)
				f.Close()

				if err != nil {
					fmt.Println("Error occured while reading directory", dirPath, err)
					return
				}

				for _, file := range files {
					if !file.IsDir() {
						atomic.AddUint64(&processor.totalFileCount, 1)
						fileFullPath := path.Join(dirPath, file.Name())
						//fmt.Println(fileFullPath, file.Size(), file.Mode())

						processor.fileProcessWaitGroup.Add(1)
						go func() {
							defer processor.fileProcessWaitGroup.Done()

							patternMatcher := PatternMatcher{
								fileFullPath,
							}

							if patternMatcher.match(patternToMatch) {
								// add some kind of concurrent datastructure
								// and then we will sort it later

								// should use channel here ??

								processor.patternMatchedChannel <- fileFullPath
							}
						}()
					}
				}
			}
		}()
	}
}
