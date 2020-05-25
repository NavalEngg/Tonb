#pragma once
#ifndef _fvmLaplacian_Header
#define _fvmLaplacian_Header

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

InNamespace
	tnbLib::fvm

Description
	Calculate the matrix for the laplacian of the field.

SourceFiles
	fvmLaplacian.C

\*---------------------------------------------------------------------------*/

#include <tmp.hxx>
#include <volFieldsFwd.hxx>
#include <surfaceFieldsFwd.hxx>
#include <fvMatrices.hxx>
#include <geometricOneField.hxx>
#include <dimensionedTypes.hxx>
#include <zeroField.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						 Namespace fvm functions Declaration
	\*---------------------------------------------------------------------------*/

	namespace fvm
	{
		template<class Type>
		tmp<fvMatrix<Type> > laplacian
		(
			const GeometricField<Type, fvPatchField, volMesh>&,
			const word&
		);

		template<class Type>
		tmp<fvMatrix<Type> > laplacian
		(
			const GeometricField<Type, fvPatchField, volMesh>&
		);


		template<class Type>
		tmp<fvMatrix<Type> > laplacian
		(
			const zeroField&,
			const GeometricField<Type, fvPatchField, volMesh>&,
			const word&
		);

		template<class Type>
		tmp<fvMatrix<Type> > laplacian
		(
			const zeroField&,
			const GeometricField<Type, fvPatchField, volMesh>&
		);


		template<class Type>
		tmp<fvMatrix<Type> > laplacian
		(
			const geometricOneField&,
			const GeometricField<Type, fvPatchField, volMesh>&,
			const word&
		);

		template<class Type>
		tmp<fvMatrix<Type> > laplacian
		(
			const geometricOneField&,
			const GeometricField<Type, fvPatchField, volMesh>&
		);


		template<class Type, class GType>
		tmp<fvMatrix<Type> > laplacian
		(
			const dimensioned<GType>&,
			const GeometricField<Type, fvPatchField, volMesh>&,
			const word&
		);

		template<class Type, class GType>
		tmp<fvMatrix<Type> > laplacian
		(
			const dimensioned<GType>&,
			const GeometricField<Type, fvPatchField, volMesh>&
		);


		template<class Type, class GType>
		tmp<fvMatrix<Type> > laplacian
		(
			const GeometricField<GType, fvPatchField, volMesh>&,
			const GeometricField<Type, fvPatchField, volMesh>&,
			const word&
		);

		template<class Type, class GType>
		tmp<fvMatrix<Type> > laplacian
		(
			const GeometricField<GType, fvPatchField, volMesh>&,
			const GeometricField<Type, fvPatchField, volMesh>&
		);


		template<class Type, class GType>
		tmp<fvMatrix<Type> > laplacian
		(
			const tmp<GeometricField<GType, fvPatchField, volMesh> >&,
			const GeometricField<Type, fvPatchField, volMesh>&,
			const word&
		);

		template<class Type, class GType>
		tmp<fvMatrix<Type> > laplacian
		(
			const tmp<GeometricField<GType, fvPatchField, volMesh> >&,
			const GeometricField<Type, fvPatchField, volMesh>&
		);


		template<class Type, class GType>
		tmp<fvMatrix<Type> > laplacian
		(
			const GeometricField<GType, fvsPatchField, surfaceMesh>&,
			const GeometricField<Type, fvPatchField, volMesh>&,
			const word&
		);

		template<class Type, class GType>
		tmp<fvMatrix<Type> > laplacian
		(
			const tmp<GeometricField<GType, fvsPatchField, surfaceMesh> >&,
			const GeometricField<Type, fvPatchField, volMesh>&,
			const word&
		);

		template<class Type, class GType>
		tmp<fvMatrix<Type> > laplacian
		(
			const GeometricField<GType, fvsPatchField, surfaceMesh>&,
			const GeometricField<Type, fvPatchField, volMesh>&
		);

		template<class Type, class GType>
		tmp<fvMatrix<Type> > laplacian
		(
			const tmp<GeometricField<GType, fvsPatchField, surfaceMesh> >&,
			const GeometricField<Type, fvPatchField, volMesh>&
		);
	}


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include <fvmLaplacian.cxx>
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_fvmLaplacian_Header
