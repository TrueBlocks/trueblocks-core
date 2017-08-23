/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#define _DEBUG
#include "acctlib.h"
#include "options.h"
#include "newblock.h"

extern bool testReadWrite(COptions& options);
extern bool testUpgrade(COptions& options);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    // Tell the system where the blocks are and which version to use
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("binary");

    CNewBlock::registerClass();

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return false;

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

    switch (options.testNum) {
        case 0: {
            ASSERT(fileExists("./oldFmt.bin"));
            cout << "Read from old binary format and do nothing...\n";
            readOneBlock_fromBinary(block, "./oldFmt.bin");
            cout << block.Format() << "\n";
            break;
        }
        case 1: {
            ASSERT(fileExists("./oldFmt.bin"));
            cout << "Read from old binary format, write to JSON...\n";
            readOneBlock_fromBinary(block, "./oldFmt.bin");
            writeToJson(block, "./newFmt.json");
            ASSERT(fileExists("./newFmt.json"));
            cout << block.Format() << "\n";
            cout << asciiFileToString("./newFmt.json") << "\n";
            break;
        }
        case 2: {
            ASSERT(fileExists("./newFmt.json"));
            cout << "Read from JSON, write to new binary format...\n";
            readOneBlock_fromJson(block, "./newFmt.json");
            newBlock = CNewBlock(block);
            writeToBinary(newBlock, "./newFmt.bin");
            ASSERT(fileExists("./newFmt.bin"));
            cout << block.Format() << "\n";
            cout << newBlock.Format() << "\n";
            break;
        }
        case 3: {
            ASSERT(fileExists("./newFmt.bin"));
            cout << "Read from new binary format, write to JSON...\n";
            readOneNewBlock_fromBinary(newBlock, "./newFmt.bin");
            writeToJson(newBlock, "./newFmt2.json");
            ASSERT(fileExists("./newFmt2.json"));
            cout << newBlock.Format() << "\n";
            cout << asciiFileToString("./newFmt2.json") << "\n";
            break;
        }
        case 4: {
            ASSERT(fileExists("./newFmt2.json"));
            cout << "Read from new JSON and we're done.\n";
            readOneNewBlock_fromJson(newBlock, "./newFmt2.json");
            cout << newBlock.Format() << "\n";
            break;
        }
        case 5: {
            ASSERT(fileExists("./oldFmt.bin"));
            cout << "Read from old binary format, using new binary format...\n";
            readOneNewBlock_fromBinary(newBlock, "./oldFmt.bin");
            cout << newBlock.Format() << "\n";

            // Clean up all but the original old format and make sure we did.
            ASSERT( fileExists("./newFmt.bin"));
            removeFile("./newFmt.bin");
            ASSERT(!fileExists("./newFmt.bin"));

            ASSERT( fileExists("./newFmt.json"));
            removeFile("./newFmt.json");
            ASSERT(!fileExists("./newFmt.json"));

            ASSERT( fileExists("./newFmt2.json"));
            removeFile("./newFmt2.json");
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
    else if (nodeType == "CAccount")         node = CAccount::CreateObject();
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
    //    else if (nodeType == "CInfuraStats")     node = CInfuraStats::CreateObject();
    //    else if (nodeType == "CPerson")          node = CPerson::CreateObject();
    //    else if (nodeType == "CAccountName")     node = CAccountName::CreateObject();
    return node;
}

//--------------------------------------------------------------
SFString baseTypeName(SFUint32 type) {
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
SFString typeName(SFUint32 type) {

    if (type == T_DATE)    return "T_DATE "    + baseTypeName(type);
    if (type == T_TIME)    return "T_TIME "    + baseTypeName(type);
    if (type == T_BOOL)    return "T_BOOL "    + baseTypeName(type);
    if (type == T_NUMBER)  return "T_NUMBER "  + baseTypeName(type);
    if (type == T_DOUBLE)  return "T_DOUBLE "  + baseTypeName(type);
    if (type == T_WEI)     return "T_WEI "     + baseTypeName(type);
    if (type == T_GAS)     return "T_GAS "     + baseTypeName(type);
    if (type == T_ETHER)   return "T_ETHER "   + baseTypeName(type);
    if (type == T_TEXT)    return "T_TEXT "    + baseTypeName(type);
    if (type == T_ADDRESS) return "T_ADDRESS " + baseTypeName(type);
    if (type == T_HASH)    return "T_HASH "    + baseTypeName(type);
    if (type == T_BLOOM)   return "T_BLOOM "   + baseTypeName(type);
    if (type == T_POINTER) return "T_POINTER " + baseTypeName(type);
    if (type == T_OBJECT)  return "T_OBJECT "  + baseTypeName(type);

    if (type == (T_OBJECT|TS_ARRAY))  return "T_OBJECT|TS_ARRAY "  + baseTypeName(type);
    if (type == (T_TEXT|TS_ARRAY))    return "T_TEXT|TS_ARRAY "    + baseTypeName(type);
    if (type == (T_ADDRESS|TS_ARRAY)) return "T_ADDRESS|TS_ARRAY " + baseTypeName(type);

    return "Unknown" + asStringU(type);
}

//--------------------------------------------------------------
bool testUpgrade(COptions& options) {

    CBaseNode *node = getNode(options.className);
    if (node) {

        CRuntimeClass *pClass = node->getRuntimeClass();

        cout << SFString('-',80) << "\n";
        cout << "className: " << pClass->m_ClassName << "\n";
        cout << "objectSize: " << pClass->m_ObjectSize << "\n";
        cout << "classSchema: " << pClass->m_classSchema << "\n";
        cout << "baseClass: " << (pClass->m_BaseClass ? pClass->m_BaseClass->m_ClassName : "None") << "\n";
        CFieldList *theList = pClass->m_FieldList;
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

        delete node;
    }

    return true;
}
