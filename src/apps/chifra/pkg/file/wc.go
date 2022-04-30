package file

import (
	"bufio"
	"bytes"
	"io"
	"os"
)

func WordCount(fileName string, ignoreHeader bool) (int, error) {
	// fmt.Println("-------------------------------------------")
	// fmt.Println(fileName, FileExists(fileName))
	// fmt.Println("-------------------------------------------")
	r, _ := os.Open(fileName)
	defer r.Close()

	var count int
	const lineBreak = '\n'

	buf := make([]byte, bufio.MaxScanTokenSize)

	for {
		bufferSize, err := r.Read(buf)
		if err != nil && err != io.EOF {
			return 0, err
		}

		var buffPosition int
		for {
			i := bytes.IndexByte(buf[buffPosition:], lineBreak)
			if i == -1 || bufferSize == buffPosition {
				break
			}
			buffPosition += i + 1
			count++
		}
		if err == io.EOF {
			break
		}
	}

	if ignoreHeader && count > 0 {
		count--
	}
	return count, nil
}
