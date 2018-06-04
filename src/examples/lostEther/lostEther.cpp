/* FROM: https:/ /github.com/ethereum/EIPs/issues/156#issuecomment-307015852
   FROM: https:/ /gist.github.com/MichaelMaurer/e073091446751b08762c8c65ae13a37d

// Report on "stuck" ether in empty contracts
// https:/ /github.com/ethereum/EIPs/issues/156

// Usage: node find-stuck-ether.js [startBlock] [endBlock] > report.txt
*/

let Eth = require('ethjs');
let eth = new Eth(new Eth.HttpProvider('http://localhost:8545'));
let ethutil = require('ethereumjs-util')

async function main() {
  try {
    let bn = (await eth.blockNumber()).toNumber()
    let startBlock = process.argv.length > 2 ? Number(process.argv[2]) : bn
    let endBlock = process.argv.length > 3 ? Number(process.argv[3]) : process.argv.length > 2 ? bn : 0
    console.error(`Scanning blocks from ${startBlock} to ${endBlock}`)

    let del = startBlock <= endBlock ? 1 : -1
    endBlock += del

    for (let bn = startBlock; bn !== endBlock; bn += del) {
      let block = await eth.getBlockByNumber(bn, true)
      process.stderr.write(`${bn} ${block.transactions.length}\n`)
      for (let tx of block.transactions) {
        // Examine only nonzero ETH contract creation without code (sends with empty TO and INPUT DATA)
        // T O D O also include the pre-EIP2 case where contracts with empty code may be created if insufficient gas
        if (!tx.to && !tx.value.isZero() && tx.input === '0x') {
          // Filter out transactions that used all their gas. This almost always indicates execution failure.
          // However, we will incorrectly drop successful transactions that used exactly all their gas.
          let txr = await eth.getTransactionReceipt(tx.hash)
          if (!txr.gasUsed.eq(tx.gas)) {
            let dest = txr.contractAddress
            let code = await eth.getCode(dest) // T O D O detect selfdestruct called since tx
            if (code === '0x') { // ether stuck in empty contract
              let val = Eth.fromWei(tx.value, 'ether').toString()
              console.log(`${block.number} ${tx.hash} ${tx.from} ${dest} ${val}`)
            }
          }
        }
      }
    }
  } catch (err) {
    console.error(err)
  }
}

main()
