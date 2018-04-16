#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#ifdef BUILD_SQL
#include "sql_database.h"

namespace qblocks {
    bool init_sqldatabase();
} // namespace qblocks
#endif
