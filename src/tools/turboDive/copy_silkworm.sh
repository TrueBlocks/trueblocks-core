#!/usr/bin/env bash

echo "Removing folders..."
rm -fR cbor-cpp
rm -fR ethash
rm -fR evmc
rm -fR evmone
rm -fR gsl
rm -fR intx
rm -fR libff
rm -fR libs
rm -fR lmdb
rm -fR secp256k1
rm -fR silkworm
rm -fR tg_api

echo "Copying libraries..."
cp -f /Users/jrush/Development/silkrpc/build//silkworm/libsecp256k1.a ./libs/
cp -f /Users/jrush/Development/silkrpc/build//silkworm/libevmone.a ./libs/
cp -f /Users/jrush/Development/silkrpc/build//silkworm/libff/libff/libff.a ./libs/
cp -f /Users/jrush/Development/silkrpc/build//silkworm/liblmdb.a ./libs/
cp -f /Users/jrush/Development/silkrpc/build//silkworm/silkworm/libsilkworm.a ./libs/
cp -f /Users/jrush/Development/silkrpc/build//silkworm/cbor-cpp/libcborcpp.a ./libs/

echo "Copying headers..."
SOURCE=/Users/jrush/Development/silkworm/
DEST=/Users/jrush/Development/trueblocks-core/src/tools/turboDive/
(cd $SOURCE && find . -name '*.h*' -print | tar --create --files-from -) | (cd $DEST && tar xvfp -)
