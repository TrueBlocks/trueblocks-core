package abi

import (
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func TestCleanAbiCache(t *testing.T) {
	// Create a temporary directory for testing
	tempDir := t.TempDir()

	tests := []struct {
		name          string
		chain         string
		path          string
		setupFiles    []string // files to create before test
		expectRemoved []string // files that should be removed
		expectExists  []string // files that should still exist
		expectError   bool
	}{
		{
			name:  "Remove v1 file and corresponding old file",
			chain: "mainnet",
			path:  filepath.Join(tempDir, "v1", "abis", "0x1234.bin"),
			setupFiles: []string{
				filepath.Join(tempDir, "v1", "abis", "0x1234.bin"),
				filepath.Join(tempDir, "abis", "0x1234.json"), // old format
			},
			expectRemoved: []string{
				filepath.Join(tempDir, "v1", "abis", "0x1234.bin"),
				filepath.Join(tempDir, "abis", "0x1234.json"),
			},
			expectExists: []string{},
		},
		{
			name:  "Remove v1 file when old file doesn't exist",
			chain: "mainnet",
			path:  filepath.Join(tempDir, "v1", "abis", "0x5678.bin"),
			setupFiles: []string{
				filepath.Join(tempDir, "v1", "abis", "0x5678.bin"),
			},
			expectRemoved: []string{
				filepath.Join(tempDir, "v1", "abis", "0x5678.bin"),
			},
			expectExists: []string{},
		},
		{
			name:  "Remove regular (non-v1) cache file",
			chain: "mainnet",
			path:  filepath.Join(tempDir, "0x9abc.json"),
			setupFiles: []string{
				filepath.Join(tempDir, "0x9abc.json"),
			},
			expectRemoved: []string{
				filepath.Join(tempDir, "0x9abc.json"),
			},
			expectExists: []string{},
		},
		{
			name:          "Handle non-existent file gracefully",
			chain:         "mainnet",
			path:          filepath.Join(tempDir, "nonexistent.json"),
			setupFiles:    []string{},
			expectRemoved: []string{},
			expectExists:  []string{},
		},
		{
			name:  "Remove v1 file with nested directory structure",
			chain: "mainnet",
			path:  filepath.Join(tempDir, "sub1", "v1", "abis", "sub2", "0xdef0.bin"),
			setupFiles: []string{
				filepath.Join(tempDir, "sub1", "v1", "abis", "sub2", "0xdef0.bin"),
				filepath.Join(tempDir, "sub1", "abis", "sub2", "0xdef0.json"), // corresponding old file
			},
			expectRemoved: []string{
				filepath.Join(tempDir, "sub1", "v1", "abis", "sub2", "0xdef0.bin"),
				filepath.Join(tempDir, "sub1", "abis", "sub2", "0xdef0.json"),
			},
			expectExists: []string{},
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			// Setup: Create test files
			for _, filePath := range tt.setupFiles {
				dir := filepath.Dir(filePath)
				if err := os.MkdirAll(dir, 0755); err != nil {
					t.Fatalf("Failed to create directory %s: %v", dir, err)
				}
				if err := os.WriteFile(filePath, []byte("test content"), 0644); err != nil {
					t.Fatalf("Failed to create test file %s: %v", filePath, err)
				}
			}

			// Execute the function
			err := CleanAbiCache(tt.chain, tt.path)

			// Check error expectation
			if tt.expectError && err == nil {
				t.Errorf("Expected error but got none")
			}
			if !tt.expectError && err != nil {
				t.Errorf("Unexpected error: %v", err)
			}

			// Check that expected files were removed
			for _, filePath := range tt.expectRemoved {
				if file.FileExists(filePath) {
					t.Errorf("Expected file %s to be removed, but it still exists", filePath)
				}
			}

			// Check that expected files still exist
			for _, filePath := range tt.expectExists {
				if !file.FileExists(filePath) {
					t.Errorf("Expected file %s to exist, but it was removed", filePath)
				}
			}
		})
	}
}

func TestCleanAbiCache_PathTransformation(t *testing.T) {
	tempDir := t.TempDir()

	tests := []struct {
		name            string
		v1Path          string
		expectedOldPath string
	}{
		{
			name:            "Transform v1 bin to old json",
			v1Path:          filepath.Join(tempDir, "v1", "abis", "0x1234.bin"),
			expectedOldPath: filepath.Join(tempDir, "abis", "0x1234.json"),
		},
		{
			name:            "Transform nested v1 path",
			v1Path:          filepath.Join(tempDir, "some", "v1", "abis", "nested", "0x5678.bin"),
			expectedOldPath: filepath.Join(tempDir, "some", "abis", "nested", "0x5678.json"),
		},
		{
			name:            "Transform complex nested v1 path",
			v1Path:          filepath.Join(tempDir, "cache", "mainnet", "v1", "abis", "0x9abc.bin"),
			expectedOldPath: filepath.Join(tempDir, "cache", "mainnet", "abis", "0x9abc.json"),
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			// Create both files
			v1Dir := filepath.Dir(tt.v1Path)
			oldDir := filepath.Dir(tt.expectedOldPath)

			if err := os.MkdirAll(v1Dir, 0755); err != nil {
				t.Fatalf("Failed to create v1 directory: %v", err)
			}
			if err := os.MkdirAll(oldDir, 0755); err != nil {
				t.Fatalf("Failed to create old directory: %v", err)
			}

			if err := os.WriteFile(tt.v1Path, []byte("v1 content"), 0644); err != nil {
				t.Fatalf("Failed to create v1 file: %v", err)
			}
			if err := os.WriteFile(tt.expectedOldPath, []byte("old content"), 0644); err != nil {
				t.Fatalf("Failed to create old file: %v", err)
			}

			// Run the function
			err := CleanAbiCache("mainnet", tt.v1Path)
			if err != nil {
				t.Fatalf("Unexpected error: %v", err)
			}

			// Verify both files were removed
			if file.FileExists(tt.v1Path) {
				t.Errorf("Expected v1 file %s to be removed", tt.v1Path)
			}
			if file.FileExists(tt.expectedOldPath) {
				t.Errorf("Expected old file %s to be removed", tt.expectedOldPath)
			}
		})
	}
}

