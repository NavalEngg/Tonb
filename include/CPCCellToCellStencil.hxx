#pragma once
#ifndef _CPCCellToCellStencil_Header
#define _CPCCellToCellStencil_Header

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
	tnbLib::CPCCellToCellStencil

Description

SourceFiles
	CPCCellToCellStencil.C

\*---------------------------------------------------------------------------*/

#include <cellToCellStencil.hxx>
#include <boolList.hxx>
#include <HashSet.hxx>
#include <Map.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							   Class CPCCellToCellStencil Declaration
	\*---------------------------------------------------------------------------*/

	class CPCCellToCellStencil
		:
		public cellToCellStencil
	{
		// Private Member Functions

			//- Calculates per point the neighbour data (= pointCells)
		void calcPointBoundaryData
		(
			const boolList& isValidBFace,
			const labelList& boundaryPoints,
			Map<labelList>& neiGlobal
		) const;

		void calcCellStencil(labelListList& globalCellCells) const;


		//- Disallow default bitwise copy construct
		CPCCellToCellStencil(const CPCCellToCellStencil&);

		//- Disallow default bitwise assignment
		void operator=(const CPCCellToCellStencil&);


	public:

		// Constructors

			//- Construct from all cells and boundary faces
		explicit CPCCellToCellStencil(const polyMesh&);

	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // !_CPCCellToCellStencil_Header
