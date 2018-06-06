/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#define _DEBUG
#include "acctlib.h"
#include "options.h"
#include "newblock.h"
#include "newreceipt.h"

extern bool testReadWrite(COptions& options);
extern bool testUpgrade(COptions& options);
extern void reportNode(CBaseNode *node);
namespace qblocks {
    extern bool writeNodeToBinary(const CBaseNode& node, const SFString& fileName);
    extern bool readNodeFromBinary(CBaseNode& node, const SFString& fileName);
};
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    CNewBlock::registerClass();
    CNewReceipt::registerClass();
    CAccountWatch::registerClass();
    CAcctCacheItem::registerClass();
    CBalHistory::registerClass();
    CIncomeStatement::registerClass();

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    if (!fileExists("./oldFmt.cache")) {
        CFilename fileName("./oldFmt.cache");
        return usage("File `" + fileName.getFullPath() + "' not found. All tests will fail without that file.");
    }

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        if (options.testNum != -1)
            testReadWrite(options);
        else if (!options.className.empty())
            testUpgrade(options);
    }

    return 0;
}

//--------------------------------------------------------------
bool testReadWrite(COptions& options) {
    // Note: we use a single block here instead of creating one in each loop
    // to see if there is any residual left over. The reads should reset the data
    CBlock block;
    CNewBlock newBlock;

    CBlock latest;
    getBlock(latest, "latest");

    switch (options.testNum) {
        case 0: {
            ASSERT(fileExists("./oldFmt.cache"));
            cout << "0. Read from old binary format and do nothing...\n";
            cout.flush();
            readBlockFromBinary(block, "./oldFmt.cache");
            reportNode(&block);
            break;
        }
        case 1: {
            ASSERT(fileExists("./oldFmt.cache"));
            cout << "1. Read from old binary format, write to JSON...\n";
            cout.flush();
            readBlockFromBinary(block, "./oldFmt.cache");
            writeToJson(block, "./newFmt.json");
            ASSERT(fileExists("./newFmt.json"));
            reportNode(&block);
            cout << asciiFileToString("./newFmt.json") << "\n";
            cout.flush();
            break;
        }
        case 2: {
            ASSERT(fileExists("./newFmt.json"));
            cout << "2. Read from JSON, write to new binary format...\n";
            cout.flush();
            readFromJson(block, "./newFmt.json");
            newBlock = CNewBlock(block);
            newBlock.finalized = isBlockFinal(newBlock.timestamp, latest.timestamp);
            writeNodeToBinary(newBlock, "./newFmt.cache");
            ASSERT(fileExists("./newFmt.cache"));
            reportNode(&block);
            reportNode(&newBlock);
            break;
        }
        case 3: {
            ASSERT(fileExists("./newFmt.cache"));
            cout << "3. Read from new binary format, write to JSON...\n";
            cout.flush();
            readOneNewBlock_fromBinary(newBlock, "./newFmt.cache");
            writeToJson(newBlock, "./newFmt2.json");
            ASSERT(fileExists("./newFmt2.json"));
            reportNode(&newBlock);
            cout << asciiFileToString("./newFmt2.json") << "\n";
            cout.flush();
            break;
        }
        case 4: {
            ASSERT(fileExists("./newFmt2.json"));
            cout << "4. Read from new JSON and we're done.\n";
            cout.flush();
            readOneNewBlock_fromJson(newBlock, "./newFmt2.json");
            reportNode(&newBlock);
            break;
        }
        case 5: {
            ASSERT(fileExists("./oldFmt.cache"));
            cout << "5. Read from old binary format, using new binary format...\n";
            cout.flush();
            readOneNewBlock_fromBinary(newBlock, "./oldFmt.cache");
            reportNode(&newBlock);

            // Clean up all but the original old format and make sure we did.
            ASSERT( fileExists("./newFmt.cache"));
            remove("./newFmt.cache");
            ASSERT(!fileExists("./newFmt.cache"));

            ASSERT( fileExists("./newFmt.json"));
            remove("./newFmt.json");
            ASSERT(!fileExists("./newFmt.json"));

            ASSERT( fileExists("./newFmt2.json"));
            remove("./newFmt2.json");
            ASSERT(!fileExists("./newFmt2.json"));

            break;
        }
    }
    return true;
}

