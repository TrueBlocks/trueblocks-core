/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

#include "performance.h"
#include "conversions.h"

namespace qblocks {

    // TODO(tjayrush): global data
    static timepoint_t t1 = qbNow2();
    void resetClock(void) { t1 = qbNow2(); }
    string_q TIC(double div) {
        timepoint_t t2 = qbNow2();
        double ret = std::chrono::duration_cast<duration_t>(t2 - t1).count();
        t1 = t2;
        return double_2_Str(ret / div, 5);
    }
    double qbNow(void) {
        struct timeval tv;
        gettimeofday(&tv, 0);
        double secs = static_cast<double>(tv.tv_sec);
        double usecs = static_cast<double>(tv.tv_usec);
        return (secs + (usecs / 100000.0));
    }

}  // namespace qblocks
