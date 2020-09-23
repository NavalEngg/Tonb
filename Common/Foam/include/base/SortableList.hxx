#pragma once
#ifndef _SortableList_Header
#define _SortableList_Header

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
	tnbLib::SortableList

Description
	A list that is sorted upon construction or when explicitly requested
	with the sort() method.

	Uses the tnbLib::stableSort() algorithm.

SourceFiles
	SortableList.C

\*---------------------------------------------------------------------------*/

#include <labelList.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							Class SortableList Declaration
	\*---------------------------------------------------------------------------*/

	template<class T>
	class SortableList
		:
		public List<T>
	{
		// Private Data

			//- Original indices
		labelList indices_;


	public:

		// Constructors

			//- Null constructor, sort later (eg, after assignment or transfer)
		SortableList();

		//- Construct from UList, sorting immediately
		explicit SortableList(const UList<T>&);

		//- Move constructor transferring List, sorting immediately
		explicit SortableList(List<T>&&);

		//- Construct given size. Sort later on
		//  The indices remain empty until the list is sorted
		explicit SortableList(const label size);

		//- Construct given size and initial value. Sort later on
		//  The indices remain empty until the list is sorted
		SortableList(const label size, const T&);

		//- Copy constructor
		SortableList(const SortableList<T>&);

		//- Move constructor
		SortableList(SortableList<T>&&);

		//- Construct from an initializer list, sorting immediately
		SortableList(std::initializer_list<T>);


		// Member Functions

			//- Return the list of sorted indices. Updated every sort
		const labelList& indices() const
		{
			return indices_;
		}

		//- Return non-const access to the sorted indices. Updated every sort
		labelList& indices()
		{
			return indices_;
		}

		//- Clear the list and the indices
		void clear();

		//- Clear the indices and return a reference to the underlying List
		List<T>& shrink();

		//- (stable) sort the list (if changed after construction time)
		//  also resizes the indices as required
		void sort();

		//- Reverse (stable) sort the list
		void reverseSort();


		// Member Operators

			//- Assignment of all entries to the given value
		inline void operator=(const T&);

		//- Assignment to UList operator. Takes linear time
		inline void operator=(const UList<T>&);

		//- Assignment operator. Takes linear time
		inline void operator=(const SortableList<T>&);

		//- Move assignment operator
		inline void operator=(SortableList<T>&&);

		//- Assignment to an initializer list
		void operator=(std::initializer_list<T>);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <SortableListI.hxx>

//#ifdef NoRepository
//#include <SortableList.cxx>
//#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_SortableList_Header
