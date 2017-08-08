## [{NAME}]

The `whenBlock` tool answers one of two questions: (1) "When did a given block occur?" or (2) "What block occurred at or before a given date and time?"

In the first case, one simply supplies a block number and the date and time of that block is displayed. In the later case, one provides a date (and optionally time) and the block number that 
occurred at or just prior to that date and time is displayed.

Name your own blocks by editing '~./quickBlocks/whenBlock.toml' and placing `name=blockNum` pairs under the [SPECIAL] section of that file (create it if not present).

The `hour`/`minute`/`second` portions of the date are optional, and if omitted, default to zero in each case.

   Special values: [ iceage | homestead | daofund | daohack | daofork | tangerine | spurious | stateclear | latest ]
[{COMMAND_HELP}]
