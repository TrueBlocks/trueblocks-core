Parameters are a constituent part of a [Function or Event](/data-model/accounts/#function). The
parameters of a function are each individual value passed into the function. Along with the
function's name, the parameter types (once canonicalized) are used to create a function's four
byte signature (or an event's 32-byte signature). Parameters are important to TrueBlocks because
we use them as part of the ABI decoding and the `--articulate` process to convert the blockchain's
bytes into human-readable text.
