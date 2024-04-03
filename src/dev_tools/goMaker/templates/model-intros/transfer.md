The `transfer` data model is produced by the reconciliation process of `chifra export`. It
represents, for all intents and purposes, the asset value transfer produced by nearly all value
transfers on any blockchain. While the details of various token transfers are varied, the
underlying mechanism is through token transfers. In our accounting, we focus on the token transfers
and label each such transfer on the `input` data or `event topic` data. In this way, our accounting
remains relatively easy (we only reconcile tokens and ETH), but we cover every conceivable token
asset transfer of any type.
