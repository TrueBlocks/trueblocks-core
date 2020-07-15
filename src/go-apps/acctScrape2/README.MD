# acctScrape2

usage

```
$ acctScrape2 <address>
```

scans all the files in the (data directory)[#data-compulsory] for occurrances of the specified (address)[#address] and outputs them to <stdout>

example (with all options)

```
./acctScrape2 0xff488fd296c38a24cccc60b43dd7254810dab64e --fmt txt --log --max 5
```

### address

The address is the only parameter and can be supplied as 

- a standard eth address (`0xff488fd296c38a24cccc60b43dd7254810dab64e`)
- a pure hex number (`ff488fd296c38a24cccc60b43dd7254810dab64e`) without the 0x header

## options

### Output Format (fmt)
```
--fmt <format>
```

options are 

- json (default)
- txt
- csv

examples

JSON (post formatted )
```
[
    {
        "address": "0xff488fd296c38a24cccc60b43dd7254810dab64e",
        "blockNumber": 8001657,
        "transactionIndex": 170
    },
    {
        "address": "0xff488fd296c38a24cccc60b43dd7254810dab64e",
        "blockNumber": 8001659,
        "transactionIndex": 157
    }
]
```

CSV
```
"address","blockNumber","transactionIndex"
"0xff488fd296c38a24cccc60b43dd7254810dab64e","8001657","170"
"0xff488fd296c38a24cccc60b43dd7254810dab64e","8001659","157"
"0xff488fd296c38a24cccc60b43dd7254810dab64e","8001661","166"
```

TXT
```
address	blockNumber	transactionIndex
0xff488fd296c38a24cccc60b43dd7254810dab64e	8001657	170
0xff488fd296c38a24cccc60b43dd7254810dab64e	8001659	157
0xff488fd296c38a24cccc60b43dd7254810dab64e	8001661	166
```

### Max Threads (max)

`--max 1000`

limit the number of concurrent threads that will be processed

default : 4

Setting the `--max` option overrides any value in the (configuration file)[#configuration-file]

### logging

`--log`

Log the following

- start
- address searched for
- filenames being processed

The logfile is stored in lods/acctScrape2.log

This allows you to confirm that each file is being processed.

Not expected to be used in general operation.

### streaming

`--stream`

Where the address is a popular one, searching could use a ridiculous amount of memory so streaming has been implemented

## configuration file

acctScrape2 uses a configuration file in **TOML** format with two values

### data (compulsory)

The data directory path.

```
data = "/var/binary-data"
```

### max (optional)

The maximum number of threads that will be used by acctScrape2

```
max = 7
```

This value, if set, overrides the default value (currently 4) but can be overriden by the command line parameter --max

## TESTS

### Streaming vs Non Streaming

`streamtest.sh` assumes a directory called data with 10,000 copies of datafile-xxxx.bin in it

The test file has 23 occurrances of address 0x75b480b30bcb0c57951837c8db362db72df25cab

Once you have run the program, the tests in stream_test can be run.

The TestStream functions assume 230,000 entries (plus a header line for TXT and CSV) 

### LOCATE TESTS

`TestListAddresses` lists all the addresses in the file to allow you to select addresses to test

The remaining tests check for 

- first address
- a middleaddress
- the last address

- an address before the first
- a non existent address in the middle

We could not test after the last record because the last record was 0xffffffffffffffffffffffffffffffffffffffff

