package main

import (
	"database/sql"
	"fmt"
	"log"

	_ "github.com/mattn/go-sqlite3"
)

func main() {
	db, err := sql.Open("sqlite3", "./txs-with-indexes.db")
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	rows, err := db.Query("select address, blockIndex, transactionIndex from txs where address = '1776281d0d40108941cb41e604dcafbdc3cd61a1'")
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
