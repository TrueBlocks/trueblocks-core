Logs appear in a possibly empty array in the transaction's receipt. They are only created if the underlying transaction suceeded. In the case where the transaction failed, no logs will appear in the receipt. Logs are only ever generated during transactions whose `to` address is a smart contract.

### How to get logs

- **CLI**:
  - run `chifra logs <txn_id>`.
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-logs)
- **API**:
  - [Calls to `/logs`](/api#operation/chaindata-logs)
