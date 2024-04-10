## Bumping the version

In `develop` branch:

|                                           |                                                  |
| ----------------------------------------- | ------------------------------------------------ |
| git checkout develop                      | # go to the develop branch                       |
| git pull                                  | # make sure you have the latest code             |
| git checkout -b feature/version-bump      | # create a new branch for the version bump       |
| edit ../VERSION                           | # bump the version number (for goMaker)          |
| edit ../src/dev_tools/utillib/version.cpp | # bump the version number (for testRunner)       |
| make test-all                             | # resolve all tests                              |
| git commit -am "Bumps version to..."      | # mark the commit as final commit before merging |
| git push                                  | # push the develop branch to remote              |

Merge to `develop`, let tests pass there, merge to `master`, let tests pass there, and then tag
the commit with the version number and announce the release.
