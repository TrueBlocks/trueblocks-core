n_blocks --> block_cnt
n_block_procs --> block_chan_cnt
n_addr_procs --> addr_chan_cnt

changed -a flag in chifra blocks which used to stand for --apps to -s due to adding --articulate option
added options --logs, --emitter, and --topics to chifra blocks
From pinMan each of these is deprecated
	freshen  bool
	remote   bool
	init_all bool
changed --cache_txs to --cache in export



--uniq_txs to --uniq -- removed the old --uniq option which was redundant


chifra when
	field headers changed from
		blocknumber	timestamp	date	name
	to
		blockNumber	timeStamp	date	name
	camel case from lower case

