package config

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/configtypes"
)

func Test_setByPath(t *testing.T) {
	var result struct {
		Int    int
		Uint   uint
		Bool   bool
		String string
		configtypes.Config
	}

	if err := setByPath(&result, []string{"INT"}, "42"); err != nil {
		t.Fatal(err)
	}
	if v := result.Int; v != 42 {
		t.Fatal("wrong value", v)
	}

	if err := setByPath(&result, []string{"UINT"}, "2"); err != nil {
		t.Fatal(err)
	}
	if v := result.Uint; v != 2 {
		t.Fatal("wrong value", v)
	}

	if err := setByPath(&result, []string{"BOOL"}, "true"); err != nil {
		t.Fatal(err)
	}
	if v := result.Bool; !v {
		t.Fatal("wrong value", v)
	}

	if err := setByPath(&result, []string{"STRING"}, "hello"); err != nil {
		t.Fatal(err)
	}
	if v := result.String; v != "hello" {
		t.Fatal("wrong value", v)
	}

	if err := setByPath(&result, []string{"CONFIG", "VERSION", "CURRENT"}, "v4.0.0"); err != nil {
		t.Fatal(err)
	}
	if v := result.Version.Current; v != "v4.0.0" {
		t.Fatal("wrong value", v)
	}

	if err := setByPath(&result, []string{"CONFIG", "KEYS", "PROVIDER", "APIKEY"}, "test-key"); err != nil {
		t.Fatal(err)
	}
	if v := result.Keys["provider"].ApiKey; v != "test-key" {
		t.Fatal("wrong value", v)
	}
	// Make sure we don't override the whole map when setting another key in already initialized map.
	if err := setByPath(&result, []string{"CONFIG", "KEYS", "PROVIDER", "SECRET"}, "secret"); err != nil {
		t.Fatal(err)
	}
	if v := result.Keys["provider"].ApiKey; v != "test-key" {
		t.Fatal("wrong value", v)
	}
	if v := result.Keys["provider"].Secret; v != "secret" {
		t.Fatal("wrong value", v)
	}

	// Make sure we don't override the whole map when setting another key in already initialized map.
	if err := setByPath(&result, []string{"CONFIG", "CHAINS", "MAINNET", "SCRAPE", "APPSPERCHUNK"}, "2000000"); err != nil {
		t.Fatal(err)
	}
	if v := result.Chains["mainnet"].Scrape.AppsPerChunk; v != 2000000 {
		t.Fatal("wrong value", v)
	}
}
