package main

import (
	"log"
	"net/http"

	tb "github.com/Great-Hill-Corporation/trueblocks-core/src/go-apps/goServer/cmd"
)

func main() {
	log.Printf("Server started at port 8080")
	log.Fatal(http.ListenAndServe(":8080", tb.NewRouter()))
}
