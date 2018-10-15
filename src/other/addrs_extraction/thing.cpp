prevThousand = 0;
address_list.clear();
while (true) {
    block = retreiveBlockFromRPC();
    address_list.append(block.extractAllAddresses());
    blocknum_t bn = block.blockNumber;
    blocknum_t curThousand = (bn / 1000) * 1000;
    if (curThousand > prevThousand) {
        writeAddressesToFile(curThousand, address_list);
        prevThousand = curThousand;
        address_list.clear();
    }
}
