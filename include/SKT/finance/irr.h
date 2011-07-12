/*
 * file irr.hpp
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

#ifndef SK_FIN_IRR_HEADER_INCLUDED_SPORJISLKSOIIJHSOI
#define SK_FIN_IRR_HEADER_INCLUDED_SPORJISLKSOIIJHSOI

#include "config.h"
#include <cmath>
#include <numeric>
#include <iterator>

#pragma warning(push)
#pragma warning(disable : 4996)

namespace SKT {
	namespace finance {
		namespace detail {
			struct Derivator {
				SK_FIN_ACC_TYPE operator()(SK_FIN_ACC_TYPE payment, SK_FIN_ACC_TYPE period) {
					return -payment * period / pow(1.0 + i, period-1.0);
				}
				SK_FIN_ACC_TYPE i;
			};

			struct Evaluator {
				SK_FIN_ACC_TYPE operator()(SK_FIN_ACC_TYPE payment, SK_FIN_ACC_TYPE period) {
					return payment / pow(1.0 + i, period);
				}
				SK_FIN_ACC_TYPE i;
			};

			// it would be better to use boost::counting_iterator, but I'd like this to be standalone.
			struct Counter {
				typedef std::input_iterator_tag iterator_category;
				typedef SK_FIN_ACC_TYPE value_type;
				typedef ptrdiff_t difference_type;
				typedef ptrdiff_t distance_type; // not sure if this is needed.
				typedef SK_FIN_ACC_TYPE const* pointer;
				typedef SK_FIN_ACC_TYPE const& reference;

				Counter(SK_FIN_ACC_TYPE start) 
					: count_(start) {
				}

				inline Counter const& operator++(void) {
					++count_;
					return *this;
				}

				inline reference operator*(void) const {
					return count_;
				}

				// not really needed here
				inline Counter const operator++(int) {
					Counter tmp = *this;
					++count_;
					return tmp;
				}

			private:
				SK_FIN_ACC_TYPE count_;
			};
		} // end namespace private

		template<typename IterP, typename IterT>
		SK_FIN_ACC_TYPE irr(IterP beginp, IterP endp, IterT begint, SK_FIN_ACC_TYPE tolerance=0.01, unsigned int maxIterations = 1000) {
			SK_FIN_ACC_TYPE val(tolerance + 0.1), i(0.0);
			detail::Evaluator e;
			detail::Derivator d;
			// should really signal if maxIterations has been exceeded.
			for(; abs(val) > tolerance && maxIterations >= 0; --maxIterations) {
				e.i = d.i = i;
				val = std::inner_product(beginp, endp, begint, 0.0, std::plus<SK_FIN_ACC_TYPE>(), e);
				i -= val / std::inner_product(beginp, endp, begint, 0.0, std::plus<SK_FIN_ACC_TYPE>(), d);
			}
			return i;
		}

		template<typename IterP, typename IterT>
		SK_FIN_ACC_TYPE irr(IterP beginp, IterP endp, IterT begint, IterT endt, SK_FIN_ACC_TYPE tolerance=0.01, unsigned int maxIterations = 1000) {
			typename std::iterator_traits<IterP>::difference_type lenp(endp-beginp), lent(endt-begint);
			if(lenp > lent) endp = beginp + lent;
			return irr(beginp, endp, begint, tolerance, maxIterations);
		}

		template<typename IterP>
		SK_FIN_ACC_TYPE irr(IterP beginp, IterP endp, bool arrears, SK_FIN_ACC_TYPE tolerance=0.01, unsigned int maxIterations = 1000) {
			return irr(beginp, endp, detail::Counter(arrears ? 1.0 : 0.0), tolerance, maxIterations);
		}
	} // end namespace finance
} // end namespace SK

#pragma warning(pop)
#endif //SK_FIN_IRR_HEADER_INCLUDED_SPORJISLKSOIIJHSOI
