/*
 * file npv.hpp
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
 * \file npv.hpp
 * Functions for calculating net present values.
 */

#ifndef SK_FIN_NPV_HEADER_INCLUDED_WOPR8UPSBWPOHWP
#define SK_FIN_NPV_HEADER_INCLUDED_WOPR8UPSBWPOHWP

#include "finance_config.h"
#include "detail.h"
#include <numeric>
#include <cmath>

namespace finance {
	template<typename Iter>
	static double npv(Iter firstPayment, Iter lastPayment, double rate, double inflation, bool arrears){
		double res(0.0);
		rate = (1.0+inflation)/(1.0+rate);
        //std::inner_product(firstPayment, lastPayment, detail::Counter(arrears ? 1.0 : 0.0), 0.0, std::plus<double>(), &pow);
		for(double at(arrears ? 1.0 : 0.0); firstPayment<lastPayment; ++firstPayment, ++at) res += *firstPayment * pow(rate, at);
		return res;
	}

	template<typename PIter, typename TIter>
	static double npv(PIter start, PIter end, TIter times, double i, double r, bool arrears){
        //detail::
		double rate((1+r)/(1+i)), res(0.0);
		for( ; start<end; ++start, ++times) res += *start * pow(rate, *times);
		return res;
	}

	static double npv(double payment, int nPayments, double i, double r, bool arrears){
		double rate((1+r)/(1+i)), res(payment * (pow(rate, nPayments) - 1.0) / (rate - 1.0));
		return arrears ? res * rate : res;
	}

	static double fv(double payment, int nPayments, double i, double r, bool arrears){
		double rate((1+i)/(1+r)), res(payment * (pow(rate, nPayments) - 1.0) / (rate - 1.0));
		return arrears ? res : res * rate;
	}
} // end namespace fin

#endif // SK_FIN_NPV_HEADER_INCLUDED_WOPR8UPSBWPOHWP
