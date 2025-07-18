# Block Time Configuration

This document describes the block time configuration system introduced to fix issue #3974 where timestamps on Optimism (and other chains) were incorrect due to hardcoded Ethereum mainnet block times.

## Problem

Previously, TrueBlocks used hardcoded values (13 or 13.3 seconds) for block time calculations across all chains. This caused incorrect timestamp-to-block conversions for chains with different block times like Optimism (2 seconds), Polygon (2 seconds), etc.

## Solution

### 1. Configuration Structure

Added a `blockTime` field to the `ChainGroup` structure in `pkg/configtypes/chain_group.go`:

```go
type ChainGroup struct {
    // ... existing fields ...
    BlockTime float64 `json:"blockTime" toml:"blockTime,omitempty"`
    // ... rest of fields ...
}
```

### 2. Block Time Helper

Created `pkg/config/blocktime.go` with:
- Default block times for known chains
- `GetBlockTime(chain string)` function that:
  - First checks configuration file
  - Falls back to known defaults
  - Uses Ethereum mainnet (13s) as ultimate fallback

### 3. Updated Calculations

Updated hardcoded values in:
- `pkg/tslib/tsdb.go`: Line 100
- `pkg/identifiers/resolve.go`: Line 201

Both now use `config.GetBlockTime(chain)` instead of hardcoded values.

### 4. Configuration

Add block times to your `trueBlocks.toml`:

```toml
[chains.mainnet]
blockTime = 13.0

[chains.optimism]
blockTime = 2.0

[chains.polygon]
blockTime = 2.0
```

## Default Block Times

| Chain | Block Time (seconds) |
|-------|---------------------|
| Ethereum Mainnet | 13.0 |
| Optimism | 2.0 |
| Polygon | 2.0 |
| BSC | 3.0 |
| Gnosis | 5.0 |
| Arbitrum | 0.25 |
| Base | 2.0 |
| Avalanche | 2.0 |
| zkSync Era | 1.0 |

See `pkg/config/blocktime.go` for the complete list.

## Testing

Run tests with:
```bash
go test ./pkg/config/blocktime_test.go
```

## Migration

No action required for existing installations. The system will use defaults if `blockTime` is not configured.