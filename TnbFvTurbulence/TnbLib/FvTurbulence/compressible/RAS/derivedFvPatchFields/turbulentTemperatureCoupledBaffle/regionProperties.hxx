#pragma once
#ifndef _regionProperties_Header
#define _regionProperties_Header

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
	regionProperties

Description
	Simple class to hold region information for coupled region simulations

SourceFiles
	regionProperties.C

\*---------------------------------------------------------------------------*/

#include <IOdictionary.hxx>
#include <tnbTime.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						   Class regionProperties Declaration
	\*---------------------------------------------------------------------------*/

	class regionProperties
		:
		public IOdictionary
	{
		// Private data

			//- List of the fluid region names
		List<word> fluidRegionNames_;

		//- List of the solid region names
		List<word> solidRegionNames_;


		// Private Member Functions

			//- Disallow default bitwise copy construct
		regionProperties(const regionProperties&);

		//- Disallow default bitwise assignment
		void operator=(const regionProperties&);


	public:

		// Constructors

			//- Construct from components
		regionProperties(const Time& runTime);


		// Destructor

		~regionProperties();


		// Member Functions

			// Access

				//- Return const reference to the list of fluid region names
		const List<word>& fluidRegionNames() const;

		//- Return const reference to the list of solid region names
		const List<word>& solidRegionNames() const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_regionProperties_Header
