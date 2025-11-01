package utils

import (
	"log"
	"path/filepath"
	"runtime"
	"strings"
)

func Trace() {
	pc := make([]uintptr, 10) // at least 1 entry needed
	runtime.Callers(2, pc)
	f := runtime.FuncForPC(pc[0])
	file, line := f.FileLine(pc[0])
	_, fn := filepath.Split(file)
	n := strings.Replace(f.Name(), "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5", ".", 1)
	log.Printf("%s:%d %s\n", fn, line, n)
}
