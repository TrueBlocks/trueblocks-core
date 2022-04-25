package main

import (
	"database/sql"
	"fmt"
	"log"
	"os"

	_ "github.com/mattn/go-sqlite3"
)

func main() {
	fmt.Println("Remving DB")
	os.Remove("./txs-with-indexes.db")

	db, err := sql.Open("sqlite3", "./txs-with-indexes.db")
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	sqlStmt := `
	CREATE TABLE IF NOT EXISTS txs (
		id INTEGER PRIMARY KEY, 
		address TEXT, 
		blockIndex INT, 
		transactionIndex INT
	);
	CREATE INDEX IF NOT EXISTS idx_address ON txs (address);
	`
	fmt.Println("Creating DB")
	_, err = db.Exec(sqlStmt)
	if err != nil {
		log.Printf("%q: %s\n", err, sqlStmt)
		return
	}
	fmt.Println("Created DB")
}
