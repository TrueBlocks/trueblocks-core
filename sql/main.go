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
	os.Remove("./txs.db")

	db, err := sql.Open("sqlite3", "./txs.db")
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	sqlStmt := `
	create table txs (address string, blockIndex integer, transactionIndex integer);
	delete from txs;
	`
	fmt.Println("Creating DB")
	_, err = db.Exec(sqlStmt)
	if err != nil {
		log.Printf("%q: %s\n", err, sqlStmt)
		return
	}

	tx, err := db.Begin()
	if err != nil {
		log.Fatal(err)
	}
	stmt, err := tx.Prepare("insert into txs(address, blockIndex, transactionIndex) values(?, ?, ?)")
	if err != nil {
		log.Fatal(err)
	}
	defer stmt.Close()

	_, err = stmt.Exec("aaaaaaaaaaaaaaaaaaaa", 1, 1)
	_, err = stmt.Exec("aaaaaaaaaaaaaaaaaaab", 2, 2)
	if err != nil {
		log.Fatal(err)
	}
	tx.Commit()

	rows, err := db.Query("select address, blockIndex, transactionIndex from txs")
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

	stmt, err = db.Prepare("select blockIndex from txs where address = ?")
	if err != nil {
		log.Fatal(err)
	}
	defer stmt.Close()
	var blockIndex string
	err = stmt.QueryRow("aaaaaaaaaaaaaaaaaaab").Scan(&blockIndex)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(blockIndex)
}
