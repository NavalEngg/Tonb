#pragma once
#ifndef _FDICPreconditioner_Header
#define _FDICPreconditioner_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2011-2019 OpenFOAM Foundation
	 \\/     M anipulation  |
-------------------------------------------------------------------------------
License
	This file is part of OpenFOAM.

	OpenFOAM is free software: you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
	for more details.

	You should have received a copy of the GNU General Public License
	along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
	tnbLib::FDICPreconditioner

Description
	Faster version of the DICPreconditioner diagonal-based incomplete
	Cholesky preconditioner for symmetric matrices
	(symmetric equivalent of DILU) in which the reciprocal of the
	preconditioned diagonal and the upper coefficients divided by the diagonal
	are calculated and stored.

SourceFiles
	FDICPreconditioner.C

\*---------------------------------------------------------------------------*/

#include <lduMatrix.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							   Class FDICPreconditioner Declaration
	\*---------------------------------------------------------------------------*/

	class FDICPreconditioner
		:
		public lduMatrix::preconditioner
	{
		// Private Data

			//- The reciprocal preconditioned diagonal
		scalarField rD_;
		scalarField rDuUpper_;
		scalarField rDlUpper_;


	public:

		//- Runtime type information
		TypeName("FDIC");


		// Constructors

			//- Construct from matrix components and preconditioner solver controls
		FDICPreconditioner
		(
			const lduMatrix::solver&,
			const dictionary& solverControlsUnused
		);

		//- Disallow default bitwise copy construction
		FDICPreconditioner(const FDICPreconditioner&) = delete;


		//- Destructor
		virtual ~FDICPreconditioner()
		{}


		// Member Functions

			//- Return wA the preconditioned form of residual rA
		virtual void precondition
		(
			scalarField& wA,
			const scalarField& rA,
			const direction cmpt = 0
		) const;


		// Member Operators

			//- Disallow default bitwise assignment
		void operator=(const FDICPreconditioner&) = delete;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_FDICPreconditioner_Header
