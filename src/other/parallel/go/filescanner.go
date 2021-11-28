// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package main

import (
	"fmt"
	"os"
	"path/filepath"
	"runtime"
)

//DirectoryWalk walks through directory and prints files
func DirectoryWalk(root string) {
	var files []string

	err := filepath.Walk(root, func(path string, info os.FileInfo, err error) error {
		files = append(files, path)
		return nil
	})
	if err != nil {
		panic(err)
	}
	for _, file := range files {
		fmt.Println(file)
	}

	fmt.Println("Count:", len(files))
}

func main() {

	if len(os.Args) != 3 {
		fmt.Println("Usage:", os.Args[0], "PATTERN", "DIRECTORY")
		fmt.Println("Example: ", os.Args[0], "H", "/home/user/Downloads")
		return
	}

	if len(os.Args[1]) < 1 {
		fmt.Println("Pattern cannot be empty")
		return
	}

	pattern := []byte(os.Args[1])
	root := os.Args[2]

	fmt.Println(pattern, root)

	if yes, err := exists(root); yes == false || err != nil {
		fmt.Println(root, "Directory available : ", yes, err)
		return
	}

	bytePattern := byte(60) // dummy initialization used to match deb file

	bytePattern = pattern[0]

	fmt.Println("Start of scan")
	ParallelDirectoryScan(bytePattern, root)
	fmt.Println("End of scan")
}

// exists returns whether the given file or directory exists
func exists(path string) (bool, error) {
	_, err := os.Stat(path)
	if err == nil {
		return true, nil
	}
	if os.IsNotExist(err) {
		return false, nil
	}
	return true, err
}

// ParallelDirectoryScan scans given directory in parallel
func ParallelDirectoryScan(patternToSearch byte, directoryToScan string) {

	directoriesChannel := make(chan string, 100)

	dirScan := directoryScanner{
		rootDirectory:    directoryToScan,
		directoryChannel: directoriesChannel,
		walkSubdirectory: true,
	}

	dirScan.scanInBackground()

	patternMatchedChannel := make(chan string, 100)

	processor := fileProcessor{
		workerCount:           runtime.NumCPU(), // set worker equal to number of cpu
		directoriesChannel:    directoriesChannel,
		patternMatchedChannel: patternMatchedChannel,
	}

	processor.processInBackground(patternToSearch)

	collector := fileNameCollector{
		patternMatchedChannel: patternMatchedChannel,
	}

	collector.collect()

	dirScan.waitForCompletion()

	processor.waitForCompletion()

	collector.buildSortedList()
	collector.print()

	processor.printStatus()
}
