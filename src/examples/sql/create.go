package main

import "fmt"

func main() {
    fmt.Println("Temporarily disabled")
}

/*
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

// Deletes the existing database in this file
func deleteDatabase(databasePath string) {
	fmt.Println("Deleting database", databasePath)
	os.Remove(databasePath)
}

// Creates an empty SQLite database in an space-optimized format
// that allows for massively quick querying of a specific address
// to get all the (blockIndex, transactionIndex) pairs it's been
// involved in
func createEmptyDatabase(databasePath string) {
	db, err := sql.Open("sqlite3", databasePath)
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	sqlStmt := `
	CREATE TABLE addresses (
		addressID 	INTEGER NOT NULL PRIMARY KEY,
		address    TEXT NOT NULL,
		UNIQUE(address)
	);
	CREATE UNIQUE INDEX idx_addresses_address on addresses(address);
	CREATE TABLE IF NOT EXISTS txs (
		txAddressID INTEGER,
		blockIndex INT, 
		transactionIndex INT,
		FOREIGN KEY(txAddressID) REFERENCES addresses(addressID)
	);
	CREATE INDEX IF NOT EXISTS idx_txs_address ON txs (txAddressID);
	`
	fmt.Println("Creating DB")
	_, err = db.Exec(sqlStmt)
	if err != nil {
		log.Printf("%q: %s\n", err, sqlStmt)
		return
	}
}

type Appearance struct {
	address          string
	blockIndex       int
	transactionIndex int
}

// Given a TrueBlocks index file in the format to be found at the
// link below, this file returns all the appearances that are in that
// specific chunk of the index
// https://gateway.pinata.cloud/ipfs/Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2
func readFile(path string) ([]Appearance, error) {
	var appearances []Appearance

	f, err := os.Open(path)
	if err != nil {
		panic(err)
	}
	defer f.Close()

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

	for i := 0; int64(i) < int64(nAddresses); i++ {
		// TODO: I am not sure this hex encoding is correct, so check it
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

// Fills a database with the first X chunks of the index, for testing
func fillDatabase(databasePath string, indexFolderPath string) {
	db, err := sql.Open("sqlite3", databasePath)
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	tx, err := db.Begin()
	if err != nil {
		log.Fatal(err)
	}
	addressesInsertStmt, err := tx.Prepare("INSERT OR IGNORE INTO addresses(address) VALUES(?)")
	if err != nil {
		log.Fatal(err)
	}
	defer addressesInsertStmt.Close()

	txsInsertStmt, err := tx.Prepare("INSERT INTO txs(txAddressID, blockIndex, transactionIndex) VALUES( (SELECT addressID FROM addresses WHERE addresses.address=?), ?, ?);")
	if err != nil {
		log.Fatal(err)
	}
	defer txsInsertStmt.Close()

	files, err := ioutil.ReadDir(indexFolderPath)
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

		var appearances, err = readFile(filepath.Join(indexFolderPath, f.Name()))

		for _, appearance := range appearances {
			// First we insert the address, if it needs to be inserted
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

// Deletes existing database, creates a new one, and fills it
// with the index data
func main() {
	// Set this as the name of the sqlite database file you want to create
	const databasePath = "./txs-with-indexes.db"
	// Set this as the path to the finalized index chunks
	const indexFolderPath = "../data/index/unchained/mainnet/finalized"

	// Delete the database
	deleteDatabase(databasePath)

	// Create a new database
	createEmptyDatabase(databasePath)

	// Fill the new database
	fillDatabase(databasePath, indexFolderPath)
}
*/
