#pragma once
#ifndef _MapGeometricFields_Header
#define _MapGeometricFields_Header

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
	tnbLib::MapInternalField

Description
	Generic internal field mapper.  For "real" mapping, add template
	specialisations for mapping of internal fields depending on mesh
	type.

\*---------------------------------------------------------------------------*/

#include <polyMesh.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	template<class Type, class MeshMapper, class GeoMesh>
	class MapInternalField
	{
	public:

		MapInternalField()
		{}

		void operator()
			(
				Field<Type>& field,
				const MeshMapper& mapper
				) const;
	};


	//- Generic Geometric field mapper.
	//  For "real" mapping, add template specialisations
	//  for mapping of internal fields depending on mesh type.
	template
		<
		class Type,
		template<class> class PatchField,
		class MeshMapper,
		class GeoMesh
		>
		void MapGeometricFields
		(
			const MeshMapper& mapper
		)
	{
		HashTable<const GeometricField<Type, PatchField, GeoMesh>*> fields
		(
			mapper.thisDb().objectRegistry::template lookupClass
			<GeometricField<Type, PatchField, GeoMesh> >()
		);

		// It is necessary to enforce that all old-time fields are stored
		// before the mapping is performed.  Otherwise, if the
		// old-time-level field is mapped before the field itself, sizes
		// will not match.

		for
			(
				typename HashTable<const GeometricField<Type, PatchField, GeoMesh>*>::
				iterator fieldIter = fields.begin();
				fieldIter != fields.end();
				++fieldIter
				)
		{
			GeometricField<Type, PatchField, GeoMesh>& field =
				const_cast<GeometricField<Type, PatchField, GeoMesh>&>
				(*fieldIter());

			//Note: check can be removed once pointFields are actually stored on
			//      the pointMesh instead of now on the polyMesh!
			if (&field.mesh() == &mapper.mesh())
			{
				field.storeOldTimes();
			}
		}

		for
			(
				typename HashTable<const GeometricField<Type, PatchField, GeoMesh>*>::
				iterator fieldIter = fields.begin();
				fieldIter != fields.end();
				++fieldIter
				)
		{
			GeometricField<Type, PatchField, GeoMesh>& field =
				const_cast<GeometricField<Type, PatchField, GeoMesh>&>
				(*fieldIter());

			if (&field.mesh() == &mapper.mesh())
			{
				if (polyMesh::debug)
				{
					Info << "Mapping " << field.typeName << ' ' << field.name()
						<< endl;
				}

				// Map the internal field
				MapInternalField<Type, MeshMapper, GeoMesh>()
					(
						field.internalField(),
						mapper
						);

				// Map the patch fields
				forAll(field.boundaryField(), patchi)
				{
					// Cannot check sizes for patch fields because of
					// empty fields in FV and because point fields get their size
					// from the patch which has already been resized

					field.boundaryField()[patchi].autoMap
					(
						mapper.boundaryMap()[patchi]
					);
				}

				field.instance() = field.time().timeName();
			}
			else if (polyMesh::debug)
			{
				Info << "Not mapping " << field.typeName << ' ' << field.name()
					<< " since originating mesh differs from that of mapper."
					<< endl;
			}
		}
	}


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_MapGeometricFields_Header
