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

#include "config.h"
#include <cmath>
#include <numeric>
#include <iterator>

#pragma warning(push)
#pragma warning(disable : 4996)

/**
 * Contains various code initially developed by Simon Knapp.
 */
namespace SKT {
	/**
	 * Contains various financial calculations.
	 */
	namespace finance {
		/**
		 * Utility code for internal rate of return calculations.
		 */
		namespace detail {
			/**
			 * Binary Function object for calculating the derivitive (w.r.t to the interest rate) of
			 * the present value of a payment.
			 */
			struct Derivator {
				/**
				 * Calculate the derivitive of the present value of a payment.
				 *
				 * \param payment The amount of the payment.
				 * \param period The time of the payment.
				 */
				SK_FIN_ACC_TYPE operator()(SK_FIN_ACC_TYPE payment, SK_FIN_ACC_TYPE period) {
					return -payment * period / pow(1.0 + i, period-1.0);
				}

				/**
				 * The interest rate to be used in the present value calculation.
				 */
				SK_FIN_ACC_TYPE i;
			};

			/**
			 * Binary Function object for calculating the present value of a payment.
			 */
			struct Evaluator {
				/**
				 * Calculate the present value of a payment.
				 *
				 * \param payment The amount of the payment.
				 * \param period The time of the payment.
				 */
				SK_FIN_ACC_TYPE operator()(SK_FIN_ACC_TYPE payment, SK_FIN_ACC_TYPE period) {
					return payment / pow(1.0 + i, period);
				}

				/**
				 * The interest rate to be used in the present value calculation.
				 */
				SK_FIN_ACC_TYPE i;
			};

			/**
			 * A simple counting iterator (which I think is standards compliant).
			 *
			 * I've not bothered to document all members, as they are described elsewhere in the documentation
			 * of the C++ standard library.
			 */
			struct Counter {
				// it would be better to use boost::counting_iterator, but I'd like this to be standalone.
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
} // end namespace SK

#pragma warning(pop)
#endif //SK_FIN_IRR_HEADER_INCLUDED_SPORJISLKSOIIJHSOI
