#pragma once
#ifndef _speciesTable_Header
#define _speciesTable_Header

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
	tnbLib::speciesTable

Description
	A table of species

SourceFiles
	speciesTableI.H
	speciesTable.C

\*---------------------------------------------------------------------------*/

#include <wordList.hxx>
#include <HashTable.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							   Class speciesTable Declaration
	\*---------------------------------------------------------------------------*/

	class speciesTable
		:
		public wordList
	{
		// Private data

		HashTable<label> specieIndices_;


		// Private member functions

		void setIndices();


	public:

		// Constructors

			//- Construct from list of specie names
		speciesTable(const wordList& specieNames);

		//- Construct from number of species and list of specie names
		speciesTable(const label nSpecies, const char** specieNames);

		//- Construct from Istream
		speciesTable(Istream&);


		// Member Functions

			//- Does the table contain the given specie
		inline bool contains(const word& specieName) const;


		// Member Operators

			//- Return a specie's name
		inline const word& operator[](const label) const;

		//- Lookup a specie's name and return its index
		inline label operator[](const word&) const;


		// Istream operators

		friend Istream& operator>>(Istream&, speciesTable&);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <speciesTableI.hxx>

#endif // !_speciesTable_Header
