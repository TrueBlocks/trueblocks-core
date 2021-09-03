# NOT COMPLETE

# v0.12.1 New Folder Structure

**Warning:** If you're not confident using the command line, please contact us prior to proceeding. If you make a mistake and delete the wrong files, you may damage your computer.

**DO NOT SAY WE DIDN'T WARN YOU.**

---

(v0.12.1 - 2021/09/02)

## Why the Change?

Several users reported problems with their $PATH variables, so we decided to remove the problem removing the `chifra` subcommands from the $PATH. The `chifra` executable still needs to be in your $PATH, however.

This change allows us to begin the process of committing to a public command line interface as we prepare to leave beta status. This also allows us greater freedom in changing underlying non-public interfaces, it forces users who may be using the underlying tools direclty to switch to the `chifra` command line only, and it allows us to start freezing our documentation.

You will not have to change anything unless you're using the sub-commands direclty (such as `ethNames` or `ethslurp`). In that case, you will have to switch to using the corresponding `chifra` command instead. (In this example, `chifra names` or `chifra slurp`.)

## Migrating to v0.12.1

Complete the following steps from the `./trueblocks-core/build` folder:

```[bash]
# remove previously built executables from the local bin
rm -fR ../bin/*

# reset the bin to its default configuration
git checkout ../bin

# clean out previously created cmake build scripts
make clean

# re-create the build scripts
cmake ../src

# build the trueblocks executables as normal
make -j2
```

That's it!

You should now be able to use `chifra` identically as you did before.

## You're Finished

Please report any problems by creating an issue.
