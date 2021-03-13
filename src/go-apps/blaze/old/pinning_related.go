// #define hashToIndexFormatFile "Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2"
// #define hashToBloomFormatFile "QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD"
//     if (publish) {
//         CPinManifest manifest;
//         manifest.fileName = "initial-manifest.json";
//         manifest.indexFormat = hashToIndexFormatFile;
//         manifest.bloomFormat = hashToBloomFormatFile;
//         manifest.prevHash = "";  // (prevHash == "" ? hashToEmptyFile : prevHash);

//         CPinnedChunkArray pList;
//         pinlib_readPinList(pList, true);
//         pinlib_forEveryPin(pList, addNewPin, &manifest);
//         manifest.toJson(cout);

//         return true;
//     }
// extern bool visitToPin(const string_q& chunkId, void* data);
// extern bool addNewPin(CPinnedChunk& pin, void* data);
//     CPinnedChunkArray pinList;
// bool visitToPin(const string_q& chunkId, void* data) {
//     LOG_INFO("  Pinning");
//     LOG_FN8(chunkId);
//     ASSERT(data);
//     CPinnedChunkArray& pinList = *(CPinnedChunkArray*)data;  // NO_LINT
//     CPinnedChunk pinRecord;
//     pinlib_pinChunk(pinList, chunkId, pinRecord);
//     return !shouldQuit();
// }
// bool addNewPin(CPinnedChunk& pin, void* data) {
//     CPinManifest* manifestPtr = (CPinManifest*)data;  // NOLINT
//     manifestPtr->newPins.push_back(pin);

//     timestamp_t unused;
//     blknum_t newEnd;
//     blknum_t newStart = bnFromPath(pin.fileName, newEnd, unused);

//     if (manifestPtr->newBlockRange.empty()) {
//         manifestPtr->newBlockRange = padNum9(newStart) + "-" + padNum9(newEnd);
//     } else {
//         blknum_t oldEnd;
//         blknum_t oldStart = bnFromPath(manifestPtr->newBlockRange, oldEnd, unused);
//         manifestPtr->newBlockRange = padNum9(min(oldStart, newStart)) + "-" + padNum9(max(oldEnd, newEnd));
//     }
//     // TODO(tjayrush): Note...
//     return !isTestMode();
// }
