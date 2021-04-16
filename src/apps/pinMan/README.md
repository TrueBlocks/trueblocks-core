## pinMan

The `pinMan` is not ready for prime time. Please return late.

## Building

After building **trueblocks** on the `develop` branch, go to the pinata test folder:

```[bash]
cd ../src/other/pinata
```

Edit **pinata.cpp** and change the two lines by entering your `pinata` api keys:

```[bash]
string_q API_KEY = "pinata_api_key: YOUR_KEY";
string_q API_SECRET_KEY = "pinata_secret_api_key: YOUR_SECRET_KEY";
```

Replace the `YOUR_KEY` and `YOUR_SECRET_KEY` values (not the whole string). Save and then:

```[bash]
make
./bin/pinata
```

## On Mac

When I run this on Mac, I get:

```[bash]
Curl response: 60: SSL peer certificate or SSH remote key was not OK
```

## On Ubuntu

Same code on Ubuntu produces:

```[bash]
Curl response: {"IpfsHash":"QmXTAALdtWWzm9x5GZV7bC9yiYS7iFFUL3B5jFVtdafuv2","PinSize":58,"Timestamp":"2020-07-21T21:43:36.709Z"}
```

## Which version of openssl?

On Mac:

```[bash]
openssl version -a
```

produces:

```[bash]
OpenSSL 1.0.2t  10 Sep 2019
built on: reproducible build, date unspecified
platform: darwin64-x86_64-cc
options:  bn(64,64) rc4(ptr,int) des(idx,cisc,16,int) idea(int) blowfish(idx)
compiler: clang -I. -I.. -I../include  -fPIC -fno-common -DOPENSSL_PIC -DOPENSSL_THREADS -D_REENTRANT -DDSO_DLFCN -DHAVE_DLFCN_H -arch x86_64 -O3 -DL_ENDIAN -Wall -DOPENSSL_IA32_SSE2 -DOPENSSL_BN_ASM_MONT -DOPENSSL_BN_ASM_MONT5 -DOPENSSL_BN_ASM_GF2m -DSHA1_ASM -DSHA256_ASM -DSHA512_ASM -DMD5_ASM -DAES_ASM -DVPAES_ASM -DBSAES_ASM -DWHIRLPOOL_ASM -DGHASH_ASM -DECP_NISTZ256_ASM
OPENSSLDIR: "/usr/local/etc/openssl"
```

On Ubuntu:

```[bash]
openssl version -a
```

produces:

```[bash]
OpenSSL 1.1.1  11 Sep 2018
built on: Wed May 27 19:15:54 2020 UTC
platform: debian-amd64
options:  bn(64,64) rc4(16x,int) des(int) blowfish(ptr)
compiler: gcc -fPIC -pthread -m64 -Wa,--noexecstack -Wall -Wa,--noexecstack -g -O2 -fdebug-prefix-map=/build/openssl-dyPhHZ/openssl-1.1.1=. -fstack-protector-strong -Wformat -Werror=format-security -DOPENSSL_USE_NODELETE -DL_ENDIAN -DOPENSSL_PIC -DOPENSSL_CPUID_OBJ -DOPENSSL_IA32_SSE2 -DOPENSSL_BN_ASM_MONT -DOPENSSL_BN_ASM_MONT5 -DOPENSSL_BN_ASM_GF2m -DSHA1_ASM -DSHA256_ASM -DSHA512_ASM -DKECCAK1600_ASM -DRC4_ASM -DMD5_ASM -DAES_ASM -DVPAES_ASM -DBSAES_ASM -DGHASH_ASM -DECP_NISTZ256_ASM -DX25519_ASM -DPADLOCK_ASM -DPOLY1305_ASM -DNDEBUG -Wdate-time -D_FORTIFY_SOURCE=2
OPENSSLDIR: "/usr/lib/ssl"
ENGINESDIR: "/usr/lib/x86_64-linux-gnu/engines-1.1"
Seeding source: os-specific
```

#### Usage

`Usage:`    chifra pins [-l|-f|-c|-i|-v|-h]  
`Purpose:`  Report on and manage the remotely pinned appearance index and associated bloom filters.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -l | --list <val> | list all pins either locally or at the pinning service, one of [local*&#124;remote] |
| -f | --freshen | push files from the local index to the pinning service and update manifest |
| -c | --compare | report differences between the manifest and the pinning service |
| -i | --init | initialize the local index by downloading bloom filters from the pinning service |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --raw       |   report JSON data from the node with minimal processing
    --very_raw  |   report JSON data from node with zero processing
    --fmt       |   export format (where appropriate). One of [none|txt|csv|json|api]
    --to_file   |   write the results to a temporary file and return the filename
    --output:fn |   write the results to file 'fn' and return the filename
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

