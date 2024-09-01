package names

import (
	"encoding/csv"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func TestCrudIntegration(t *testing.T) {
	chain := utils.GetTestChain()
	tmpDirPath := filepath.Join(os.TempDir(), "trueblocks")
	if err := os.MkdirAll(tmpDirPath, 0777); err != nil {
		t.Fatal(err)
	}
	defer func() {
		if err := os.RemoveAll(tmpDirPath); err != nil {
			t.Fatal("os.RemoveAll:", err)
		}
	}()
	loadTestDatabase := func() *os.File {
		tempFile, err := os.OpenFile(
			filepath.Join(os.TempDir(), "trueblocks", "names_custom.tab"),
			os.O_RDWR|os.O_CREATE|os.O_TRUNC,
			0777,
		)
		if err != nil {
			t.Fatal(err)
		}
		return tempFile
	}
	tempFile := loadTestDatabase()
	customNames = map[base.Address]types.Name{}
	addrStr := "0x1f9090aae28b8a3dceadf281b0f12828e676c326"
	addr := base.HexToAddress(addrStr)
	expected := types.Name{
		Name:    "test name",
		Address: addr,
	}

	expected.IsCustom = true
	customNamesMutex.Lock()
	customNames[expected.Address] = expected
	customNamesMutex.Unlock()
	if err := writeCustomNames(tempFile); err != nil {
		t.Fatal(err)
	}

	tempFile.Seek(0, 0)
	r := csv.NewReader(tempFile)
	r.Comma = '\t'

	result, err := r.ReadAll()
	if err != nil {
		t.Fatal(err)
	}

	if value := result[1][1]; value != expected.Address.Hex() {
		t.Fatal("create: wrong address", value)
	}
	if value := result[1][2]; value != expected.Name {
		t.Fatal("create: wrong name", value)
	}

	// Read
	read := ReadName(DatabaseCustom, chain, addr)

	if value := read.Address; value != expected.Address {
		t.Fatal("read: wrong address", value)
	}
	if value := read.Name; value != expected.Name {
		t.Fatal("read: wrong name", value)
	}

	// Update
	updated, err := setIfExists(tempFile, &types.Name{
		Name:    "new name",
		Address: addr,
	})
	if err != nil {
		t.Fatal("update:", err)
	}
	if name := updated.Name; name != "new name" {
		t.Fatal("wrong name", name)
	}
	if addr := updated.Address.Hex(); addr != addrStr {
		t.Fatal("wrong address", addr)
	}

	// Delete
	deleted, err := changeDeleted(tempFile, addr, true)
	if err != nil {
		t.Fatal("delete:", err)
	}
	if deleted == nil {
		t.Fatal("delete: returned name is nil")
	}
	if !deleted.Deleted {
		t.Fatal("delete: delete flag not set")
	}

	// Undelete
	undeleted, err := changeDeleted(tempFile, addr, false)
	if err != nil {
		t.Fatal("undelete:", err)
	}
	if undeleted == nil {
		t.Fatal("undelete: returned name is nil")
	}
	if undeleted.Deleted {
		t.Fatal("undelete: delete flag not unset")
	}

	// Invalid remove
	// Commented out, because C++ doesn't check it
	// _, err = names.RemoveName(names.DatabaseCustom, tempFile, addr)
	// if err == nil {
	// 	t.Fatal("remove: expected error")
	// }

	// Remove
	// Set flag first
	_, err = changeDeleted(tempFile, addr, true)
	if err != nil {
		t.Fatal("remove: delete:", err)
	}
	removed, err := removeIfExists(tempFile, addr)
	if err != nil {
		t.Fatal("remove:", err)
	}
	if name := removed.Name; name != "new name" {
		t.Fatal("remove: wrong name", name)
	}
	if addr := removed.Address.Hex(); addr != addrStr {
		t.Fatal("remove: wrong address", addr)
	}

	// Check what was written to the file
	tempFile = loadTestDatabase()
	testDb, err := unmarshallCustomNames(tempFile, nil, types.Custom, &map[base.Address]types.Name{})
	if err != nil && !errors.Is(err, io.EOF) {
		t.Fatal("remove: unmarshallCustomNames:", err)
	}
	if _, ok := testDb[addr]; ok {
		t.Fatal("record was removed, but it is still present")
	}
}

func setIfExists(output *os.File, name *types.Name) (result *types.Name, err error) {
	if _, ok := customNames[name.Address]; !ok {
		return nil, fmt.Errorf("no custom name for address %s", name.Address.Hex())
	}

	name.IsCustom = true
	customNamesMutex.Lock()
	defer customNamesMutex.Unlock()
	customNames[name.Address] = *name
	return name, writeCustomNames(output)
}

func removeIfExists(output *os.File, address base.Address) (name *types.Name, err error) {
	found, ok := customNames[address]
	if !ok {
		return nil, fmt.Errorf("no custom name for address %s", address.Hex())
	}
	customNamesMutex.Lock()
	defer customNamesMutex.Unlock()

	delete(customNames, address)
	return &found, writeCustomNames(output)
}
