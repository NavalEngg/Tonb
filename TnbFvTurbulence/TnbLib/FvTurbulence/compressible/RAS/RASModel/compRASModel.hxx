#pragma once
#ifndef _compRASModel_Header
#define _compRASModel_Header

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

Namespace
	tnbLib::compressible::RASModels

Description
	Namespace for compressible RAS turbulence models.


Class
	tnbLib::compressible::RASModel

Description
	Abstract base class for turbulence models for compressible and combusting
	flows.

SourceFiles
	RASModel.C
	newTurbulenceModel.C

\*---------------------------------------------------------------------------*/

#include <CompTurbulenceModel.hxx>
#include <volFields.hxx>
#include <surfaceFields.hxx>
#include <nearWallDist.hxx>
#include <fvm.hxx>
#include <fvc.hxx>
#include <fvMatrices.hxx>
#include <basicThermo.hxx>
#include <IOdictionary.hxx>
#include <Switch.hxx>
#include <bound.hxx>
#include <autoPtr.hxx>
#include <runTimeSelectionTables.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace compressible
	{

		/*---------------------------------------------------------------------------*\
								   Class RASModel Declaration
		\*---------------------------------------------------------------------------*/

		class RASModel
			:
			public turbulenceModel,
			public IOdictionary
		{

		protected:

			// Protected data

				//- Turbulence on/off flag
			Switch turbulence_;

			//- Flag to print the model coeffs at run-time
			Switch printCoeffs_;

			//- Model coefficients dictionary
			dictionary coeffDict_;

			//- Value of y+ at the edge of the laminar sublayer
			scalar yPlusLam_;

			//- Lower limit of k
			dimensionedScalar k0_;

			//- Lower limit of epsilon
			dimensionedScalar epsilon0_;

			//- Small epsilon value used to avoid divide by zero
			dimensionedScalar epsilonSmall_;

			//- Lower limit for omega
			dimensionedScalar omega0_;

			//- Small omega value used to avoid divide by zero
			dimensionedScalar omegaSmall_;

			//- Laminar to turbulent viscosity ratio
			scalar muRatio_;

			//- Near wall distance boundary field
			nearWallDist y_;


			// Protected member functions

				//- Print model coefficients
			virtual void printCoeffs();


		private:

			// Private Member Functions

				//- Disallow default bitwise copy construct
			RASModel(const RASModel&);

			//- Disallow default bitwise assignment
			void operator=(const RASModel&);


		public:

			//- Runtime type information
			TypeName("RASModel");


			// Declare run-time constructor selection table

#ifndef SWIG
			declareRunTimeSelectionTable
			(
				autoPtr,
				RASModel,
				dictionary,
				(
					const volScalarField& rho,
					const volVectorField& U,
					const surfaceScalarField& phi,
					const basicThermo& thermoPhysicalModel
					),
					(rho, U, phi, thermoPhysicalModel)
			);
#endif

			// Constructors

				//- Construct from components
			RASModel
			(
				const word& type,
				const volScalarField& rho,
				const volVectorField& U,
				const surfaceScalarField& phi,
				const basicThermo& thermoPhysicalModel
			);


			// Selectors

				//- Return a reference to the selected turbulence model
			static autoPtr<compressible::RASModel> New
			(
				const volScalarField& rho,
				const volVectorField& U,
				const surfaceScalarField& phi,
				const basicThermo& thermoPhysicalModel
			);


			//- Destructor
			virtual ~RASModel()
			{}


			// Member Functions

				// Access

					//- Return the value of k0 which k is not allowed to be less than
			const dimensionedScalar& k0() const
			{
				return k0_;
			}

			//- Return the value of epsilon0 which epsilon is not allowed to be
			//  less than
			const dimensionedScalar& epsilon0() const
			{
				return epsilon0_;
			}

			//- Return the value of epsilonSmall which is added to epsilon when
			//  calculating nut
			const dimensionedScalar& epsilonSmall() const
			{
				return epsilonSmall_;
			}

			//- Return the value of omega0 which epsilon is not allowed to be
			//  less than
			const dimensionedScalar& omega0() const
			{
				return omega0_;
			}

			//- Return the value of omegaSmall which is added to epsilon when
			//  calculating nut
			const dimensionedScalar& omegaSmall() const
			{
				return omegaSmall_;
			}

			//- Return maximum allowed ratio of laminar to turbulent viscosity
			scalar muRatio() const
			{
				return muRatio_;
			}

			//- Allow k0 to be changed
			dimensionedScalar& k0()
			{
				return k0_;
			}

			//- Allow epsilon0 to be changed
			dimensionedScalar& epsilon0()
			{
				return epsilon0_;
			}

			//- Allow epsilonSmall to be changed
			dimensionedScalar& epsilonSmall()
			{
				return epsilonSmall_;
			}

			//- Allow omega0 to be changed
			dimensionedScalar& omega0()
			{
				return omega0_;
			}

			//- Allow omegaSmall to be changed
			dimensionedScalar& omegaSmall()
			{
				return omegaSmall_;
			}

			//- Allow nuRatio to be modified
			scalar& muRatio()
			{
				return muRatio_;
			}

			//- Return the near wall distances
			const nearWallDist& y() const
			{
				return y_;
			}

			//- Calculate y+ at the edge of the laminar sublayer
			scalar yPlusLam(const scalar kappa, const scalar E) const;

			//- Const access to the coefficients dictionary
			const dictionary& coeffDict() const
			{
				return coeffDict_;
			}


			//- Return the turbulence viscosity
			virtual tmp<volScalarField> mut() const = 0;

			//- Return the effective viscosity
			virtual tmp<volScalarField> muEff() const;

			//- Return the effective turbulent thermal diffusivity
			virtual tmp<volScalarField> alphaEff() const = 0;

			//- Return the turbulence kinetic energy
			virtual tmp<volScalarField> k() const = 0;

			//- Return the turbulence kinetic energy dissipation rate
			virtual tmp<volScalarField> epsilon() const = 0;

			//- Return the Reynolds stress tensor
			virtual tmp<volSymmTensorField> R() const = 0;

			//- Return the effective stress tensor including the laminar stress
			virtual tmp<volSymmTensorField> devRhoReff() const = 0;

			//- Return the source term for the momentum equation
			virtual tmp<fvVectorMatrix> divDevRhoReff() const = 0;

			//- Return yPlus for the given patch
			virtual tmp<scalarField> yPlus
			(
				const label patchI,
				const scalar Cmu
			) const;

			//- Solve the turbulence equations and correct the turbulence viscosity
			virtual void correct() = 0;

			//- Read RASProperties dictionary
			virtual bool read() = 0;
		};


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace compressible
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_RASModel_Header
