package cache

import (
	"bufio"
	"bytes"
	"os"
	"testing"
)

func TestWriteBlock(t *testing.T) {
	// first, read the input file
	expected, err := os.ReadFile("./cache_block.bin")
	if err != nil {
		t.Fatal(err)
	}
	br := bytes.NewReader(expected)
	inputReader := bufio.NewReader(br)

	// read block, so that we have structure to write
	block, err := ReadBlock((*bufio.Reader)(inputReader))
	if err != nil {
		t.Fatal("while reading file:", err)
	}

	// We will write to a buffer
	var writeBuf bytes.Buffer
	writer := bufio.NewWriter(&writeBuf)

	err = WriteBlock(writer, block)
	if err != nil {
		t.Fatal(err)
	}
	// Make sure we've written everything
	writer.Flush()
	result := writeBuf.Bytes()

	if !bytes.Equal(result, expected) {
		// o, _ := os.Create("/tmp/output.bin")
		// _, err := o.Write(result)
		// if err != nil {
		// 	t.Fatal(err)
		// }
		// o.Close()
		t.Fatal("not same", bytes.Compare(result, expected))
	}
}

// This is useful for debugging. The test opens a file containing
// paths to cache files, then opens each file, reads a structure from it
// and writes it back to a buffer, expecting the same binary data.
// func Test_CacheFiles(t *testing.T) {
// 	listF, err := os.Open("/tmp/list")
// 	if err != nil {
// 		t.Fatal(err)
// 	}
// 	defer listF.Close()

// 	scanner := bufio.NewScanner(listF)

// 	for scanner.Scan() {
// 		filename := scanner.Text()
// 		t.Log("-- file ", filename, " --")
// 		expected, err := os.ReadFile(path.Join("/Volumes/IndexCache/trueblocks/cache/mainnet", filename))
// 		if err != nil {
// 			t.Fatal(err)
// 		}
// 		br := bytes.NewReader(expected)
// 		inputReader := bufio.NewReader(br)

// 		block, err := ReadBlock(inputReader)
// 		if err != nil && err.Error() == "invalid schema" {
// 			log.Println("File too old")
// 			continue
// 		}

// 		if err != nil {
// 			t.Fatal("while reading", err)
// 		}
// 		var writeBuf bytes.Buffer
// 		writer := bufio.NewWriter(&writeBuf)

// 		err = WriteBlock(writer, block)
// 		if err != nil {
// 			t.Fatal("while writing", err)
// 		}
// 		// Make sure we've written everything
// 		writer.Flush()
// 		result := writeBuf.Bytes()
// 		if !bytes.Equal(result, expected) {
// 			o, _ := os.Create("/tmp/output.bin")
// 			_, err := o.Write(result)
// 			if err != nil {
// 				t.Fatal(err)
// 			}
// 			o.Close()
// 			t.Fatal("not same", filename)
// 		}
// 	}
// }
