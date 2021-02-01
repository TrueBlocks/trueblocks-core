package main

import (
	"fmt"
	"log"
	"net/http"

	"sync"

	"github.com/Great-Hill-Corporation/trueblocks-core/src/go-apps/goServer/cmd"
)

type countHandler struct {
	mu sync.Mutex // guards n
	n  int
}

func (h *countHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	h.mu.Lock()
	defer h.mu.Unlock()
	h.n++
	fmt.Fprintf(w, "count is %d\n", h.n)
	log.Println("Done serving that one.")
}

func main() {
	log.Println("Serving on port 8080...")
	http.Handle("/count", new(countHandler))
//	thing.Thing()
	log.Fatal(http.ListenAndServe(":8080", nil))
	cmd.Show()
}
