package config

import (
	"testing"
)

func Test_setByPath(t *testing.T) {
	var result struct {
		Int    int
		Uint   uint
		Bool   bool
		String string
		ConfigFile
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

	if err := setByPath(&result, []string{"CONFIGFILE", "VERSION", "CURRENT"}, "v3.0.0-release"); err != nil {
		t.Fatal(err)
	}
	if v := result.Version.Current; v != "v3.0.0-release" {
		t.Fatal("wrong value", v)
	}

	if err := setByPath(&result, []string{"CONFIGFILE", "KEYS", "PROVIDER", "APIKEY"}, "test-key"); err != nil {
		t.Fatal(err)
	}
	if v := result.Keys["provider"].ApiKey; v != "test-key" {
		t.Fatal("wrong value", v)
	}
	// Make sure we don't override the whole map when setting another key in already initialized map.
	if err := setByPath(&result, []string{"CONFIGFILE", "KEYS", "PROVIDER", "SECRET"}, "secret"); err != nil {
		t.Fatal(err)
	}
	if v := result.Keys["provider"].ApiKey; v != "test-key" {
		t.Fatal("wrong value", v)
	}
	if v := result.Keys["provider"].Secret; v != "secret" {
		t.Fatal("wrong value", v)
	}

	// Make sure we don't override the whole map when setting another key in already initialized map.
	if err := setByPath(&result, []string{"CONFIGFILE", "CHAINS", "MAINNET", "SCRAPE", "APPSPERCHUNK"}, "2000000"); err != nil {
		t.Fatal(err)
	}
	if v := result.Chains["mainnet"].Scrape.AppsPerChunk; v != 2000000 {
		t.Fatal("wrong value", v)
	}
}
