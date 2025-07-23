# TrueBlocks Core Development Instructions

## Project Overview

TrueBlocks is a Go-based blockchain data indexing system that provides fast, local access to Ethereum transaction data through the "Unchained Index". The project consists of:

- **chifra**: Main CLI tool (`src/apps/chifra/`) built with Cobra framework
- **SDK**: Multi-language bindings (Go, TypeScript, Python) in `sdk/`
- **khedra**: Extension system for specialized indexing (`khedra/`)
- **examples**: Integration examples demonstrating SDK usage (`examples/`)

## Architecture Patterns

**Command Structure**: Each chifra command follows the pattern:
- Command definition in `src/apps/chifra/cmd/[command].go`
- Internal logic in `src/apps/chifra/internal/[command]/`
- Package-specific types in `src/apps/chifra/pkg/types/`

**Data Flow**: RPC → Cache → Index → Output
- RPC calls through `pkg/rpc/`
- Binary cache system in `pkg/cache/`
- Index operations in `pkg/index/`
- Output formatting in `pkg/output/`

**Base Types**: Core blockchain types in `pkg/base/`:
- `base.Address` for Ethereum addresses
- `base.Hash` for transaction/block hashes  
- `base.Wei` for precise ETH calculations
- `base.Timestamp` for block times

## Development Workflows

**Building**: Use CMake from `build/` directory or Go directly:
```bash
# Full build with CMake
cd build && make
# Go build only
cd src/apps/chifra && go build
```

**Testing**: Multi-layer approach:
- Unit tests: `go test ./...` in chifra directory
- Integration tests: `scripts/test-all.sh` 
- Golden file tests in `tests/gold/`
- Set `TEST_MODE=true` for test-specific behavior

**Code Generation**: 
- SDK bindings auto-generated from chifra commands
- Templates in `examples/templates/`
- Use `scripts/go-mod-tidy.sh` for dependency management

## Critical Conventions

**Error Handling**: Always use TrueBlocks patterns:
- `pkg/logger` for logging (not fmt.Print)
- `pkg/validate` for input validation
- Return structured errors, not bare strings

**Configuration**: 
- Config files in `~/.local/share/trueblocks/` or `~/Library/Application Support/TrueBlocks/`
- Use `pkg/config` for settings access
- Environment variables prefixed with `TB_`

**Data Structures**:
- Implement `String()` methods for custom types
- Use `pkg/types` for shared data structures
- Follow naming: `Simple[Type]` for basic structs, `[Type]` for full objects

**Multi-Chain Support**:
- Chain-specific configs in config directories
- Use `base.KnownChain` constants
- RPC endpoints configurable per chain

## SDK Integration

**Language Bindings**: 
- Go SDK is primary (`sdk/*.go`)
- TypeScript/Python generated from Go definitions
- All SDKs call chifra binary under the hood

**Examples Structure**:
- Each example is self-contained with `go.mod`
- Follow `examples/simple/` pattern for new examples
- Include both programmatic and CLI usage

## Common Pitfalls

- **Don't** use `fmt.Print` - use `pkg/logger`
- **Don't** hardcode chain IDs - use configuration
- **Don't** bypass cache system - use `pkg/cache` for performance
- **Don't** ignore `TEST_MODE` environment variable in test code
- **Always** validate addresses with `base.IsValidAddress()`
- **Always** handle Wei arithmetic with `base.Wei` type

## Key Files to Reference

- `src/apps/chifra/cmd/root.go` - CLI framework setup
- `src/apps/chifra/pkg/base/types.go` - Core type definitions  
- `examples/simple/main.go` - Basic SDK usage pattern
- `scripts/test-all.sh` - Complete testing workflow