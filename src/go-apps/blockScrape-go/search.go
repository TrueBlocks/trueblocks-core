package main

import (
	"fmt"
	"io/ioutil"
	"strconv"
)

// Searching!

// AddrSighting - An appearance of an address
type AddrSighting struct {
	block int
	txIdx int
}

func searchForAddress(address string, fileNames chan string, sightings chan AddrSighting) {
	for fileName := range fileNames {
		data, err := ioutil.ReadFile("block/" + fileName)
		if err != nil {
			fmt.Println("Error:", err)
		}
		fmt.Print(string(data))
		sightings <- AddrSighting{0, 0}
	}
}

func testSearch() {
	fileNames := make(chan string)
	sightings := make(chan AddrSighting)

	for i := 0; i < 10; i++ {
		go searchForAddress("0xe3e1d84`f4d369faa89b01393b34a8193da6dead", fileNames, sightings)
	}

	for i := 6000000; i < 6000000+10000; i++ {
		fileName := PadLeft(strconv.Itoa(i), 9) + ".txt"
		fileNames <- fileName
	}

	done := make(chan int)
	<-done
}
