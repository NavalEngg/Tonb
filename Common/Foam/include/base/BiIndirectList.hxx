#pragma once
#ifndef _BiIndirectList_Header
#define _BiIndirectList_Header

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
	tnbLib::BiIndirectList

Description
	Indexes into negList (negative index) or posList (zero or positive index).

SourceFiles
	BiIndirectListI.H

\*---------------------------------------------------------------------------*/

#include <List.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						   Class BiIndirectList Declaration
	\*---------------------------------------------------------------------------*/

	template<class T>
	class BiIndirectList
	{
		// Private Data

		UList<T>& posList_;
		UList<T>& negList_;
		List<label> addressing_;


	public:

		// Constructors

			//- Construct given the complete lists and the addressing array
		inline BiIndirectList
		(
			const UList<T>& posList,
			const UList<T>& negList,
			const labelUList&
		);

		//- Move constructor given the complete list and by moving addressing
		inline BiIndirectList
		(
			const UList<T>& posList,
			const UList<T>& negList,
			List<label>&&
		);


		// Member Functions

			// Access

				//- Return the number of elements in the list
		inline label size() const;

		//- Return true if the list is empty (ie, size() is zero).
		inline bool empty() const;

		inline const UList<T>& posList() const;
		inline const UList<T>& negList() const;

		//- Return the list addressing
		inline const List<label>& addressing() const;

		//- Calculate index given whether index is into posList or negList
		inline static label posIndex(const label);
		inline static label negIndex(const label);


		// Edit

			//- Reset addressing
		inline void resetAddressing(const labelUList&);


		// Member Operators

			//- Return the addressed elements as a List
		inline List<T> operator()() const;

		//- Return non-const access to an element
		inline T& operator[](const label);

		//- Return const access to an element
		inline const T& operator[](const label) const;

		//- Move assignment to List of addressed elements
		inline void operator=(List<T>&&);

		//- Assignment to UList of addressed elements
		inline void operator=(const UList<T>&);

		//- Assignment of all entries to the given value
		inline void operator=(const T&);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <BiIndirectListI.hxx>

#endif // !_BiIndirectList_Header
