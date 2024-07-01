package config

import (
	"bytes"
	"fmt"
	"testing"
)

func Test_write(t *testing.T) {
	expected := `[version]
  # Do not edit
  current = ''

[settings]
  # The location of the per chain caches
  cachePath = ''
  # The location of the per chain unchained indexes
  indexPath = ''
  # The default chain to use if none is provided
  defaultChain = 'mainnet'

  [settings.notify]
    url = ''
    author = ''

[pinning]
  # The pinning gateway to query when downloading the unchained index
  gatewayUrl = 'https://ipfs.unchainedindex.io/ipfs/'
  # The local endpoint for the IPFS daemon
  localPinUrl = 'http://localhost:5001'
  # The remote endpoint for pinning on Pinata
  remotePinUrl = 'https://api.pinata.cloud/pinning/pinFileToIPFS'

# Do not edit these values unless instructed to do so.
[unchained]
  # The default publisher of the index if none other is provided
  preferredPublisher = 'publisher.unchainedindex.eth'
  # The address of the current version of the Unchained Index
  smartContract = '0x0c316b7042b419d07d343f2f4f5bd54ff731183d'
`

	var buf bytes.Buffer
	if err := write(&buf, defaultConfig); err != nil {
		t.Fatal("writing:", err)
	}

	// Some nice error reporting just in case this test fails
	str := string(buf.Bytes())
	line := 0
	col := 1
	var correctPart string
	for index, char := range str {
		if byte(char) != expected[index] {
			t.Fatal("wrong value\n", correctPart, fmt.Sprintf("Line %d, column %d", line, col))
		}
		correctPart += string(char)
		col++
		if char == '\n' {
			line++
			col = 1
		}
	}
}
