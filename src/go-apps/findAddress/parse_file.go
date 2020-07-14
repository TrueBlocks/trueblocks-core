package main

import (
	"encoding/binary"
	"fmt"
	"log"
	"os"
	"sort"
	"sync"
)

func getHeader(f *os.File) (head Header, err error) {
	err = binary.Read(f, binary.LittleEndian, &head)
	return
}

func getAddressRecord(f *os.File) (addressRec addressRecord, err error) {
	err = binary.Read(f, binary.LittleEndian, &addressRec)
	return
}

func getAddressRecordAndCompare(f *os.File, target Address) (bigger bool, err error) {
	addressRec, err := getAddressRecord(f)
	if err != nil {
		return false, err
	}

	return addressRec.Bytes.compare(target) >= 0, nil
}

func findAddress(numRecs uint32, addr Address, f *os.File) (pos int) {

	comp := func(pos int) bool {
		//fmt.Println(pos)
		if pos == -1 {
			return false
		}
		if pos == int(numRecs) {
			return true
		}
		f.Seek(int64(HeaderSize+pos*AddressSize), 0)
		bigger, err := getAddressRecordAndCompare(f, addr)
		if err != nil {
			fmt.Println(err)
			return false
		}
		return bigger
	}
	pos = sort.Search(int(numRecs), comp)
	f.Seek(int64(HeaderSize+pos*AddressSize), 0)
	rec, err := getAddressRecord(f)
	if err != nil {
		return -1
	}
	if rec.Bytes.compare(addr) != 0 {
		return -1
	}
	return
}

func getAppearanceRecords(f *os.File, numAddresses uint32, offset uint32, numRecs uint32) (appy []appearanceRecord, err error) {
	_, err = f.Seek(int64(HeaderSize+numAddresses*AddressSize+offset*AppearanceSize), 0)
	if err != nil {
		log.Fatal(err)
	}
	appy = make([]appearanceRecord, numRecs)

	err = binary.Read(f, binary.LittleEndian, &appy)

	return
}

func parseFile(fileName string, addr Address, resultChannel chan []appearanceRecord, wg *sync.WaitGroup, logging bool) {
	defer wg.Done()
	if logging {
		log.Println(fileName)
	}
	f, err := os.Open(fileName)
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()
	head, err := getHeader(f)
	if err != nil {
		log.Fatal(err)
	}
	if head.Magic != MagicNumber {
		log.Fatal("invalid format", fileName)
	}
	var results []appearanceRecord
	newPos := findAddress(head.NumberOafAddresses, addr, f)
	if newPos == -1 {
		resultChannel <- results // empty array
		return
	}
	//fmt.Println(newPos)
	_, err = f.Seek(int64(HeaderSize+newPos*AddressSize), 0)
	if err != nil {
		log.Fatal(err)
	}
	adx, err := getAddressRecord(f)
	//fmt.Printf("0x%x : %d   %d\n", adx.Bytes, adx.Cnt, adx.Offset)

	results, err = getAppearanceRecords(f, head.NumberOafAddresses, adx.Offset, adx.Cnt)

	if err != nil {
		log.Fatal(err)
	}
	resultChannel <- results

	return
}
