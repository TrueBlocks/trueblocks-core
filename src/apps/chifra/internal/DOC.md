## Addresses

**One or more addresses (one required), optional topic and fourbyte**

```
    chifra export [flags] <address> [address...] [topics...] [fourbytes...]
```

**One or more addresses (one required)**

```
    chifra abis [flags] <address> [address...]

    # alias for chifra export --appeareances
    chifra list [flags] <address> [address...]

    # alais for chifra export [CRUD options]
    chifra monitors [flags] <address> [address...]
```

## Addresses (optional Blocks)

**One or more addresses (required), optional block list**

```
    chifra state [flags] <address> [address...] [block...]

    chifra slurp [flags] <address> [address...] [block...]
```

**Two or more addresses (two required), optional block list**

```
    chifra tokens [flags] <address> <address> [address...] [block...]
```

## BlockIds

**One or more blocks (one required)**

```
    chifra blocks [flags] <block> [block...]
```

**Optional blocks (but should have a required mode of [ extract | check ])**

```
    chifra chunks [flags] <block> [block...]
```

## BlockIds / Dates

**One or more block identifier or dates (at least one block or at least one date)**

```
    # This could be not required (list by default)
    chifra when [flags] < block | date > [ block... | date... ]
```


## TxIds

**One of more transaction ids (at least one required)**

```
    chifra transactions [flags] <tx_id> [tx_id...]

    chifra receipts [flags] <tx_id> [tx_id...]

    chifra logs [flags] <tx_id> [tx_id...]

    chifra traces [flags] <tx_id> [tx_id...]
```

## Subcommands

**One or more mode (one required)**

```
    chifra scrape [flags] <mode> [mode...]
```

**One or more mode (optional - defaults to some)**

```
    chifra status [flags] [mode...]
```

**One or more mode**

```
    # This is wrong - should be required mode
    chifra scrape [flags] [mode...]
```

## Search Terms

**One or more terms (optional)**

```
    # This is wrong (term is optional)
    chifra explore [flags] <term> [term...]
```

**One or more terms (optional)**

```
    # This is wrong (term is optional)
    chifra names [flags] <term> [term...]
```

## None

**Flags only**

```
    # This is wrong - should be required mode
    chifra pins [flags]

    chifra serve [flags]

    chifra init [flags]

    chifra quotes [flags]
```
