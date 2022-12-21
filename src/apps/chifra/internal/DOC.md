## Addresses

**One or more addresses (one required), optional topic and fourbyte**

```[shell]
    chifra export [flags] <address> [address...] [topics...] [fourbytes...]
```

**One or more addresses (one required)**

```[shell]
    chifra abis [flags] <address> [address...]

    # alias for chifra export --appeareances
    chifra list [flags] <address> [address...]

    # alais for chifra export [CRUD options]
    chifra monitors [flags] <address> [address...]
```

## Addresses (optional Blocks)

**One or more addresses (required), optional block list**

```[shell]
    chifra state [flags] <address> [address...] [block...]

    chifra slurp [flags] <address> [address...] [block...]
```

**Two or more addresses (two required), optional block list**

```[shell]
    chifra tokens [flags] <address> <address> [address...] [block...]
```

## BlockIds

**One or more blocks (one required)**

```[shell]
    chifra blocks [flags] <block> [block...]
```

**Optional blocks (but should have a required mode of [ extract | check ])**

```[shell]
    chifra chunks [flags] <block> [block...]
```

## BlockIds / Dates

**One or more block identifier or dates (at least one block or at least one date)**

```[shell]
    # This could be not required (list by default)
    chifra when [flags] < block | date > [ block... | date... ]
```

## TxIds

**One of more transaction ids (at least one required)**

```[shell]
    chifra transactions [flags] <tx_id> [tx_id...]

    chifra receipts [flags] <tx_id> [tx_id...]

    chifra logs [flags] <tx_id> [tx_id...]

    chifra traces [flags] <tx_id> [tx_id...]
```

## Subcommands

**One or more mode (one required)**

```[shell]
    chifra scrape [flags] <mode> [mode...]
```

**One or more mode (optional - defaults to some)**

```[shell]
    chifra config [flags] [mode...]
```

**One or more mode**

```[shell]
    # This is wrong - should be required mode
    chifra scrape [flags] [mode...]
```

## Search Terms

**One or more terms (optional)**

```[shell]
    # This is wrong (term is optional)
    chifra explore [flags] <term> [term...]
```

**One or more terms (optional)**

```[shell]
    # This is wrong (term is optional)
    chifra names [flags] <term> [term...]
```

## None

**Flags only**

```[shell]
    # This is wrong - should be required mode
    chifra pins [flags]

    chifra daemon [flags]

    chifra init [flags]
```
