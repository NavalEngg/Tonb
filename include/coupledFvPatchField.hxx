#pragma once
#ifndef _coupledFvPatchField_Header
#define _coupledFvPatchField_Header

#include <BlockLduInterfaceField.hxx>
#include <CoeffField.hxx>

#include <lduInterfaceField.hxx>
#include <fvPatchField.hxx>
#include <coupledFvPatch.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						Class coupledFvPatchField Declaration
	\*---------------------------------------------------------------------------*/

	template<class Type>
	class coupledFvPatchField
		:
		public BlockLduInterfaceField<Type>,
		public fvPatchField<Type>
	{

	public:

		//- Runtime type information
		TypeName(coupledFvPatch::typeName_());


		// Constructors

			//- Construct from patch and internal field
		coupledFvPatchField
		(
			const fvPatch&,
			const DimensionedField<Type, volMesh>&
		);

		//- Construct from patch and internal field and patch field
		coupledFvPatchField
		(
			const fvPatch&,
			const DimensionedField<Type, volMesh>&,
			const Field<Type>&
		);

		//- Construct from patch, internal field and dictionary
		coupledFvPatchField
		(
			const fvPatch&,
			const DimensionedField<Type, volMesh>&,
			const dictionary&,
			const bool valueRequired = false
		);

		//- Construct by mapping the given coupledFvPatchField onto a new patch
		coupledFvPatchField
		(
			const coupledFvPatchField<Type>&,
			const fvPatch&,
			const DimensionedField<Type, volMesh>&,
			const fvPatchFieldMapper&
		);

		//- Construct as copy
		coupledFvPatchField
		(
			const coupledFvPatchField<Type>&
		);

		//- Construct and return a clone
		virtual tmp<fvPatchField<Type> > clone() const = 0;

		//- Construct as copy setting internal field reference
		coupledFvPatchField
		(
			const coupledFvPatchField<Type>&,
			const DimensionedField<Type, volMesh>&
		);

		//- Construct and return a clone
		virtual tmp<fvPatchField<Type> > clone
		(
			const DimensionedField<Type, volMesh>&
		) const = 0;


		// Member functions

			// Access

				//- Return true if this patch field is derived from
				//  coupledFvPatchField<Type>.
		virtual bool coupled() const
		{
			return true;
		}

		//- Return neighbour field of internal field
		virtual tmp<Field<Type> > patchNeighbourField() const = 0;


		// Evaluation functions

			//- Return patch-normal gradient
		virtual tmp<Field<Type> > snGrad() const;

		//- Initialise the evaluation of the patch field
		virtual void initEvaluate
		(
			const Pstream::commsTypes commsType
		);

		//- Evaluate the patch field
		virtual void evaluate
		(
			const Pstream::commsTypes commsType
		);

		//- Return the matrix diagonal coefficients corresponding to the
		//  evaluation of the value of this patchField with given weights
		virtual tmp<Field<Type> > valueInternalCoeffs
		(
			const tmp<scalarField>&
		) const;

		//- Return the matrix source coefficients corresponding to the
		//  evaluation of the value of this patchField with given weights
		virtual tmp<Field<Type> > valueBoundaryCoeffs
		(
			const tmp<scalarField>&
		) const;

		//- Return the matrix diagonal coefficients corresponding to the
		//  evaluation of the gradient of this patchField
		virtual tmp<Field<Type> > gradientInternalCoeffs() const;

		//- Return the matrix source coefficients corresponding to the
		//  evaluation of the gradient of this patchField
		virtual tmp<Field<Type> > gradientBoundaryCoeffs() const;


		// Coupled interface functionality

			//- Transform given patch component field
		virtual void transformCoupleField
		(
			scalarField& f,
			const direction cmpt
		) const = 0;

		//- Update result field based on interface functionality
		virtual void updateInterfaceMatrix
		(
			const scalarField& psiInternal,
			scalarField& result,
			const lduMatrix&,
			const scalarField& coeffs,
			const direction,
			const Pstream::commsTypes commsType,
			const bool switchToLhs
		) const = 0;


		// Block coupled interface functionality

			//- Transform given patch field
		virtual void transformCoupleField
		(
			Field<Type>& f
		) const
		{
			notImplemented
			(
				"coupledFvPatchField<Type>::transformCoupleField"
				"(Field<Type>& f) const"
			);
		}

		//- Initialise neighbour matrix update
		virtual void initInterfaceMatrixUpdate
		(
			const Field<Type>&,
			Field<Type>&,
			const BlockLduMatrix<Type>&,
			const CoeffField<Type>&,
			const Pstream::commsTypes commsType,
			const bool switchToLhs
		) const
		{}

		//- Update result field based on interface functionality
		virtual void updateInterfaceMatrix
		(
			const Field<Type>&,
			Field<Type>&,
			const BlockLduMatrix<Type>&,
			const CoeffField<Type>&,
			const Pstream::commsTypes commsType,
			const bool switchToLhs
		) const
		{
			notImplemented
			(
				"coupledFvPatchField<Type>::updateInterfaceMatrix"
			);
		}


		//- Write
		virtual void write(Ostream&) const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include <coupledFvPatchField.cxx>
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#endif // !_coupledFvPatchField_Header
