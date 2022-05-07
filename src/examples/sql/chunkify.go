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
	"time"

	_ "github.com/mattn/go-sqlite3"
)

// Copied from create.go b/c idk how to import
func deleteDatabase(databasePath string) {
	fmt.Println("Deleting database", databasePath)
	os.Remove(databasePath)
}

// Copied from create.go b/c idk how to import
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
	db, err := sql.Open("sqlite3", "./txs-with-indexes.db")
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	// We break up the database into chunks of 1M blocks, but you could
	// easily do this with a dynamic number that aims for some number of
	// appearances. You'd just sort by block number, and then do some
	// really basic SQL scripts. Note how quickly you can do this!
	for i := 0; i < 5; i++ {
		var chunkDBPath = fmt.Sprintf("chunk%d.db", i)

		// Delete and then create an empty database
		deleteDatabase(chunkDBPath)
		createEmptyDatabase(chunkDBPath)

		time.Sleep(10000)

		tx, err := db.Begin()
		if err != nil {
			log.Fatal(err)
		}
		if i >= 1 {
			_, err = tx.Exec("DETACH DATABASE chunk_database;")
			if err != nil {
				log.Fatal(err)
			}
		}
		_, err = tx.Exec(fmt.Sprintf("ATTACH DATABASE \"%s\" AS chunk_database; INSERT INTO chunk_database.addresses SELECT * FROM addresses; INSERT INTO chunk_database.txs SELECT * FROM txs where blockIndex >= %d AND blockIndex < %d;", chunkDBPath, i*1_000_000, (i+1)*1_000_000))
		if err != nil {
			log.Fatal(err)
		}

		tx.Commit()
	}
}
*/
