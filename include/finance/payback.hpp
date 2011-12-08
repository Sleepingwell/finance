/*
 * file payback.hpp
 * Copyright (C) 2011       Simon Knapp
 *
 * This progrm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file payback.hpp
 * Functions and associated utilities for calculating (non-discounted) payback periods.
 */
#ifndef SK_FIN_PAYBACK_HEADER_INCLUDED_POIURWIFBNIUHJW
#define SK_FIN_PAYBACK_HEADER_INCLUDED_POIURWIFBNIUHJW

#include "finance_config.h"
#include <limits>
#include <cassert>

namespace finance {
	template<typename Iter>
	static SK_FIN_ACC_TYPE paybackPeriod(Iter start, Iter end, SK_FIN_ACC_TYPE expenditure) {
		if(expenditure <= 0.0) return 0.0;
		SK_FIN_ACC_TYPE tot(0.0), at(0.0);
		bool found(false);
		for(; start != end; ++start, ++at) if(found = (tot += *start) > expenditure) break;
		if(!found) return std::numeric_limits<SK_FIN_ACC_TYPE>::infinity();
		tot = (at + (expenditure - (tot - *start)) / *start);
		assert(tot >= 0.0);
		return tot;
	}
} // end namespace finance

#endif //SK_FIN_PAYBACK_HEADER_INCLUDED_POIURWIFBNIUHJW
