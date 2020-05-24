#pragma once
#ifndef _mutSptAlmStdWFunFvPatchScalarField_Header
#define _mutSptAlmStdWFunFvPatchScalarField_Header

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
	tnbLib::compressible::RASModels::
	mutSpalartAllmarasStandardWallFunctionFvPatchScalarField

Description
	Wall function boundary condition for walls

SourceFiles
	mutSpalartAllmarasStandardWallFunctionFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#include <mutWallFunctionFvPatchScalarField.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace compressible
	{
		namespace RASModels
		{

			/*---------------------------------------------------------------------------*\
			  Class mutSpalartAllmarasStandardWallFunctionFvPatchScalarField Declaration
			\*---------------------------------------------------------------------------*/

			class mutSpalartAllmarasStandardWallFunctionFvPatchScalarField
				:
				public mutWallFunctionFvPatchScalarField
			{
			protected:

				// Protected member functions

					//- Calculate yPLus
				virtual tmp<scalarField> calcYPlus(const scalarField& magUp) const;

				//- Calculate the turbulence viscosity
				virtual tmp<scalarField> calcMut() const;


			public:

				//- Runtime type information
				TypeName("mutSpalartAllmarasStandardWallFunction");


				// Constructors

					//- Construct from patch and internal field
				mutSpalartAllmarasStandardWallFunctionFvPatchScalarField
				(
					const fvPatch&,
					const DimensionedField<scalar, volMesh>&
				);

				//- Construct from patch, internal field and dictionary
				mutSpalartAllmarasStandardWallFunctionFvPatchScalarField
				(
					const fvPatch&,
					const DimensionedField<scalar, volMesh>&,
					const dictionary&
				);

				//- Construct by mapping given
				//  mutSpalartAllmarasStandardWallFunctionFvPatchScalarField
				//  onto a new patch
				mutSpalartAllmarasStandardWallFunctionFvPatchScalarField
				(
					const mutSpalartAllmarasStandardWallFunctionFvPatchScalarField&,
					const fvPatch&,
					const DimensionedField<scalar, volMesh>&,
					const fvPatchFieldMapper&
				);

				//- Construct as copy
				mutSpalartAllmarasStandardWallFunctionFvPatchScalarField
				(
					const mutSpalartAllmarasStandardWallFunctionFvPatchScalarField&
				);

				//- Construct and return a clone
				virtual tmp<fvPatchScalarField> clone() const
				{
					return tmp<fvPatchScalarField>
						(
							new mutSpalartAllmarasStandardWallFunctionFvPatchScalarField
							(
								*this
							)
							);
				}

				//- Construct as copy setting internal field reference
				mutSpalartAllmarasStandardWallFunctionFvPatchScalarField
				(
					const mutSpalartAllmarasStandardWallFunctionFvPatchScalarField&,
					const DimensionedField<scalar, volMesh>&
				);

				//- Construct and return a clone setting internal field reference
				virtual tmp<fvPatchScalarField> clone
				(
					const DimensionedField<scalar, volMesh>& iF
				) const
				{
					return tmp<fvPatchScalarField>
						(
							new mutSpalartAllmarasStandardWallFunctionFvPatchScalarField
							(
								*this,
								iF
							)
							);
				}


				// Member functions

					// Evaluation functions

						//- Calculate and return the yPlus at the boundary
				virtual tmp<scalarField> yPlus() const;


				// I-O

					//- Write
				virtual void write(Ostream& os) const;
			};


			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

		} // End namespace RASModels
	} // End namespace compressible
} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_mutSptAlmStdWFunFvPatchScalarField_Header
