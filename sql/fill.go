package main

import (
	"database/sql"
	"encoding/binary"
	"encoding/hex"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"path/filepath"
	"strings"

	_ "github.com/mattn/go-sqlite3"
)

type Appearance struct {
	address          string
	blockIndex       int
	transactionIndex int
}

func readFile(path string) ([]Appearance, error) {
	var appearances []Appearance

	f, err := os.Open(path)
	if err != nil {
		panic(err)
	}
	defer f.Close()

	// https://gateway.pinata.cloud/ipfs/Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2

	//
	headerByteSlice := make([]byte, 44)
	_, err = f.Read(headerByteSlice)
	if err != nil {
		log.Fatal(err)
	}
	nAddresses := binary.LittleEndian.Uint32(headerByteSlice[44-8 : 44-4])
	nAppearances := binary.LittleEndian.Uint32(headerByteSlice[44-4:])

	log.Printf("Number Addresses: %s\n", nAddresses)
	log.Printf("Number Appearances: %s\n", nAppearances)

	addressByteSlice := make([]byte, 28*nAddresses)
	_, err = f.Read(addressByteSlice)
	if err != nil {
		log.Fatal(err)
	}

	appearanceByteSlice := make([]byte, 8*nAppearances)
	_, err = f.Read(appearanceByteSlice)
	if err != nil {
		log.Fatal(err)
	}

	// For each
	for i := 0; int64(i) < int64(nAddresses); i++ {
		// TODO: this address parsing is a bug, what is the encoding we want?
		var address = hex.EncodeToString(addressByteSlice[(i * 28) : (i*28)+20])
		var startRecord = binary.LittleEndian.Uint32(addressByteSlice[(i*28)+20 : (i*28)+20+4])
		var nRecord = binary.LittleEndian.Uint32(addressByteSlice[(i*28)+20+4 : (i*28)+20+4+4])

		for j := 0; int64(j) < int64(nRecord); j++ {
			var start = (int(startRecord) * 8) + (j * 8)
			var blockIndex = binary.LittleEndian.Uint32(appearanceByteSlice[start : start+4])
			var transactionIndex = binary.LittleEndian.Uint32(appearanceByteSlice[start+4 : start+4+4])
			appearances = append(appearances, Appearance{address: address, blockIndex: int(blockIndex), transactionIndex: int(transactionIndex)})
		}
	}

	return appearances, nil
}

func main() {
	db, err := sql.Open("sqlite3", "./txs-with-indexes.db")
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	// Go about inserting into the db!
	tx, err := db.Begin()
	if err != nil {
		log.Fatal(err)
	}
	addressesInsertStmt, err := tx.Prepare("INSERT OR IGNORE INTO addresses(address) VALUES(?)")
	if err != nil {
		log.Fatal(err)
	}
	defer addressesInsertStmt.Close()

	txsInsertStmt, err := tx.Prepare("insert into txs(txAddressID, blockIndex, transactionIndex) values( (SELECT addressID FROM addresses where addresses.address=?), ?, ?);")
	if err != nil {
		log.Fatal(err)
	}
	defer txsInsertStmt.Close()

	var path = "../data/index/unchained/mainnet/finalized"

	files, err := ioutil.ReadDir(path)
	if err != nil {
		log.Fatal(err)
	}
	// For each file, read in all the addresses, and their appearances
	var i = 0
	for _, f := range files {
		if strings.HasSuffix(f.Name(), ".gz") {
			continue
		}
		i++
		fmt.Println(f.Name(), i)

		var appearances, err = readFile(filepath.Join(path, f.Name()))

		for _, appearance := range appearances {
			// First we insert the address
			_, err = addressesInsertStmt.Exec(appearance.address)
			if err != nil {
				log.Fatal(err)
			}
			// Then, we insert the transaction
			_, err = txsInsertStmt.Exec(appearance.address, appearance.blockIndex, appearance.transactionIndex)
			if err != nil {
				log.Fatal(err)
			}
		}
		if i == 15 {
			break
		}
	}
	tx.Commit()
}
