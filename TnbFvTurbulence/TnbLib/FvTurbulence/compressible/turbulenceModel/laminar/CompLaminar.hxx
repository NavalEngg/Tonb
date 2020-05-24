#pragma once
#ifndef _CompLaminar_Header
#define _CompLaminar_Header

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
	tnbLib::compressible::laminar

Description
	Turbulence model for laminar compressible flow.

SourceFiles
	laminar.C

\*---------------------------------------------------------------------------*/

#include <CompTurbulenceModel.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace compressible
	{

		/*---------------------------------------------------------------------------*\
								   Class laminar Declaration
		\*---------------------------------------------------------------------------*/

		class laminar
			:
			public turbulenceModel
		{

		public:

			//- Runtime type information
			TypeName("laminar");

			// Constructors

				//- Construct from components
			laminar
			(
				const volScalarField& rho,
				const volVectorField& U,
				const surfaceScalarField& phi,
				const basicThermo& thermophysicalModel
			);


			// Selectors

				//- Return a reference to the selected turbulence model
			static autoPtr<laminar> New
			(
				const volScalarField& rho,
				const volVectorField& U,
				const surfaceScalarField& phi,
				const basicThermo& thermophysicalModel
			);


			//- Destructor
			virtual ~laminar()
			{}


			// Member Functions

				//- Return the turbulence viscosity, i.e. 0 for laminar flow
			virtual tmp<volScalarField> mut() const;

			//- Return the effective viscosity, i.e. the laminar viscosity
			virtual tmp<volScalarField> muEff() const
			{
				return tmp<volScalarField>(new volScalarField("muEff", mu()));
			}

			//- Return the effective turbulent thermal diffusivity,
			//  i.e. the laminar thermal diffusivity
			virtual tmp<volScalarField> alphaEff() const
			{
				return tmp<volScalarField>(new volScalarField("alphaEff", alpha()));
			}

			//- Return the turbulence kinetic energy, i.e. 0 for laminar flow
			virtual tmp<volScalarField> k() const;

			//- Return the turbulence kinetic energy dissipation rate,
			//  i.e. 0 for laminar flow
			virtual tmp<volScalarField> epsilon() const;

			//- Return the Reynolds stress tensor, i.e. 0 for laminar flow
			virtual tmp<volSymmTensorField> R() const;

			//- Return the effective stress tensor, i.e. the laminar stress
			virtual tmp<volSymmTensorField> devRhoReff() const;

			//- Return the source term for the momentum equation
			virtual tmp<fvVectorMatrix> divDevRhoReff() const;

			//- Correct the laminar viscosity
			virtual void correct();

			//- Read turbulenceProperties dictionary
			virtual bool read();
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace incompressible
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_CompLaminar_Header
