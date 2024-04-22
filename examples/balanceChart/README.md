# Example - Balance Chart

This example uses the SDK to first find the block numbers of the chain (Eth Mainnet) for specific month ends from the start of the chain until the present. It then fetches the balance of a specific address at each of these block numbers. The balance is then plotted on a chart.

## Running the example

The example code is built using the regular build instructions, but you can build and run it locally using

```bash
go build -o ./balanceChart ./...
```

which will produce a binary called `balanceChart`. You can then run it with

```bash
./balanceChart
```

## Historical chart of balances for prefund accounts

Here's a bar chart:

![alt text](./chart.png)
