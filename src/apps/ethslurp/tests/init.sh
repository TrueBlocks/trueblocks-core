clear

# clean up prior tests
rm -fR ~/.quickBlocks.test

# prepare for these tests
mkdir ~/.quickBlocks.test
mkdir ~/.quickBlocks.test/configs
mkdir ~/.quickBlocks.test/abis
mkdir ~/.quickBlocks.test/makeClass

find ~/.quickBlocks.test                             >proposed/test1.txt 2>&1
echo "test1: default behaviour should show help"    >>proposed/test1.txt 2>&1
cat api_key | ethslurp -t                           >>proposed/test1.txt 2>&1
find ~/.quickBlocks.test                            >>proposed/test1.txt 2>&1
cat ~/.quickBlocks.test/quickBlocks.toml            >>proposed/test1.txt 2>&1

echo "Running ethslurp test suite..."
echo
echo "test1: default behaviour should show help"
