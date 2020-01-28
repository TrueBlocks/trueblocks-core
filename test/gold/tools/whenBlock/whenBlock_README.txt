whenBlock argc: 2 [1:-th] 
whenBlock -th 
#### Usage

`Usage:`    whenBlock [-l|-t|-s|-v|-h] &lt; block | date &gt; [ block... | date... ]  
`Purpose:`  Finds the nearest block prior to a date, or the nearest date prior to a block.
            Alternatively, search for one of 'special' blocks.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | block_list | one or more dates, block numbers, hashes, or special named blocks (see notes) |
| -l | --list | export a list of the 'special' blocks |
| -t | --timestamps | ignore other options and generate timestamps only |
| -s | --skip <num> | only applicable if --timestamps is on, the step between block numbers in the export |
| -x | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- The block list may contain any combination of `number`, `hash`, `date`, special `named` blocks.
- Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].
- You may customize the list of named blocks by editing ~/.quickBlocks/whenBlock.toml.
- The following `named` blocks are currently configured:
  - first (`0`), firstTrans (`46147`), iceage (`200000`), devcon1 (`543626`)
  - homestead (`1150000`), daofund (`1428756`), daohack (`1718497`), daofork (`1920000`)
  - devcon2 (`2286910`), tangerine (`2463000`), spurious (`2675000`), stateclear (`2717576`)
  - eea (`3265360`), ens2 (`3327417`), parityhack1 (`4041179`), byzantium (`4370000`)
  - devcon3 (`4469339`), parityhack2 (`4501969`), kitties (`4605167`), makerdao (`4620855`)
  - devcon4 (`6610517`), constantinople (`7280000`), devcon5 (`8700401`), istanbul (`9069000`)
  - muirglacier (`9200000`), latest (``)

