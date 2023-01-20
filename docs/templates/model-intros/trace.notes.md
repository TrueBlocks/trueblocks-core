When produced using the `--raw` option, this data model uses `transactionPosition` instead of `transactionIndex`. When produced without `--raw`, the model uses `transactionIndex` to be consistent with all other models.

When a trace represents a self-destruct, four fields change thier meaning if one is exporting in `txt` or `csv` format. The following table describes that change:

| Field Name            | Header Field Name |
| --------------------- | ----------------- |
| action::Address       | action::from      |
| action::RefundAddress | action::to        |
| action::Balance       | action::value     |
