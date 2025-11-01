# TrueBlocks Core Development Instructions

## Operational Modes

### Step-by-Step Mode (Careful Implementation)
When I say "We want to go into step-by-step mode" or similar, switch to these rules:

🚫 **Never Run Commands Without Permission:**
- `make clean && make all`
- `make examples`
- `make generate`
- `go build` or `go test`
- Any script execution

🛑 **Stop Between Steps:**
- Never run multiple operations consecutively
- Stop after each step for review and approval
- Wait for explicit "go ahead" before proceeding

📋 **Planning Process:**
- Show what I want to do WITHOUT making changes first
- Explain WHY each change is needed
- Wait for approval before making any code or file changes
- One step at a time with review

🔒 **PERSISTENCE RULE (CRITICAL):**
- **ONCE IN STEP-BY-STEP MODE, STAY THERE INDEFINITELY**
- Do NOT fall back to normal mode unless explicitly told "exit step-by-step mode"
- Step-by-step mode persists across multiple requests and conversations
- Every action must be approved individually, no exceptions

### Design Mode (Discussion + Analysis)
When I say "Let's go into design mode" or similar, switch to these rules:

📋 **Lightweight Discussion:**
- NO full codebase scans or comprehensive exploration upfront
- NO telling user about current code state unless specifically asked
- Discussion-focused mode for architectural analysis and planning

🔍 **Just-in-Time File Reading:**
- Before discussing ANY specific file, component, or implementation detail, ALWAYS read that file first
- Check file contents immediately before referencing to ensure accuracy
- Never assume file contents based on previous knowledge - files change between requests
- Only read files when discussion requires specific implementation details

💭 **Discussion + Analysis:**
- Architectural analysis and conceptual discussion
- NO code modifications, builds, tests, or implementations
- Focus on design decisions, trade-offs, and approaches
- Reference actual code patterns only when files have been read

🎯 **Focus Areas:**
- Go architecture decisions and package design
- Build system and CMake considerations
- Module versioning and dependency management
- Testing strategies and integration approaches
- SDK generation and multi-language binding patterns

📋 **Output Format:**
- Bullet points and structured analysis
- Pros/cons of different approaches
- Clear recommendations with reasoning
- Questions to clarify requirements

🔒 **NO MODIFICATION RULE:**
- **ONCE IN DESIGN MODE, STAY THERE INDEFINITELY**
- Must explicitly exit design mode before making any code changes
- Do NOT fall back to normal mode unless explicitly told "exit design mode"
- Design mode persists across multiple requests and conversations
- If asked to implement something in design mode, respond: "Still in design mode - should I exit design mode and implement this?"

### Mode Switching Rules (CRITICAL)
- **MUTUALLY EXCLUSIVE**: You can NEVER be in both design mode and step-by-step mode simultaneously
- **Only two ways to exit each mode:**
  1. Explicit command: "exit [mode-name] mode"
  2. Command to enter the other mode: "go into [other-mode] mode"
- **Mode persistence**: Once in a mode, stay there across all requests and conversations until explicitly changed

### Normal Mode (Default)
- Full autonomy to make changes, run builds, execute tests
- Use judgment to determine appropriate actions
- Can run multiple commands and make multiple file changes
- Default mode when no other mode is active

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
# Go build only (always use -o /tmp to avoid workspace clutter)
cd src/apps/chifra && go build -o /tmp/chifra
# For other modules
go build -o /tmp/[binary-name] .
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