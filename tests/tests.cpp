/*
 * file tests.hpp
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

#include "../include/SKT/finance/irr.h"
#include "../include/SKT/finance/npv.h"
#include "../include/SKT/finance/payback.h"

int main(int argc, char* argv[]) {
	unsigned int nPayments(10), i;
	double *payments = new double[nPayments];
	double *times = new double[nPayments];
	double tolerance(0.001);
	payments[0] = -1.0*nPayments;
	times[0] = 0.0;
	for(i=1; i<nPayments; ++i) {
		payments[i] = 5.0;
		times[i] = static_cast<double>(i);
	}
	printf("irrt = %f\n", SKT::finance::irr(payments, payments+nPayments, times, times+nPayments, tolerance));
	printf("irr = %f\n", SKT::finance::irr(payments, payments+nPayments, false, tolerance));
	printf("fva = %f\n", SKT::finance::fv(5.0, 10, .1, 0.0, true));
	printf("fv = %f\n", SKT::finance::fv(5.0, 10, .1, 0.0, false));
	printf("npva = %f\n", SKT::finance::npv(5.0, 10, .1, 0.0, true));
	printf("npv = %f\n", SKT::finance::npv(5.0, 10, .1, 0.0, false));
	printf("pp = %f\n", SKT::finance::paybackPeriod(payments+1, payments+nPayments, 26.34));
	delete [] payments;
	delete [] times;
	system("PAUSE");
	return 0;
}
