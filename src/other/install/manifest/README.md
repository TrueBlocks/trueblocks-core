## Steps to Update

1. Copy the manifest file from the computer running the scraper and pinning (in our case `linux`)

2. Upload that file to Pinata
3. Copy the IPFS hash produced by Pinata

4. Go to EtherScan
5. Find unchainedindex.eth smart contract
6. Use Etherscan to submit the new hash

7. Update the local chunk files by running `chifra init --all`
