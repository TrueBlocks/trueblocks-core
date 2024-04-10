### Notes

When produced using the `--raw` option to `chifra traces`, this data model actually produces `transactionPosition` instead of `transactionIndex`. When produced without the `--raw` option, the model uses `transactionIndex` to be consistent with other data models such as the `transaction`.

Traces and TraceActions, when produced during a self-destruct, export different fields when rendered in JSON. In CSV and TXT output, these fields change thier meaning while retaining the header of the original fields. The following table describes these differences:

Fields that change during self-destruct transaction:

| Field         | When rendered in JSON | When rendered in csv/txt |
| ------------- | --------------------- | ------------------------ |
| Action.From   |                       | Action.Address           |
| Action::To    |                       | Action.RefundAddress     |
| Action::Value |                       | Action.Balance           |
|               | Action.RefundAddress  |                          |
|               | Action.Balance        |                          |
