package main

import (
	"log"
	"net/http"

	// WARNING!
	// Change this to a fully-qualified import path
	// once you place this file into your project.
	// For example,
	//
	//    sw "github.com/myname/myrepo/go"
	//
	tb "./cmd"
)

func main() {
	log.Printf("Server started at port 8080")
	tb.RunWebsocketPool()
	log.Fatal(http.ListenAndServe(":8080", tb.NewRouter()))
}
