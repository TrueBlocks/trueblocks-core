By shifting to GoLang tracing we removed a "fake" trace that had callType == "creation". This trace was completely manufactured by us as an indication of a smart contract creation. In the new version, instead
we now include the actual trace returned by the node which contains either an empty call type or a call type of "creation" but also includes the `init` code in the traceAction and the `code` in the
traceResult. The traceResult always did display the newly created contract's address.
