Usage | blockScrape [-l\|-c\|-d\|-i|-r|-f|-s|-v|-h]</td></tr>
Purpose | Indexes non-emtpy blocks (i.e. one or more transactions). Alternatively, lists non-empty blocks or checks for correctness.</td></tr>
<tr><td colspan=2><font color=yellow>Where:</font></td></tr>
<tr><td colspan=2>
<pre>
    -l  (or --list)       list all non-empty block numbers
	-c  (or --check)      check that empty blocks are empty, and visa versa. Optionally start at :block
	-d  (or --deep)       when doing 'check', do a deep check
	-i  (or --indexOnly)  only create the index, don't save any blocks (applies only to --refreshen mode)
	-r  (or --refreshen)  remove the full-block index and re-create it
	-f  (or --freshen)    starting from the most recently visited block, append any new, non-empty block numbers to the index
	-s  (or --skip)       export once every 5,000 lines (applies only to --verbose --list mode - used for testing)
	-v  (or --verbose)    set verbose level. Follow with a number to set level (-v0 for silent)
	-h  (or --help)       display this help screen</td></tr>
</pre></td></tr>
<tr><td colspan=2>Powered by <font color=lightblue>QuickBlocks.io</font></td></tr>
</table>
