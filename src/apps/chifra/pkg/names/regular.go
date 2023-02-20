package names

import (
	"io"
	"log"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// loadRegularMap loads the regular names from the cache
func loadRegularMap(chain string, thePath string, terms []string, parts Parts, ret *map[types.Address]types.SimpleName) error {
	gr, err := NewNameReader(thePath)
	if err != nil {
		log.Fatal(err)
	}

	for {
		n, err := gr.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			log.Fatal(err)
		}
		if doSearch(&n, terms, parts) {
			s := n.ToSimpleName()
			(*ret)[s.Address] = s
		}
	}

	return nil

	// callbackFunc := func(n Name) error {
	// 	if doSearch(&n, terms, parts) {
	// 		s := n.ToSimpleName()
	// 		fmt.Println(n)
	// 		(*ret)[n.Address] = s
	// 	}
	// 	return nil
	// }

	// errorHandler := func(err *csv.ParseError) bool {
	// 	fmt.Println(err)
	// 	return false // true
	// }

	// gocsv.TagSeparator = "\t"
	// if theFile, err := os.OpenFile(thePath, os.O_RDWR|os.O_CREATE, os.ModePerm); err != nil {
	// 	return err
	// } else {
	// 	defer theFile.Close()
	// 	result := make([]*Name, 0, 10000)
	// 	if err := gocsv.UnmarshalFileWithErrorHandler(theFile, errorHandler, &result); err != nil {
	// 		fmt.Println("gocsv error:", err)
	// 		// return err
	// 	}
	// 	for _, n := range result {
	// 		fmt.Println(n)
	// 		if doSearch(n, terms, parts) {
	// 			s := n.ToSimpleName()
	// 			(*ret)[n.Address] = s
	// 		}
	// 	}
	// }

	// return nil
}
