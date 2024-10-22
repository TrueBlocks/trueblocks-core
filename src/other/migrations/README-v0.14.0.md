# v0.14.0 goLang version of Chifra

---

(October 3, 2021)

## IMPORTANT NOTE

This migration has been retired. If your data is prior to this version, you must clear your cache and index folders and start from scratch

## Why the Change?

Version 0.14.0 was a major change to our code base, but requires only a minor migration, as no internal data structures have changed.

There was one breaking change to command line options as detailed below. Most likely, this change won't effect you. In fact, unless you've written shell scripts that use the `--verbose` option, you don't need to do anything.

The change involved a complete re-write of the `chifra` command line code in Go. Previously, all of this code was in C++ which made it very hard to maintain and especially hard to extend. By moving to Go, we can take advantage of one thing we've always wanted to take advantage of: concurrent code. Block data, it turns out, is amazing amenable to being processed in parallel. Here's a tracking issue: https://github.com/TrueBlocks/trueblocks-core/issues/1790

### Breaking Change to --verbose flag

The `--verbose` flag has been part of `chifra` from the beginning. You may still use it, but as of version 0.14.0, the `--verbose` command line option has been made a **swith** as opposed to a **flag**. This means you may no longer attach a value to the `--verbose` flag.

Previously, one could specify `--verbose N` where `N` indicated the log level. This usage will now report an error.

This is a breaking change and will break any previously-existing shell scripts or front-end applications using `--verbose N`.

## You're Finished

Please report any problems by creating an issue.

## Deprecated Command Line Options

- The `--report` option of `chifra config` has been removed. Simply run `chifra config` with no options for the same result.
- The `--init_all` option of `chifra pins` has been removed. Use `chifra init --all` instead.
- The globally available `--file:fn` option has been made consistent with other flags and no longer requires the colon (:).

## Previous Migration

[Click here](./README-v0.12.1.md) for the previous migration.
