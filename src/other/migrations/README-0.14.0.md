# v0.14.0 Go Version of Chifra

---

(v0.14.0 - 2021/09/28)

## Why the Change?

Version 0.14.0 is a major change to our code base, but a minor migration, as no internal data structures changed.

There was only one breaking change to command line options, but it was relatively obscure, so most likely it won't effect you. In fact, unless you've written shell scripts that use the `--verbose` option, you don't need to do anything.

The change involved a complete re-write of the `chifra` command line code in Go. Previously, all of this code was in C++ which made it very hard to maintain and especially hard to extend. By moving to Go, we can take advantage of one thing we've always wanted to take advantage of: concurrent code. Block data, it turns out, is amazing amenable to being processed in parallel. Here's a tracking issue: https://github.com/TrueBlocks/trueblocks-core/issues/1790

### Breaking Change to --verbose flag

It has always been possible to add a `--verbose` flag to any chifra command. This is still the case, but as of version 0.14.0, the `--verbose` flag has been made a **swith**. This means you may no longer add a value after the `--verbose` flag.

Previously, you could specify `--verbose N` where `N` was the log level. This will now report an error.

To accomplish the same thing, do `--log_level N`. You will get the same result as previous.

The only way to use the `--verbose` flag is to include it or omit it, you may no longer attach a number.

This change will break any previously-existing shell scripts or front end applications that used `--verbose N`.

## You're Finished

Please report any problems by creating an issue.

## Deprecated Command Line Options

The `--report` option of `chifra status` has been removed. Simply run `chifra status` with no options for the same result.
The `--init_all` option of `chifra pins` has been removed. Use `chifra pins --init --all` instead.
The `--file:fn` option has been made consistent with other Go language flags and no longer requires the colon (:).
