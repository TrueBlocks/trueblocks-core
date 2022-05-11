package main

import "fmt"

func main() {
    fmt.Println("Temporarily disabled")
}

/*
import (
	"database/sql"
	"fmt"
	"log"
	"os"

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

func main() {
	var joinedChunksDBPath = "./merged-chunks.db"

	os.Remove(joinedChunksDBPath)
	db, err := sql.Open("sqlite3", joinedChunksDBPath)
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	for i := 0; i < 5; i++ {
		var chunkDBPath = fmt.Sprintf("chunk%d.db", i)

		chunkDB, err := sql.Open("sqlite3", chunkDBPath)
		defer chunkDB.Close()

		rows, err := chunkDB.Query(fmt.Sprintf("select txAddressID, blockIndex, transactionIndex from txs where txAddressID = (SELECT addressID from addresses where addresses.address = '%s')", address))
		if err != nil {
			log.Fatal(err)
		}
		defer rows.Close()
		for rows.Next() {
			var address string
			var blockIndex int
			var transactionIndex int
			err = rows.Scan(&address, &blockIndex, &transactionIndex)
			if err != nil {
				log.Fatal(err)
			}
			fmt.Println(address, blockIndex, transactionIndex)
		}
		err = rows.Err()
		if err != nil {
			log.Fatal(err)
		}
	}
}
*/
