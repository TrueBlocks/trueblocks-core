//go:build integration
// +build integration

package file

import (
	"os"
	"testing"
)

func TestEnsureDirectoryExists(t *testing.T) {
	dir := "./test_dir"

	os.RemoveAll(dir)

	err := EstablishFolder(dir)
	if err != nil {
		t.Fatalf("ensureDirectoryExists() failed: %v", err)
	}

	if _, err := os.Stat(dir); os.IsNotExist(err) {
		t.Fatalf("Expected directory %s to be created, but it wasn't", dir)
	}

	os.RemoveAll(dir)
}

// func TestEnsureDirectoryExistsNonWritable(t *testing.T) {
// 	dir := "./test_dir"

// 	os.MkdirAll(dir, 0555)

// 	err := EstablishFolder(dir)
// 	if err == nil {
// 		t.Fatalf("Expected error due to non-writable directory, but got none")
// 	}

// 	os.RemoveAll(dir)
// }
