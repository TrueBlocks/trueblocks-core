test:
	@make
	@cacheMan -n tests/dao.bin      >tests/dao.txt
	@cacheMan -n tests/singular.bin >tests/singular.txt
	@cacheMan -n tests/firstBlood.bin >tests/firstBlood.txt
	@cacheMan -n tests/gnosis.bin >tests/gnosis.txt
	@cacheMan -n tests/ens.bin      >tests/ens.txt
	@cacheMan -n tests/dao.bin -bn | grep -v List | sed 's/^00//' >tests/blkonly.txt
	@cat tests/dao.txt tests/singular.txt tests/ens.txt tests/firstBlood.txt tests/gnosis.txt >tests/theCaches.txt
	@echo "yes" | cacheMan -mnl tests/test* >tests/merged.txt
	@rm -f merged.bin
	@make test1
	@make test2

test1:
	@cacheMan -nl tests/gnosis.bin | grep "which: 00000" | wc >tests/counts.txt
	@cacheMan -nl tests/gnosis.bin | grep "which: 00000"      >>tests/counts.txt
	@cacheMan -nl tests/gnosis.bin | grep "which: 00001" | wc >>tests/counts.txt
	@cacheMan -nl tests/gnosis.bin | grep "which: 00001"      >>tests/counts.txt
	@cacheMan -nl tests/gnosis.bin | grep "which: 00002" | wc >>tests/counts.txt
	@cacheMan -nl tests/gnosis.bin | grep "which: 00002"      >>tests/counts.txt

test2:
	@cacheMan -e:0 tests/gnosis.bin ; cacheMan -nl extract.bin | wc >tests/counts2.txt
	@cacheMan -e:0 tests/gnosis.bin ; cacheMan -nl extract.bin      >>tests/counts2.txt
	@cacheMan -e:1 tests/gnosis.bin ; cacheMan -nl extract.bin | wc >>tests/counts2.txt
	@cacheMan -e:1 tests/gnosis.bin ; cacheMan -nl extract.bin      >>tests/counts2.txt
	@cacheMan -e:2 tests/gnosis.bin ; cacheMan -nl extract.bin | wc >>tests/counts2.txt
	@cacheMan -e:2 tests/gnosis.bin ; cacheMan -nl extract.bin      >>tests/counts2.txt
	@rm -f extract.bin

gold:
	@ethslurp -f:hack 0x314159265dd8dbb310642f98f50c066173c1259b | tee x
	@ethslurp -f:hack 0x012233b3c8177f0778d910ed88170b82de3bfe57 | tee -a x
	@ethslurp -f:hack 0xEA2FfcbD626454E1ccB07Fd95D44f7335aF433bA | tee -a x
	@ethslurp -f:hack 0xbb9bc244d798123fde783fcc1c72d3bb8c189413 | tee -a x
	@ethslurp -f:hack 0xBf4eD7b27F1d666546E30D74d50d173d20bca754 | tee -a x
	@ethslurp -f:hack 0xc0ee9db1a9e07ca63e4ff0d5fb6f86bf68d47b89 | tee -a x
	@ethslurp -f:hack 0xf835a0247b0063c04ef22006ebe57c5f11977cc4 | tee -a x
	@ethslurp -f:hack 0xaf30d2a7e90d7dc361c8c4585e9bb7d2f6f15bc7 | tee -a x
	@ethslurp -f:hack 0xbe4e25443Df23382228E6347b06565edD838f901 | tee -a x
	@ethslurp -f:hack 0x851b7F3Ab81bd8dF354F0D7640EFcD7288553419 | tee -a x
	@ethslurp -f:hack 0x19076bd01e86fa3d76334e27d4b8e6789688a42d | tee -a x
	@ethslurp -f:hack 0xaeC2E87E0A235266D9C5ADc9DEb4b2E29b54D009 | tee -a x
	@ethslurp -f:hack 0xe736091fc36f1ad476f5e4e03e4425940822d3ba | tee -a x
	@ethslurp -f:hack 0xbdF5c4F1c1A9D7335a6a68d9AA011D5F40cf5520 | tee -a x
	@ethslurp -f:hack 0xc78310231aa53bd3d0fea2f8c705c67730929d8f | tee -a x
	@ethslurp -f:hack 0x5901Deb2C898D5dBE5923E05e510E95968a35067 | tee -a x
	@cut -f13 x | sort | grep -v block >tests/blkonly.txt
	@rm -f x

