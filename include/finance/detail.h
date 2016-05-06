#ifndef COUNTING_ITERATORAT_HEADER_INCLUDED_PE98TUW0EOIBNOIJPT
#define COUNTING_ITERATORAT_HEADER_INCLUDED_PE98TUW0EOIBNOIJPT

#include "finance_config.h"
#include <iterator>
#include <cmath>


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
            Derivator(SK_FIN_ACC_TYPE i=0.0) : i(i) {}
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
            Evaluator(SK_FIN_ACC_TYPE i=0.0) : i(i) {}
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
         * @note it would be better to use boost::counting_iterator, but I'd like this library to be standalone.
         *
		 * @note I've not bothered to document all members, as they are described elsewhere in the documentation
		 * of the C++ standard library.
		 */
		struct Counter {
			// 
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
    } // end namespace detail
} // end namespace finance

#endif //COUNTING_ITERATORAT_HEADER_INCLUDED_PE98TUW0EOIBNOIJPT
