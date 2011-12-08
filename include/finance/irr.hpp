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

/**
 * \file irr.hpp
 * Functions and associated utilities for calculating internal rates of return.
 */
#ifndef SK_FIN_IRR_HEADER_INCLUDED_SPORJISLKSOIIJHSOI
#define SK_FIN_IRR_HEADER_INCLUDED_SPORJISLKSOIIJHSOI

#include "finance_config.h"
#include "detail.h"
#include <cmath>
#include <numeric>

#pragma warning(push)
#pragma warning(disable : 4996)

/**
 * Contains various financial calculations.
 */
namespace finance {

	/**
	 * Calculates the internal rate of return for a series of payments using Newton's method.
	 *
	 * The algorithm starts with an initial estimate of \c 0.0, and runs until the result is
	 * either within \p tolerance of the 'true' root, or for a maximum of \p maxIterations iterations.
	 *
	 * \tparam IterP The type of the iterators over the payments.
	 * \tparam IterT The type of the iterator over the time periods.
	 *
	 * \param beginp Iterator pointing to the beginning of the payments.
	 * \param endp Iterator pinting to the end of the payments.
	 * \param begint Iterator pointing the beginning of the times of the payments
	 * \param tolerance Specifies how 'exact' the answer should be (the answer will be within \p tolerance of
	 * the true value).
	 * \param maxIterations The maximum number of iterations to run.
	 *
	 * \todo There should be a mechanism for informing the caller if \p maxIterations was reached.
	 */
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

	/**
	 * \overload
	 *
	 * Calculates the internal rate of return for a series of payments using Newton's method.
	 *
	 * The algorithm starts with an initial estimate of \c 0.0, and runs until the result is
	 * either within \p tolerance of the 'true' root, or for a maximum of \p maxIterations iterations.
	 *
	 * \tparam IterP The type of the iterators over the payments.
	 * \tparam IterT The type of the iterator over the time periods.
	 *
	 * \param beginp Iterator pointing to the beginning of the payments.
	 * \param endp Iterator pinting to the end of the payments.
	 * \param begint Iterator pointing the beginning of the times of the payments.
	 * \param endt Iterator pointing to the end of the times of the payments. If there are more time points
	 * than payments, then the number of payments is reduced to match the number of time periods.
	 * \param tolerance Specifies how 'exact' the answer should be (the answer will be within \p tolerance of
	 * the true value).
	 * \param maxIterations The maximum number of iterations to run.
	 */
	template<typename IterP, typename IterT>
	SK_FIN_ACC_TYPE irr(IterP beginp, IterP endp, IterT begint, IterT endt, SK_FIN_ACC_TYPE tolerance=0.01, unsigned int maxIterations = 1000) {
		typename std::iterator_traits<IterP>::difference_type lenp(endp-beginp), lent(endt-begint);
		if(lenp > lent) endp = beginp + lent;
		return irr(beginp, endp, begint, tolerance, maxIterations);
	}

	/**
	 * Calculates the internal rate of return for a series of payments using Newton's method.
	 *
	 * The algorithm starts with an initial estimate of \c 0.0, and runs until the result is
	 * either within \p tolerance of the 'true' root, or for a maximum of \p maxIterations iterations.
	 *
	 * \tparam IterP The type of the iterators over the payments.
	 * \tparam IterT The type of the iterator over the time periods.
	 *
	 * \param beginp Iterator pointing to the beginning of the payments.
	 * \param endp Iterator pinting to the end of the payments.
	 * \param arrears Whether the payments are in arrears (\c true) or in advance (\c false).
	 * \param tolerance Specifies how 'exact' the answer should be (the answer will be within \p tolerance of
	 * the true value).
	 * \param maxIterations The maximum number of iterations to run.
	 *
	 * This version assumes that the payments are one period appart.
	 */
	template<typename IterP>
	SK_FIN_ACC_TYPE irr(IterP beginp, IterP endp, bool arrears, SK_FIN_ACC_TYPE tolerance=0.01, unsigned int maxIterations = 1000) {
		return irr(beginp, endp, detail::Counter(arrears ? 1.0 : 0.0), tolerance, maxIterations);
	}
} // end namespace finance

#pragma warning(pop)
#endif //SK_FIN_IRR_HEADER_INCLUDED_SPORJISLKSOIIJHSOI
