## goMaker

`goMaker` is development-only tool that aids in various aspects of building TrueBlocks. The program is
disabled for most users, but for core developers it can be used to automatically generate help files,
API interface specs, command line options parsers and many of the test case files. Generally, regular
users will not use `goMaker`.

### Usage

`Usage:`    goMaker
`Purpose:`  Automatically writes files for various purposes.

`Notes:`

- You must run this tool from the root of the TrueBlocks repository.
- Template files are stored in ./src/dev_tools/goMaker/templates.
- No command line options are accepted.

### Notes on Commands

The options, notes, and descriptions for the `chifra` subcommands are stored in a file
called `cmd-line-options.csv` in the folder mentioned above. This file is a CSV file
with the following fields:

| Name         | Description                         | Notes                                                                                                              |
| ------------ | ----------------------------------- | ------------------------------------------------------------------------------------------------------------------ |
| num          | internal use                        |                                                                                                                    |
| folder       | the type of the tool                | one of \[`apps` \| `tools`\]                                                                                       |
| group        | the group to which the item belongs | one of \[ `Accounts` \| `Admin` \| `Chain Data` \| `Chain State` \| `Other` \]                                     |
| route        | the subcommand                      | also called route in various places                                                                                |
| tool         | internal use                        | an artifact from previous versions of goMaker                                                                      |
| longName     | the name of option                  | may be empty for some special types of rows                                                                        |
| hotKey       | hot key for the option              | may be empty                                                                                                       |
| def_val      | default for the option              | most likely empty                                                                                                  |
| attributes   | attributes for the option           | any combination of \[ `visible` \|  `required` \| `docs` \| `config` \] (see below)                                |
| option_type  | the option's type                   | \[ `group` \| `command` \| `positional` \| `flag` \| `switch` \| `note` \| `alias` \] (see below)                  |
| data_type    | the option's data type              | various combinations of `basic types`, `enums`, `lists`, or `lists of enums` (see below)                           |
| return_type  | the type of structure returned      | if present, the type of the structure returned by the associated SDK routine (see below)                           |
| summary      | the summary for the subcommand      | displayed by `chifra --help`                                                                                       |
| usage        | the format of the arguments         | in true Linux fashion                                                                                              |
| capabilities | the command's global capabilities   | see [this explaination](https://github.com/TrueBlocks/trueblocks-core/blob/develop/docs/content/chifra/globals.md) |
| description  | the subcommand's full description   | shown with `chifra cmd --help`                                                                                     |

- `attributes` is a pipe (`|`) separated list of the following:
  - `visible` - the option is visible on the command line, hidden (but available) otherwise
  - `required` - the option is required
  - `docs` - the option is documented (if present, `visible` must be `true`)
  - `config` - the option is a configuration file option (documented in the help file, not available on the command line)

- `option_type` is one of the following:
  - `group` - the broad group the subcommand belongs to in the documentation
  - `command` - the subcommand's controling row in the `cmd-line-options.csv` file (used for documentation only)
  - `positional` - a positional argument (a command line argument without a leading `--option`)
  - `flag` - a flag (a command line argument that requires a trailing value)
  - `switch` - a switch (a boolean command line argument)
  - `note` - a note (used for documentation only, this appears in the notes section of the READMEs)
  - `alias` - an alias (used for the Hugo website only, this aliases old pages)

### Notes on Data Models

The `goMaker` program also generates a huge number of source code files and documentation related to the various data models produced or consumed by the various TrueBlocks tools. These data models are stored in `.toml` files in the `./src/dev_tools/goMaker/templates/classDefinitions` folder and the model's fields (in a `.csv`) are stored in a subfolder called `fields`. There are two files for each data model (a `.toml` and a `.csv`) names identically to the data model's name.

The `.toml` file contains the following fields:

| Name         | Description                                                                      | Notes                                                                                        |
| ------------ | -------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------- |
| class        | the name of teh data model in lower case                                         |                                                                                              |
| contained_by | if this data model is contained by other models, the list of other models        |                                                                                              |
| doc_group    | corresponds the subcommand's group for documentation purposes                    |                                                                                              |
| doc_route    | corresponds to the subsection of the data model documentation for this type      | poorly named                                                                                 |
| doc_descr    | the short description for this type                                              | see below                                                                                    |
| go_output    | if not empty, the destination folder for the generated code. Disabled if empty   |                                                                                              |
| go_model     | an override for the generic (i.e. somewhat complicated) `Blocks` model only      |                                                                                              |
| produced_by  | a list of commands that produces this data model                                 |                                                                                              |
| cache_as     | if set to `group`, the cache for this type is a slice. A single value otherwise. |                                                                                              |
| cache_by     | the fields by which to identify cache items                                      | (one of \[ `address,block` \| `address,block,fourbyte` \| `address,tx` \| `block` \| `tx` \] |
| cache_type   | one of \[ `cacheable` \| `marshal_only` \]                                       |                                                                                              |

### Notes on Templates

The `./src/dev_tools/goMaker/templates` folder also contains a number of templates used by the `goMaker` program. The names of these templates corresponds to the location in the repo's paths the generated files will be written. For example, the `./sdk_route.go.tmpl` writes files to the `./sdk` folder. The filename of the file is `<route>.go` where `<route>` is the route of the subcommand. The template name may contain the word "route" or the word "type" which is sequentially replaced with either the routes or the data model types.

### Fin

Enough already. Experiment if you must.
