## migrate

The `migrate` tool runs migrations. Do not use this tool unless told to do so by TrueBlocks' devs.

### Usage

`Purpose:`
  Run migrations.

`Usage:`
  migrate [flags]

`Flags:`
  -m, --mode <val>   either effectuate or test to see if a migration is necessary, one of [test|cache|index]

`Global Flags:`
  -x, --fmt string     export format, one of [none|json*|txt|csv]
  -v, --verbose uint   set verbose level (optional level defaults to 1)
  -h, --help           display this help screen