//--------------------------------------------------------------
CBaseNode *getNode(const SFString& nodeType) {
    CBaseNode *node = NULL;
         if (nodeType == "CAbi")             node = CAbi::CreateObject();
    else if (nodeType == "CFunction")        node = CFunction::CreateObject();
    else if (nodeType == "CParameter")       node = CParameter::CreateObject();
    else if (nodeType == "CBranch")          node = CBranch::CreateObject();
    else if (nodeType == "CInfix")           node = CInfix::CreateObject();
    else if (nodeType == "CLeaf")            node = CLeaf::CreateObject();
    else if (nodeType == "CTreeNode")        node = CTreeNode::CreateObject();
    else if (nodeType == "CTreeRoot")        node = CTreeRoot::CreateObject();
    else if (nodeType == "CAccountWatch")    node = CAccountWatch::CreateObject();
    else if (nodeType == "CBalHistory")      node = CBalHistory::CreateObject();
    else if (nodeType == "CBlock")           node = CBlock::CreateObject();
    else if (nodeType == "CIncomeStatement") node = CIncomeStatement::CreateObject();
    else if (nodeType == "CLogEntry")        node = CLogEntry::CreateObject();
    else if (nodeType == "CPriceQuote")      node = CPriceQuote::CreateObject();
    else if (nodeType == "CReceipt")         node = CReceipt::CreateObject();
    else if (nodeType == "CRPCResult")       node = CRPCResult::CreateObject();
    else if (nodeType == "CTrace")           node = CTrace::CreateObject();
    else if (nodeType == "CTraceAction")     node = CTraceAction::CreateObject();
    else if (nodeType == "CTraceResult")     node = CTraceResult::CreateObject();
    else if (nodeType == "CTransaction")     node = CTransaction::CreateObject();
    else if (nodeType == "CNewBlock")        node = CNewBlock::CreateObject();
    else if (nodeType == "CNewReceipt")      node = CNewReceipt::CreateObject();
    //    else if (nodeType == "CInfuraStats")     node = CInfuraStats::CreateObject();
    //    else if (nodeType == "CPerson")          node = CPerson::CreateObject();
    //    else if (nodeType == "CAccountName")     node = CAccountName::CreateObject();
    return node;
}

//--------------------------------------------------------------
SFString baseTypeName(uint64_t type) {
    SFString ret;
    if (type & TS_NUMERAL) ret += (" TS_NUMERAL " + asStringU(type));
    if (type & TS_STRING)  ret += (" TS_STRING "  + asStringU(type));
    if (type & TS_DATE)    ret += (" TS_DATE "    + asStringU(type));
    if (type & TS_ARRAY)   ret += (" TS_ARRAY "   + asStringU(type));
    if (type & TS_OBJECT)  ret += (" TS_OBJECT "  + asStringU(type));
    if (type & TS_POINTER) ret += (" TS_POINTER " + asStringU(type));
    if (type & TS_BIGNUM)  ret += (" TS_BIGNUM "  + asStringU(type));
    return Strip(ret.Substitute("  "," "), ' ');
}

//--------------------------------------------------------------
SFString typeName(uint64_t type) {

    if (type == T_DATE)      return "T_DATE "    + baseTypeName(type);
    if (type == T_TIME)      return "T_TIME "    + baseTypeName(type);
    if (type == T_BOOL)      return "T_BOOL "    + baseTypeName(type);
    if (type == T_NUMBER)    return "T_NUMBER "  + baseTypeName(type);
    if (type == T_DOUBLE)    return "T_DOUBLE "  + baseTypeName(type);
    if (type == T_WEI)       return "T_WEI "     + baseTypeName(type);
    if (type == T_GAS)       return "T_GAS "     + baseTypeName(type);
    if (type == T_ETHER)     return "T_ETHER "   + baseTypeName(type);
    if (type == T_TEXT)      return "T_TEXT "    + baseTypeName(type);
    if (type == T_ADDRESS)   return "T_ADDRESS " + baseTypeName(type);
    if (type == T_TIMESTAMP) return "T_TIMESPAN "+ baseTypeName(type);
    if (type == T_HASH)      return "T_HASH "    + baseTypeName(type);
    if (type == T_BLOOM)     return "T_BLOOM "   + baseTypeName(type);
    if (type == T_POINTER)   return "T_POINTER " + baseTypeName(type);
    if (type == T_OBJECT)    return "T_OBJECT "  + baseTypeName(type);

    if (type == (T_OBJECT|TS_ARRAY))  return "T_OBJECT|TS_ARRAY "  + baseTypeName(type);
    if (type == (T_TEXT|TS_ARRAY))    return "T_TEXT|TS_ARRAY "    + baseTypeName(type);
    if (type == (T_ADDRESS|TS_ARRAY)) return "T_ADDRESS|TS_ARRAY " + baseTypeName(type);

    return "Unknown" + asStringU(type);
}

//--------------------------------------------------------------
void reportNode(CBaseNode *node) {
    CRuntimeClass *pClass = node->getRuntimeClass();

    cout << SFString('-',80) << "\n";
    cout << "className: " << pClass->m_ClassName << "\n";
    cout << "objectSize: " << pClass->m_ObjectSize << "\n";
    cout << "baseClass: " << (pClass->m_BaseClass ? pClass->m_BaseClass->m_ClassName : "None") << "\n";
    CFieldList *theList = pClass->m_FieldList;
    if (!theList) {
        cout << "Field list not found for " << pClass->m_ClassName << "\n";
    } else {
        LISTPOS pPos = theList->GetHeadPosition();
        while (pPos) {
            const CFieldData *item = theList->GetNext(pPos);
            cout << "\tfieldName: " << item->getName()  << "\n";
            cout << "\t  fieldID: "   << item->getID()    << "\n";
            cout << "\t  fieldType: " << typeName(item->getType())  << "\n";
            cout << "\t  hidden: "    << item->isHidden() << "\n";
        }
        cout << node->Format() << "\n";
        cout << "\n";
    }
    cout.flush();
}

//--------------------------------------------------------------
bool testUpgrade(COptions& options) {

    CBaseNode *node = getNode(options.className);
    if (node) {
        reportNode(node);
        delete node;
    } else {
        cout << "Unknown node of type " << options.className << " not created.\n";
    }

    return true;
}
