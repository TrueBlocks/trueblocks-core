# Migrations

There was a near complete re-write of the entire repo between version 5.9.3 and 6.0.0. The repo version as of this document is v6.4.5. These instructions help you migrate to the new format

## How to migrate to v6.4.5

### Update Import Paths and Dependencies

The repository structure has been completely reorganized with proper Go module versioning. You'll need to update all import paths and `go.mod` dependencies as follows:

#### 1. Chifra Core Application

**Old import path:**
```go
github.com/TrueBlocks/trueblocks-core/src/apps/chifra
```

**New import path:**
```go
github.com/TrueBlocks/trueblocks-chifra/v6
```

Update this in all locations including:
- Go source file imports
- `go.mod` files
- Any scripts or configuration that reference the old path

#### 2. TrueBlocks SDK

**Old import path:**
```go
github.com/TrueBlocks/trueblocks-sdk/v5
```

**New import path:**
```go
github.com/TrueBlocks/trueblocks-sdk/v6
```

#### 3. Khedra Extension System

**Old import path:**
```go
github.com/TrueBlocks/trueblocks-khedra/v5
```

**New import path:**
```go
github.com/TrueBlocks/trueblocks-khedra/v6
```

### Development Tools Notice

The development tools (goMaker, testRunner, etc.) were internal-only tools and are now managed as separate repositories. We recommend using them at your own risk. **We do not provide migration paths for internal-use-only tools.**

### Migration Example

If you have a `go.mod` file that looks like this:
```go
module your-project

go 1.21

require (
    github.com/TrueBlocks/trueblocks-core/src/apps/chifra v0.0.0
    github.com/TrueBlocks/trueblocks-sdk/v5 v5.9.3
)
```

Update it to:
```go
module your-project

go 1.21

require (
    github.com/TrueBlocks/trueblocks-chifra/v6 v6.4.5
    github.com/TrueBlocks/trueblocks-sdk/v6 v6.4.5
)
```

And then make sure to update the import paths in your `.go` files.

## Reason for this change

The radical modifications to the codebase between version 5.9.3 and 6.4.5 were driven by three fundamental issues that required breaking changes:

### 1. Repository Versioning and Management

Chifra was previously unversioned, which made proper management of the repositories nearly impossible, especially when used in the mini-dApps. We always knew that at some point we would have to make this radical a change. We tried to implement a lesser change initially, but it became impossible at a certain point, thus necessitating the major breaking changes.

### 2. Branch Structure Modernization

The 5.9.3 and earlier releases used `master` as the main branch. They now use `main` as most other modern repos do. This was a long-ago naming convention that we knew we would one day have to fix to align with modern Git practices. Rather than making incremental changes, we decided to break everything all at once and migrate to `main` as the default branch.

### 3. Idiomatic Go Development

This repo was originally born as a C++ project. The new format is much more idiomatic GoLang. Our hope is to spend much less time managing what was essentially borken. All our development tools (testRunner, goMaker, scripts) are now much easier to manage, and our CI/CD processes are significantly improved.

