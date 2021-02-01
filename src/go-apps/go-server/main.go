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
	port := "8080"
	log.Printf("Server started at port " + port)
	router := tb.NewRouter()
	log.Fatal(http.ListenAndServe(":"+port, router))
}
