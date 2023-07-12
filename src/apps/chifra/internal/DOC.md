**Flags only**

```[shell]
  chifra daemon [flags]
  chifra init [flags]
  chifra scrape [flags]
```

**One or more addresses**

```[shell]
  chifra abis [flags] <address> [address...]
  chifra list [flags] <address> [address...]
  chifra monitors [flags] <address> [address...]
```

**One or more addresses with optional blocks**

```[shell]
  chifra state [flags] <address> [address...] [block...]
  chifra slurp [flags] <address> [address...] [block...]
```

**Two or more addresses with optional blocks**

```[shell]
  chifra tokens [flags] <address> <address> [address...] [block...]
```

**One or more addresses with optional topics, fourbyte, or blocks**

```[shell]
  chifra export [flags] <address> [address...] [topics...] [fourbytes...]
```

**One or more terms**

```[shell]
  chifra explore [flags] <term> [term...]
  chifra names [flags] <term> [term...]
```

**One mode**

```[shell]
  chifra config <mode> [flags]
```

**One mode with optional blocks or addresses**

```[shell]
  chifra chunks <mode> [flags] [blocks...] [address...]
```

**One or more modes**

```[shell]
  chifra status <mode> [mode...] [flags]
```

**One or more transaction ids**

```[shell]
  chifra transactions [flags] <tx_id> [tx_id...]
  chifra receipts [flags] <tx_id> [tx_id...]
  chifra logs [flags] <tx_id> [tx_id...]
  chifra traces [flags] <tx_id> [tx_id...]
```

**One or more block ids (or dates)**

```[shell]
  chifra blocks [flags] <block> [block...]
  chifra when [flags] < block | date > [ block... | date... ]
```
