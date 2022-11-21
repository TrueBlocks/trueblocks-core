/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "transaction.h"
#include "etherlib.h"

namespace qblocks {

//-----------------------------------------------------------------------
string_q CTransaction::getReconcilationPath(const address_t& address) const {
    string_q path = getPathToBinaryCache(CT_RECONS, address, blockNumber, transactionIndex);
    establishFolder(path);
    return path;
}

//-----------------------------------------------------------------------
bool CTransaction::readReconsFromCache(const address_t& accountedFor) {
    statements.clear();
    if (isTestMode()) {
        return false;
    }

    string_q path = getReconcilationPath(accountedFor);
    if (!fileExists(path)) {
        return false;
    }

    CArchive archive(READING_ARCHIVE);
    if (archive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
        archive >> statements;
        archive.Release();
        for (auto& statement : statements) {
            if (statement.m_schema < getVersionNum(0, 43, 0)) {
                // This is an old version of the reconciliation, delete it and return false
                LOG_WARN("Back-level cache for statements found. Removing....");
                statements.clear();
                ::remove(path.c_str());
                return false;
            }

            // Freshen in case user has changed the names database since putting the statement in the cache
            CAccountName tokenName;
            if (findToken(statement.assetAddr, tokenName)) {
                statement.assetSymbol = tokenName.symbol;
                statement.decimals = tokenName.decimals;
            }

            statement.pTransaction = this;
        }
        return !shouldQuit();
    }

    return false;
}

//-----------------------------------------------------------------------
void CTransaction::cacheIfReconciled(const address_t& accountedFor) const {
    if (isTestMode() || !isReconciled(accountedFor)) {
        return;
    }

    lockSection();

    string_q path = getReconcilationPath(accountedFor);
    CArchive archive(WRITING_ARCHIVE);
    if (archive.Lock(path, modeWriteCreate, LOCK_WAIT)) {
        LOG4("Writing to cache for ", path);
        archive << statements;
        archive.Release();
    }

    unlockSection();
}

//-----------------------------------------------------------------------
bool CTransaction::isReconciled(const address_t& accountedFor) const {
    bool ret = true;
    for (auto statement : statements) {
        if (!statement.trialBalance()) {
            ostringstream os;
            os << bMagenta << "unreconciled " << (isEtherAddr(statement.assetAddr) ? "eth-transfer" : "tok-transfer")
               << ": " << cOff << "chifra export " << accountedFor
               << " --accounting --statements --ether --fmt json --first_block " << (blockNumber - 1)
               << " --last_block " << (blockNumber + 1);
            LOG_WARN(os.str());
            ret = false;
        }
    }
    return ret;
}

}  // namespace qblocks
