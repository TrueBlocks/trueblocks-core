package index

import (
	"strings"
	"testing"
)

func Test_calculateCid(t *testing.T) {
	r := strings.NewReader("hello world")
	cid, err := calculateCid(r)
	if err != nil {
		t.Fatal(err)
	}
	if s := cid.String(); s != "QmaozNR7DZHQK1ZcU9p7QdrshMvXqWK6gpu5rmrkPdT3L4" {
		t.Fatal("wrong CID:", s)
	}
}
