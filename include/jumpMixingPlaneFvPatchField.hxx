#pragma once
#ifndef _jumpMixingPlaneFvPatchField_Header
#define _jumpMixingPlaneFvPatchField_Header

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
	jumpMixingPlaneFvPatchField

Description
	MixingPlane interface patch field with a jump condition

Author
	Ilaria De Dominicis, General Electric Power, (March 2016)

Contributor
	Hrvoje Jasak, Wikki Ltd.

GE CONFIDENTIAL INFORMATION 2016 General Electric Company. All Rights Reserved

SourceFiles
	jumpMixingPlaneFvPatchField.C

\*---------------------------------------------------------------------------*/

#include <mixingPlaneFvPatchField.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
					Class jumpMixingPlaneFvPatchField Declaration
	\*---------------------------------------------------------------------------*/

	template<class Type>
	class jumpMixingPlaneFvPatchField
		:
		public mixingPlaneFvPatchField<Type>
	{
	public:

		//- Runtime type information
		TypeName("jumpMixingPlane");


		// Constructors

			//- Construct from patch and internal field
		jumpMixingPlaneFvPatchField
		(
			const fvPatch&,
			const DimensionedField<Type, volMesh>&
		);

		//- Construct from patch, internal field and dictionary
		jumpMixingPlaneFvPatchField
		(
			const fvPatch&,
			const DimensionedField<Type, volMesh>&,
			const dictionary&
		);

		//- Construct by mapping given jumpMixingPlaneFvPatchField
		//  onto a new patch
		jumpMixingPlaneFvPatchField
		(
			const jumpMixingPlaneFvPatchField<Type>&,
			const fvPatch&,
			const DimensionedField<Type, volMesh>&,
			const fvPatchFieldMapper&
		);

		//- Construct as copy
		jumpMixingPlaneFvPatchField
		(
			const jumpMixingPlaneFvPatchField&
		);

		//- Construct as copy setting internal field reference
		jumpMixingPlaneFvPatchField
		(
			const jumpMixingPlaneFvPatchField<Type>&,
			const DimensionedField<Type, volMesh>&
		);


		// Member functions

			// Access

				//- Return the interface type
		virtual const word& interfaceFieldType() const
		{
			return mixingPlaneFvPatchField<Type>::type();
		}


		//- Return the "jump" across the patch as a "half" field
		virtual tmp<Field<Type> > jump() const = 0;


		// Evaluation functions

			//- Return neighbour patch field
		virtual tmp<Field<Type> > patchNeighbourField() const;

		//- Initialise neighbour matrix update
		virtual void initInterfaceMatrixUpdate
		(
			const scalarField& psiInternal,
			scalarField& result,
			const lduMatrix& m,
			const scalarField& coeffs,
			const direction cmpt,
			const Pstream::commsTypes commsType,
			const bool switchToLhs
		) const;

		//- Update result field based on interface functionality
		virtual void updateInterfaceMatrix
		(
			const scalarField& psiInternal,
			scalarField& result,
			const lduMatrix&,
			const scalarField& coeffs,
			const direction cmpt,
			const Pstream::commsTypes commsType,
			const bool switchToLhs
		) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include <jumpMixingPlaneFvPatchField.cxx>
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#endif // !_jumpMixingPlaneFvPatchField_Header
