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
	"io/ioutil"
	"log"
	"os"
	"sync"
	"errors"
	"sort"
	"encoding/binary"
	"encoding/hex"
	"bytes"
)

const (
	RANDOM_FILES_DIR	= "./random_files"
	FLAG_OFFSET			= 1
	MAX_THREADS			= 256
	MAGIC_NUMBER		= 3735928559
	ADDR_LEN			= 20
	HASH_LEN			= 32
	EXPECTED_NUM_ROWS_TO_FIND = 256
)

const (
	// to avoid using 'unsafe' in Go, we must hardode the sizes of our structs
	HEADER_SIZE			= 44
	ADDRESS_SIZE		= 28
	APPEARRANCE_SIZE	= 8
)
type Address [ADDR_LEN]byte
type EthHash [HASH_LEN]byte

// header of the File
type Header struct {
	Magic				uint32
	Hash				EthHash
	Num_addresses		uint32
	Num_appearrances	uint32
}
type AddressRec struct {
	Bytes		Address
	Offset		uint32
	Cnt			uint32
}
type AppearranceRec struct {
	Blk			uint32
	Txid		uint32
}

type Result struct {
	Index		uint32
	Value		[]AppearranceRec
	Err			error
}

var wg sync.WaitGroup

func make_header(bytes []byte) Header {

	hdr := new(Header)
	hdr.Magic =binary.LittleEndian.Uint32(bytes[0:])
	//hdr.Hash = ... we ignore the unused field
	hdr.Num_addresses = binary.LittleEndian.Uint32(bytes[4 + HASH_LEN:]);
	hdr.Num_appearrances = binary.LittleEndian.Uint32(bytes[4 + HASH_LEN + 4:]);
	return *hdr;
}
func make_address_rec(bytes []byte) AddressRec {

	addr := new(AddressRec)
	copy(addr.Bytes[0:ADDR_LEN],bytes[:ADDR_LEN])
	addr.Offset = binary.LittleEndian.Uint32(bytes[ADDR_LEN:])
	addr.Cnt = binary.LittleEndian.Uint32(bytes[ADDR_LEN+4:])
	return *addr
}
func make_appearrance_rec(bytes []byte) AppearranceRec {

	appearrance := new(AppearranceRec)
	appearrance.Blk = binary.LittleEndian.Uint32(bytes[:4])
	appearrance.Txid = binary.LittleEndian.Uint32(bytes[4:])
	return *appearrance
}
func get_files(dir string) []os.FileInfo {

	files, err := ioutil.ReadDir(dir)
	if err != nil {
		log.Fatal(fmt.Sprintf("Can't read directory %s : %v",dir,err))
	}
	return files
}

func launch_scan_on_file(output chan <- Result, idx uint32, fileName string,srch_addr []byte) {

	defer wg.Done()

	res := new(Result)
	res.Index = idx

	data, err := ioutil.ReadFile(fileName)
	if err != nil {
		log.Println(fmt.Sprintf("Can't open file fore reading: %s: %v",fileName,err))
		res.Err = err
		output <- *res
		return
	}
	buffer_len := uint32(len(data))
	if buffer_len < HEADER_SIZE {
		estr := fmt.Sprintf("File size of %s is less than the Header",fileName)
		log.Println(estr)
		res.Err = errors.New(estr)
		output <- *res
		return
	}
	header := make_header(data[0:])

	if header.Magic != MAGIC_NUMBER {
		estr := fmt.Sprintf("File %v has wrong magic number",fileName)
		log.Println(estr)
		res.Err = errors.New(estr)
		output <- *res
		return
	}

	var expected_file_len uint32 =	header.Num_addresses * ADDRESS_SIZE +
							header.Num_appearrances * APPEARRANCE_SIZE +
							HEADER_SIZE

	if buffer_len != expected_file_len {
		estr := fmt.Sprintf("File %v %v doesn't have proper length (bytes). Must have %v = %v*%v + %v*%v, but has %v",
				idx,fileName,
				expected_file_len,
				HEADER_SIZE,ADDRESS_SIZE,ADDRESS_SIZE * header.Num_addresses,
				APPEARRANCE_SIZE,header.Num_appearrances)

		log.Println(estr)
		res.Err = errors.New(estr)
		output <- *res
		return
	}
	var pos uint64 = HEADER_SIZE
	appearrances := make([]AppearranceRec,0,EXPECTED_NUM_ROWS_TO_FIND)
	for rec_no := uint32(0);  rec_no<header.Num_addresses; rec_no++ {
		addr_rec := make_address_rec(data[pos:]);
		//fmt.Println("addr = ",hex.EncodeToString(addr_rec.Bytes[:]));
		if bytes.Compare(addr_rec.Bytes[0:], srch_addr) == 0 {
			appear_pos :=	HEADER_SIZE + header.Num_addresses * ADDRESS_SIZE +
							addr_rec.Offset * APPEARRANCE_SIZE
			for i:=uint32(0); i < addr_rec.Cnt; i++ {
				appearrance_rec := make_appearrance_rec(data[appear_pos:])
				appearrances = append(appearrances,appearrance_rec)
				appear_pos = appear_pos + APPEARRANCE_SIZE
			}
			break
		}
		pos = pos + ADDRESS_SIZE
	}

	res.Value = appearrances
	res.Err = nil
	output <- *res
}

func collect_results(dir string, files []os.FileInfo, srch_addr []byte) []AppearranceRec {

	numFiles := len(files)
	results := make(chan Result,numFiles)
	for i := uint32(0); i < uint32(numFiles) ; i++ {
		if files[i].IsDir() {
			continue
		}
		fname := files[i].Name()
		fullName := fmt.Sprintf("%s/%s",dir,fname)
		wg.Add(1)
		go launch_scan_on_file(results,i,fullName,srch_addr)
	}
	output := make([]AppearranceRec,0,numFiles)
	wg.Wait()
	close(results)

	for r := range results {
		//fmt.Printf("collect: file %s: output: %d, error = %v\n",files[r.Index].Name(),r.Value,r.Err);
		if r.Err == nil {
			output = append(output,r.Value...)
		}
	}

	return output
}

func dump(d []AppearranceRec) {
	for i,datum := range d {
		if i > 0 {
			fmt.Printf(",")
		}
		fmt.Printf("{%v:%v}",datum.Blk,datum.Txid)
	}
}
func main() {
	args := os.Args
	if len(args) != 3 {
		log.Fatal(fmt.Sprintf("usage: %s [directory] [search_addr]",args[0]))
	}
	dir := args[1]
	srch_addr_str := args[2]

	files := get_files(dir)
	numFiles:= len(files)
	if len(files) == 0 {
		log.Fatal(fmt.Sprintf("Directory %s is empty",dir))
	}

	if numFiles >= MAX_THREADS {
		log.Fatal(fmt.Sprintf("Too many files, MAX_THREADS=%d, numFiles=%d",MAX_THREADS,numFiles))
	}
	srch_addr, err := hex.DecodeString(srch_addr_str)
	if err != nil {
		log.Fatal(fmt.Sprintf("Invalid addr: %v",err))
	}
	results := collect_results(dir,files,srch_addr)

	sort.Slice(results, func(i int, j int) bool { return results[i].Blk < results[j].Blk })
	dump(results)
	fmt.Println()

	os.Exit(0)
}
