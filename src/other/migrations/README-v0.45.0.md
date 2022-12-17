# v0.45.0 Better Docker Support

(December 16, 2022)

## Why the Change

In this version we modifed two tools in order to better support our docker version. These two tools are

1) `chifra serve` which was changed to `chifra daemon`, and
2) `chifra status` wich was changed to `chifra config`.

Both of these changes will make supporting our Docker version easier, in particular, the change to `chifra daemon` as this will allow us to open multiple long running tools under a single process.

The migrations are simple.

## The Migrations

1) In any code using `chifra` where you use `chifra status`, replace the same with `chifra config`. Also, insert the `--module` flag in front of any positional values specifying a data type. Add `show` as a positional value to the command.

2) Whereever you previously used `chifra serve`, use `chifra daemon`.

All other options for all other tools are the same.

For both changes, the old tool reports that you must use the new tool.

## What if it doesn't work

Don't worry. It will work.

## You're finished

You're finished.

## Previous Migration

[Click here](./README-v0.44.0.md) for the previous migration.
