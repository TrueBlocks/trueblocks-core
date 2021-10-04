dataUpgradeTest argc: 1 
dataUpgradeTest 
Purpose:
  Test upgrading binary data to new format.

Usage:
  dataUpgradeTest [flags] mode

Arguments:
  mode - either a number between 0 and 5, a C++ TrueBlocks class name, the word 'upgradeTest' (required)

Flags:

Global Flags:
  -x, --fmt string     export format, one of [none|json*|txt|csv|api]
  -v, --verbose uint   set verbose level (optional level defaults to 1)
  -h, --help           display this help screen
