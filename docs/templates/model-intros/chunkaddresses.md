<!-- markdownlint-disable MD033 MD036 MD041 -->
The `addresses` data model is produced by `chifra chunks` and represents the records found in the addresses table of each Unchained Index chunk. The `offset` and `count` fields represent the location and number of records in the `appearances` table to which the address table is related.

The following commands produce and manage [{PLURAL}]:

| Tools                                              |                                                                 |
| -------------------------------------------------- | --------------------------------------------------------------- |
| [chifra chunks](/docs/chifra/admin/#chifra-chunks) | manage pinned index of appearances and associated Bloom filters |

[{PROPER}] data is made of the following data fields:

[{FIELDS}]