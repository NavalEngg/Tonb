#pragma once
#ifndef _SubList_Header
#define _SubList_Header

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
	tnbLib::SubList

Description
	A List obtained as a section of another List.

	Since the SubList is itself unallocated, no storage is allocated or
	de-allocated during its use.  To achieve this behaviour, SubList is
	derived from UList rather than List.

SourceFiles
	SubListI.H

\*---------------------------------------------------------------------------*/

#include <List.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							   Class SubList Declaration
	\*---------------------------------------------------------------------------*/

	template<class T>
	class SubList
		:
		public UList<T>
	{

	public:

		// Static Member Functions

			//- Return a null SubList
		inline static const SubList<T>& null();


		// Constructors

			//- Construct from UList and sub-list size
		inline SubList
		(
			const UList<T>& list,
			const label subSize
		);

		//- Construct from UList, sub-list size and start index
		inline SubList
		(
			const UList<T>& list,
			const label subSize,
			const label startIndex
		);

		//- Copy constructor
		inline SubList(const SubList<T>& sl) = default;


		// Member Operators

			//- Allow cast to a const List<T>&
		inline operator const tnbLib::List<T>&() const;

		//- Assignment of all entries to the given sub-list
		inline void operator=(const SubList<T>&);

		//- Assignment of all entries to the given list
		inline void operator=(const UList<T>&);

		//- Assignment of all entries to the given value
		inline void operator=(const T&);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <SubListI.hxx>

#endif // !_SubList_Header
