contract A {
  struct Donation { address addr; uint256 value; address recipient; }
  function donate(Donation[] donations) { }
  function donate((address,uint256,address)[] donations) { }
}
