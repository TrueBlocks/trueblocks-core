// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"sync"
	"errors"
	"sort"
)

const (
	RANDOM_FILES_DIR	= "./random_files"
	FLAG_OFFSET			= 1
	MAX_THREADS			= 256
)

type Result struct {
	Index		uint32
	Value		byte
	Err			error
}

var wg sync.WaitGroup

func get_files(dir string) []os.FileInfo {

	files, err := ioutil.ReadDir(dir)
	if err != nil {
		log.Fatal(fmt.Sprintf("Can't read directory %s : %v",dir,err))
	}
	return files
}

func process_file(output chan <- Result, idx uint32, fileName string) {

	defer wg.Done()

	res := new(Result)
	res.Index = idx
	res.Value = 0

	bytes, err := ioutil.ReadFile(fileName)
	if err != nil {
		log.Println(fmt.Sprintf("Can't open file fore reading: %s: %v",fileName,err))
		res.Err = err
		output <- *res
		return
	}
	if len(bytes) <= FLAG_OFFSET {
		log.Println(errors.New(fmt.Sprintf("Possibly invalid file %s ",fileName)))
		res.Err = err
		output <- *res
		return
	}
	res.Value = bytes[FLAG_OFFSET]
	res.Err = nil
	output <- *res
}

func collect_results(dir string, files []os.FileInfo) []byte {

	numFiles := len(files)
	results := make(chan Result,numFiles)
	for i := uint32(0); i < uint32(numFiles) ; i++ {
		if files[i].IsDir() {
			continue
		}
		fname := files[i].Name()
		fullName := fmt.Sprintf("%s/%s",dir,fname)
		wg.Add(1)
		go process_file(results,i,fullName)
	}
	output := make([]byte,0,numFiles)
	wg.Wait()
	close(results)

	for r := range results {
		//fmt.Printf("collect: file %s: output: %d, error = %v\n",files[r.Index].Name(),r.Value,r.Err);
		if r.Err == nil {
			output = append(output,r.Value)
		}
	}

	return output
}

func dump(d []byte) {
	for i,datum := range d {
		if i > 0 {
			fmt.Printf(",")
		}
		fmt.Printf("%x",datum)
	}
}
func main() {
	args := os.Args
	if len(args) != 2 {
		log.Fatal(fmt.Sprintf("usage: %s [directory]",args[0]))
	}
	dir := args[1]


	files := get_files(dir)
	numFiles:= len(files)
	if len(files) == 0 {
		log.Fatal(fmt.Sprintf("Directory %s is empty",dir))
	}

	if numFiles >= MAX_THREADS {
		log.Fatal(fmt.Sprintf("Too many files, MAX_THREADS=%d, numFiles=%d",MAX_THREADS,numFiles))
	}
	fmt.Printf("Unsorted: ")
	results := collect_results(dir,files)
	dump(results)
	fmt.Println()

	sort.Slice(results, func(i int, j int) bool { return results[i] < results[j] })
	fmt.Printf("Sorted: ")
	dump(results)
	fmt.Println()

	os.Exit(0)
}
