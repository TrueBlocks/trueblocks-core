package main

import (
	"database/sql"
	"fmt"
	"log"

	_ "github.com/mattn/go-sqlite3"
)

func list() {
	db, err := sql.Open("sqlite3", "./txs-with-indexes.db")
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	for {
		// Get the address from the user
		// NOTE: if you're looking for a fun text, enter: 1776281d0d40108941cb41e604dcafbdc3cd61a1
		var address string
		fmt.Print("Insert the new address to search: ")
		fmt.Scanln(&address)

		rows, err := db.Query(fmt.Sprintf("select txAddressID, blockIndex, transactionIndex from txs where txAddressID = (SELECT addressID from addresses where addresses.address = '%s')", address))
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
