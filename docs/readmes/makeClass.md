## makeClass

`makeClass` is development-only tool that aids in various aspects of building TrueBlocks. The program is disabled for most users, but for core developers it can be used to automatically generate help files, API interface specs, command line options parsers and many of the test case files. Generally, regular users will not use `makeClass`.

Purpose:
  Automatically writes C++ for various purposes.

Usage:
  makeClass [flags] <file> [file...]

Arguments:
  files - one or more class definition files (required)

Flags:
  -a, --all         list, or run all class definitions found in the local folder
  -o, --options     export options code (check validity in the process)
  -g, --gocmds      export go command code
  -m, --readmes     create readme files for each tool and app
  -f, --format      format source code files (.cpp and .h) found in local folder and below
  -d, --dump        dump any classDefinition config tomls to screen and quit (hidden)
  -A, --openapi     export openapi.yaml file for API documentation (hidden)

Global Flags:
  -x, --fmt string     export format, one of [none|json*|txt|csv]
  -v, --verbose uint   set verbose level (optional level defaults to 1)
  -h, --help           display this help screen

**Source code**: [`dev_tools/makeClass`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/dev_tools/makeClass)

