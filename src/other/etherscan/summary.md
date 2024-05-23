**Group:  Accounts**  
**Module:** account  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: balance             <address>               <tag>
    action: balancehistory      <address>               <tag>
    action: balancemulti        <address_list>          <tag>
    action: txlist              <address>               <block-range>      <pagination-sort>
    action: txlistinternal      <address>               <block-range>      <pagination-sort>
    action: txlistinternal                              <block-range>      <pagination-sort>
    action: txsBeaconWithdrawal <address>               <block-range>      <pagination-sort>
    action: txlistinternal      <hash>
    action: tokentx             <contract>  <address>   <block-range>      <pagination-sort>
    action: tokennfttx          <contract>  <address>   <block-range>      <pagination-sort>
    action: token1155tx         <contract>  <address>   <block-range>      <pagination-sort>
    action: getminedblocks      <address>               <blocktype>         <pagination>
```

**Group:  Contracts**  
**Module:** contract  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: getabi              <address>
    action: getsourcecode       <address>
    action: getcontractcreation <contract-list>
```

**Group:  Transactions**  
**Module:** transaction  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: getstatus           <hash>
    action: gettxreceiptstatus  <hash>
```

**Group:  Blocks**  
**Module:** block  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: getblockreward              <blknum>
    action: getblockcountdown           <blknum>
    action: getblocknobytime            <timestamp>     <direction>
```

**Group:  Blocks**  
**Module:** blockstats  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: dailyavgblocksize           <date-range>    <sort>
    action: dailyblkcount               <date-range>    <sort> 
    action: dailyblockrewards           <date-range>    <sort> 
    action: dailyavgblocktime           <date-range>    <sort> 
    action: dailyuncleblkcount          <date-range>    <sort> 
```

**Group:  Logs**  
**Module:** logs  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: getLogs                     <block-range>    <topics>    <pagination>
    action: getLogs         <address>   <block-range>                <pagination>
    action: getLogs         <address>   <block-range>    <topics>    <pagination>
```

**Group:  Proxy**  
**Module:** proxy  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: eth_blockNumber
    action: eth_getBlockByNumber                    <tag>           boolean=true
    action: eth_getUncleByBlockNumberAndIndex       <tag>           index=<hex>
    action: eth_getBlockTransactionCountByNumber    <tag>
    action: eth_getTransactionByHash                <hash>
    action: eth_getTransactionByBlockNumberAndIndex <tag>           index=<hex>
    action: eth_getTransactionCount                 <tag>           <address>
    action: eth_sendRaw Transaction                 <hex>
    action: eth_getTransactionReceipt               <hash>
    action: eth_gasPrice
    action: eth_call                                <tag>           <address>       <hex>
    action: eth_getCode                             <tag>           <address>
    action: eth_getStorageAt                        <tag>           <address>       <hex>
    action: eth_estimateGas                         <hex>           <address>       <hex>   <gasPrice>  <gas>
```

**Group:  Tokens**  
**Module:** account  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: addresstokenbalance         <address>                       <pagination>
    action: addresstokennftinventory    <contract>      <address>       <pagination>
    action: tokenbalance                <contract>      <address>       <tag>
    action: tokenbalancehistory         <contract>      <address>       <tag>
```

**Group:  Tokens**  
**Module:** stats  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: tokensupply                 <contract>
​    action: tokensupplyhistory          <contract>      <tag> 
```

**Group:  Tokens**  
**Module:** token  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: tokenholderlist             <contract>      <pagination>
    action: tokeninfo                   <contract> 
```

**Group:  GasTracker**  
**Module:** gastracker  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: gasestimate                 <gasprice>
    action: gasoracle 
```

**Group:  GasTracker**  
**Module:** stats  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: dailyavggaslimit            <date-range>        <sort> 
    action: dailygasused                <date-range>        <sort> 
    action: dailyavggasprice            <date-range>        <sort> 
```

**Group:  Stats**  
**Module:** stats  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: ethsupply
    action: ethsupply2
    action: ethprice
    action: nodecount
    action: chainsize                   <date-range>        <sort>      clienttype=geth     syncmode=default
    action: dailytxnfee                 <date-range>        <sort> 
    action: dailynewaddress             <date-range>        <sort> 
    action: dailynetutilization         <date-range>        <sort> 
    action: dailyavghashrate            <date-range>        <sort> 
    action: dailytx                     <date-range>        <sort> 
    action: dailyavgnetdifficulty       <date-range>        <sort> 
    action: ethdailymarketcap           <date-range>        <sort> 
    action: ethdailyprice               <date-range>        <sort> 
```

**Group:  Pro**  
**Module:** account  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: balancehistory              <address>              blockno=8000000
​    action: tokenbalancehistory         <contract>      <address>            blockno=8000000 
```

**Group:  Pro**  
**Module:** stats  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: dailyavgblocksize           <date-range>        <sort>
    action: dailyblkcount               <date-range>        <sort> 
    action: dailyblockrewards           <date-range>        <sort> 
    action: dailyavgblocktime           <date-range>        <sort> 
    action: dailyuncleblkcount          <date-range>        <sort> 
    action: dailyavggaslimit            <date-range>        <sort> 
    action: dailygasused                <date-range>        <sort> 
    action: dailyavggasprice            <date-range>        <sort> 
    action: dailytxnfee                 <date-range>        <sort> 
    action: dailynewaddress             <date-range>        <sort> 
    action: dailynetutilization         <date-range>        <sort> 
    action: dailyavghashrate            <date-range>        <sort> 
    action: dailytx                     <date-range>        <sort> 
    action: dailyavgnetdifficulty       <date-range>        <sort> 
    action: ethdailymarketcap           <date-range>        <sort> 
    action: ethdailyprice               <date-range>        <sort> 
​    action: tokensupplyhistory          <contract>      blockno=8000000 
```

**Group:  Pro**  
**Module:** token  
**url:** https://api.etherscan.io/api?module=MODULE&apikey=KEY
```
    action: tokeninfo                   <contract> 
```
