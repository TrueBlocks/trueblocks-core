ADD SECTION ON --file:option


# v0.14.0 Go Version of Chifra

---

(v0.14.0 - 2021/09/28)

## Why the Change?

This is a major change to our code base, but a minor migration as no data structures changed. There is one breaking change to the `--verbose` option that only applies if you're using `chifra` in a script. If not, you may ignore this notice.

### Breaking Change to --verbose flag

Users have always been able to add a `--verbose` flag to any chifra command. This is still the case, but there has been a breaking change. Previously, you were able to specify a `log_level` with the `--verbose` command, but that is not longer the case. With this version `--verbose` has been made a `boolean` flag, which means it no longer accepts a `log_level`. A new command line option has been added to all commands to allows specifying detail level when debugging.

If you previous had shell scripts or code that used a command option such as this:

```
--verbose <val>
```

where `<val>` was some integer. You must replace that with:

```
--log_level <val>
```

You may still use the `--verbose` flag, but it will no longer accept a `log_level` value. An error will be reported.

## You're Finished

Please report any problems by creating an issue.

## Deprecated Command Line Options

The `--report` option of `chifra status` has been removed. Simply run `chifra status` with no options for the same result.
The `--init_all` option of `chifra pins` has been removed. Use `chifra pins --init --all` instead.
