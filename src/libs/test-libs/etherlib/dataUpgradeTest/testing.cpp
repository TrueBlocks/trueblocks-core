/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
extern void reportNode(CBaseNode* node);
namespace qblocks {
extern bool writeNodeToBinary(const CBaseNode& node, const string_q& fileName);
extern bool readNodeFromBinary(CBaseNode& node, const string_q& fileName);
};  // namespace qblocks
//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
    acctlib_init(quickQuitHandler);

    CNewBlock::registerClass();
    CNewReceipt::registerClass();
    UNHIDE_FIELD(CBaseNode, "cname");

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    if (!fileExists("./oldFmt.cache")) {
        CFilename fileName("./oldFmt.cache");
        return options.usage("File '" + fileName.getFullPath() + "' not found. All tests will fail without that file.");
    }

    for (auto command : options.commandLines) {
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
    getBlock_light(latest, "latest");

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
            string_q contents;
            asciiFileToString("./newFmt.json", contents);
            cout << contents << "\n";
            cout.flush();
            break;
        }
        case 2: {
            ASSERT(fileExists("./newFmt.json"));
            cout << "2. Read from JSON, write to new binary format...\n";
            cout.flush();
            readFromJson(block, "./newFmt.json");
            newBlock = CNewBlock(block);
            newBlock.finalized = 1;  // isBlockFinal(newBlock.timestamp, latest.timestamp, (60 * 5));
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
            string_q contents;
            asciiFileToString("./newFmt2.json", contents);
            cout << contents << "\n";
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
            ASSERT(fileExists("./newFmt.cache"));
            remove("./newFmt.cache");
            ASSERT(!fileExists("./newFmt.cache"));

            ASSERT(fileExists("./newFmt.json"));
            remove("./newFmt.json");
            ASSERT(!fileExists("./newFmt.json"));

            ASSERT(fileExists("./newFmt2.json"));
            remove("./newFmt2.json");
            ASSERT(!fileExists("./newFmt2.json"));

            break;
        }
    }
    return true;
}

//--------------------------------------------------------------
CBaseNode* getNode(const string_q& nodeType) {
    CBaseNode* node = NULL;
    if (nodeType == "CAbi")
        node = CAbi::createObject();  // NOLINT
    else if (nodeType == "CFunction")
        node = CFunction::createObject();
    else if (nodeType == "CParameter")
        node = CParameter::createObject();
    else if (nodeType == "CBranch")
        node = CBranch::createObject();
    else if (nodeType == "CInfix")
        node = CInfix::createObject();
    else if (nodeType == "CLeaf")
        node = CLeaf::createObject();
    else if (nodeType == "CTreeNode")
        node = CTreeNode::createObject();
    else if (nodeType == "CTreeRoot")
        node = CTreeRoot::createObject();
    else if (nodeType == "CAccountWatch")
        node = CAccountWatch::createObject();
    else if (nodeType == "CApiSpec")
        node = CApiSpec::createObject();
    else if (nodeType == "CBlock")
        node = CBlock::createObject();
    else if (nodeType == "CIncomeStatement")
        node = CIncomeStatement::createObject();
    else if (nodeType == "CLogEntry")
        node = CLogEntry::createObject();
    else if (nodeType == "CPriceQuote")
        node = CPriceQuote::createObject();
    else if (nodeType == "CReceipt")
        node = CReceipt::createObject();
    else if (nodeType == "CRPCResult")
        node = CRPCResult::createObject();
    else if (nodeType == "CTrace")
        node = CTrace::createObject();
    else if (nodeType == "CTraceAction")
        node = CTraceAction::createObject();
    else if (nodeType == "CTraceResult")
        node = CTraceResult::createObject();
    else if (nodeType == "CTransaction")
        node = CTransaction::createObject();
    else if (nodeType == "CNewBlock")
        node = CNewBlock::createObject();
    else if (nodeType == "CNewReceipt")
        node = CNewReceipt::createObject();
    return node;
}

//--------------------------------------------------------------
bool visitField(const CFieldData& fld, void* data) {
    cout << fld << "\n";
    return true;
}

//--------------------------------------------------------------
void reportNode(CBaseNode* node) {
    CRuntimeClass* pClass = node->getRuntimeClass();
    cout << string_q(80, '-') << "\n";
    cout << "className: " << pClass->m_ClassName << "\n";
    cout << "baseClass: " << (pClass->m_BaseClass ? pClass->m_BaseClass->m_ClassName : "None") << "\n";
    pClass->forEveryField(visitField, NULL);
    cout << node->Format() << "\n";
    cout << "\n";
    cout.flush();
}

//--------------------------------------------------------------
bool testUpgrade(COptions& options) {
    CBaseNode* node = getNode(options.className);
    if (node) {
        reportNode(node);
        node->getRuntimeClass()->hideAllFields();
        reportNode(node);
        node->getRuntimeClass()->showAllFields();
        reportNode(node);
        delete node;
    } else {
        cout << "Unknown node of type " << options.className << " not created.\n";
    }

    return true;
}