func TestPathToAbisCache(t *testing.T) {
	tests := []struct {
		name           string
		chain          string
		fileName       string
		expectedSuffix string // We'll check that the result ends with this
		hasExtension   bool   // Whether we expect an extension to be added
	}{
		{
			name:           "Address filename gets extension",
			chain:          "mainnet",
			fileName:       "0x1234567890abcdef1234567890abcdef12345678",
			expectedSuffix: "0x1234567890abcdef1234567890abcdef12345678.json", // assuming .json is the ABI extension
			hasExtension:   true,
		},
		{
			name:           "Non-address filename unchanged",
			chain:          "mainnet",
			fileName:       "known.bin",
			expectedSuffix: "known.bin",
			hasExtension:   false,
		},
		{
			name:           "Empty filename",
			chain:          "mainnet",
			fileName:       "",
			expectedSuffix: "",
			hasExtension:   false,
		},
		{
			name:           "Different chain",
			chain:          "sepolia",
			fileName:       "0xabcdef1234567890abcdef1234567890abcdef12",
			expectedSuffix: "0xabcdef1234567890abcdef1234567890abcdef12.json",
			hasExtension:   true,
		},
		{
			name:           "Filename with existing extension",
			chain:          "mainnet",
			fileName:       "test.txt",
			expectedSuffix: "test.txt",
			hasExtension:   false,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			result := PathToAbisCache(tt.chain, tt.fileName)

			// Check that result ends with expected suffix
			if !strings.HasSuffix(result, tt.expectedSuffix) {
				t.Errorf("Expected result to end with %s, got %s", tt.expectedSuffix, result)
			}

			// Check that result contains the chain (via GetRootPathFromCacheType)
			// This is an indirect test since we can't easily mock the walk package
			if result == "" && tt.fileName != "" {
				t.Errorf("Expected non-empty result for non-empty filename")
			}

			// Check that path is absolute (assuming GetRootPathFromCacheType returns absolute path)
			if tt.fileName != "" && !filepath.IsAbs(result) {
				t.Errorf("Expected absolute path, got %s", result)
			}

			// For address filenames starting with 0x, verify extension was added
			if strings.HasPrefix(tt.fileName, "0x") && tt.hasExtension {
				if !strings.Contains(result, ".") {
					t.Errorf("Expected extension to be added to address filename, got %s", result)
				}
			}
		})
	}
}

func TestPathToAbisCache_ExtensionHandling(t *testing.T) {
	// Test that the extension comes from walk.CacheTypeToExt[walk.Cache_Abis]
	result1 := PathToAbisCache("mainnet", "0x1234")
	result2 := PathToAbisCache("mainnet", "0x1234")

	// Both should be identical
	if result1 != result2 {
		t.Errorf("Expected consistent results, got %s and %s", result1, result2)
	}

	// Should contain an extension for 0x prefixed files
	if !strings.Contains(result1, ".") {
		t.Errorf("Expected extension to be added, got %s", result1)
	}
}

func TestPathToKnownAbis(t *testing.T) {
	tests := []struct {
		name           string
		fileName       string
		expectedSuffix string
	}{
		{
			name:           "Simple filename",
			fileName:       "erc20.json",
			expectedSuffix: filepath.Join("abis", "erc20.json"),
		},
		{
			name:           "Empty filename",
			fileName:       "",
			expectedSuffix: "abis",
		},
		{
			name:           "Nested filename",
			fileName:       "tokens/usdc.json",
			expectedSuffix: filepath.Join("abis", "tokens", "usdc.json"),
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			result := PathToKnownAbis(tt.fileName)

			// Check that result ends with expected suffix
			if !strings.HasSuffix(result, tt.expectedSuffix) {
				t.Errorf("Expected result to end with %s, got %s", tt.expectedSuffix, result)
			}

			// Check that result is absolute path (assuming config.PathToRootConfig() returns absolute)
			if !filepath.IsAbs(result) {
				t.Errorf("Expected absolute path, got %s", result)
			}

			// Check that it contains "abis" directory
			if !strings.Contains(result, "abis") {
				t.Errorf("Expected path to contain 'abis' directory, got %s", result)
			}
		})
	}
}

// TestCleanAbiCache_DirectoryCleanup tests that empty directories are properly cleaned up
func TestCleanAbiCache_DirectoryCleanup(t *testing.T) {
	tempDir := t.TempDir()

	// Create a nested directory structure with a file at the end
	nestedPath := filepath.Join(tempDir, "level1", "level2", "level3", "test.json")
	if err := os.MkdirAll(filepath.Dir(nestedPath), 0755); err != nil {
		t.Fatalf("Failed to create nested directories: %v", err)
	}
	if err := os.WriteFile(nestedPath, []byte("test"), 0644); err != nil {
		t.Fatalf("Failed to create test file: %v", err)
	}

	// Run CleanAbiCache
	err := CleanAbiCache("mainnet", nestedPath)
	if err != nil {
		t.Fatalf("Unexpected error: %v", err)
	}

	// File should be removed
	if file.FileExists(nestedPath) {
		t.Errorf("Expected file to be removed")
	}

	// Note: We can't easily test the directory cleanup without mocking file.CleanFolderIfEmpty
	// or having access to the actual cache root path. This would require integration with
	// the actual walk package functionality.
}
