#pragma once
#ifndef _cubicUpwindFitPolynomial_Header
#define _cubicUpwindFitPolynomial_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     | Version:     4.0
	\\  /    A nd           | Web:         http://www.foam-extend.org
	 \\/     M anipulation  | For copyright notice see file Copyright
-------------------------------------------------------------------------------
License
	This file is part of foam-extend.

	foam-extend is free software: you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by the
	Free Software Foundation, either version 3 of the License, or (at your
	option) any later version.

	foam-extend is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with foam-extend.  If not, see <http://www.gnu.org/licenses/>.

Class
	tnbLib::cubicUpwindFitPolynomial

Description
	Cubic polynomial for upwind biased interpolation fitting.

	Can be used with the UpwindFit scheme to create a cubic surface
	interpolation scheme

\*---------------------------------------------------------------------------*/

#include <vector.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
					 Class cubicUpwindFitPolynomial Declaration
	\*---------------------------------------------------------------------------*/

	class cubicUpwindFitPolynomial
	{
	public:

		// Member functions

		static label nTerms(const direction dim)
		{
			return
				(
					dim == 1 ? 4 :
					dim == 2 ? 8 :
					dim == 3 ? 14 : 0
					);
		}

		static void addCoeffs
		(
			scalar* coeffs,
			const vector& d,
			const scalar weight,
			const direction dim
		)
		{
			register label curIdx = 0;

			coeffs[curIdx++] = weight;
			coeffs[curIdx++] = weight * d.x();
			coeffs[curIdx++] = weight * sqr(d.x());
			coeffs[curIdx++] = weight * pow(d.x(), 3);

			if (dim >= 2)
			{
				coeffs[curIdx++] = weight * d.y();
				coeffs[curIdx++] = weight * d.x()*d.y();
				coeffs[curIdx++] = weight * sqr(d.y());
				coeffs[curIdx++] = weight * d.x()*sqr(d.y());
			}
			if (dim == 3)
			{
				coeffs[curIdx++] = weight * d.z();
				coeffs[curIdx++] = weight * d.x()*d.z();
				coeffs[curIdx++] = weight * d.y()*d.z();
				coeffs[curIdx++] = weight * sqr(d.z());
				coeffs[curIdx++] = weight * d.x()*d.y()*d.z();
				coeffs[curIdx++] = weight * d.x()*sqr(d.z());
			}
		}
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_cubicUpwindFitPolynomial_Header
