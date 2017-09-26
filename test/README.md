# Tests Folder

This folder holds the results of the test cases that you may run from ./build by entering `make test`.

If you're running a fully-synchronized local node, all the tests should pass. If you are not running a local node, you may still run these tests, but many of them will fail.

As a fallback for those not running a local node, you may preceed the testing command with an environment variable that will cause QuickBlocks to 'fallback' to [Infura](http://infrua.io): `FALLBACK=infura make test`.
