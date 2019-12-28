#pragma once
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
	Foam::regIOobject

Description
	regIOobject is an abstract class derived from IOobject to handle
	automatic object registration with the objectRegistry.

SourceFiles
	regIOobject.C
	regIOobjectRead.C
	regIOobjectWrite.C

\*---------------------------------------------------------------------------*/
#ifndef _regIOobject_Header
#define _regIOobject_Header

#include <IOobject.hxx>
#include <typeInfo.hxx>
#include <OSspecific.hxx>
#include <optimisationSwitch.hxx>

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
						 Class regIOobject Declaration
\*---------------------------------------------------------------------------*/

	class regIOobject
		:
		public IOobject
	{

	private:

		// Private data

			//- Is this object registered with the registry
		bool registered_;

		//- Is this object owned by the registry
		bool ownedByRegistry_;

		//- Time of last modification
		mutable time_t lastModified_;

		//- eventNo of last update
		label eventNo_;

		//- Istream for reading
		Istream* isPtr_;


		// Private member functions

			//- Return Istream
		Istream& readStream();

		//- Dissallow assignment
		void operator=(const regIOobject&);


	public:

		// Static data

			//- Runtime type information
		TypeName("regIOobject");

		//- File modification skew
		static const debug::optimisationSwitch fileModificationSkew;


		// Constructors

			//- Construct from IOobject. Optional flag for if IOobject is the
			//  top level regIOobject.
		regIOobject(const IOobject&, const bool isTime = false);

		//- Construct as copy
		regIOobject(const regIOobject&);

		//- Construct as copy, and transferring registry registration to copy
		//  if registerCopy is true
		regIOobject(const regIOobject&, bool registerCopy);


		// Destructor

		virtual ~regIOobject();


		// Member functions

			// Registration

				//- Add object to registry
		bool checkIn();

		//- Remove object from registry
		bool checkOut();

		//- Is this object owned by the registry?
		inline bool ownedByRegistry() const;

		//- Transfer ownership of this object to its registry
		inline void store();

		//- Transfer ownership of the given object pointer to its registry
		//  and return reference to object.
		template<class Type>
		inline static Type& store(Type*);

		//- Transfer ownership of the given object pointer to its registry
		//  and return reference to object.
		template<class Type>
		inline static Type& store(autoPtr<Type>&);

		//- Release ownership of this object from its registry
		inline void release();


		// Dependency checking

			//- Event number at last update.
		inline label eventNo() const;

		//- Event number at last update.
		inline label& eventNo();

		//- Am I uptodate with respect to other regIOobjects
		bool upToDate(const word&) const;
		bool upToDate(const word&, const word&) const;
		bool upToDate(const word&, const word&, const word&) const;
		bool upToDate
		(
			const word&,
			const word&,
			const word&,
			const word&
		) const;

		//- Flag me as up to date
		void setUpToDate();


		// Edit

			//- Rename
		virtual void rename(const word& newName);


		// Reading

			//- Return Istream and check object type against that given
		Istream& readStream(const word&);

		//- Close Istream
		void close();

		//- Virtual readData function.
		//  Must be defined in derived types for which
		//  re-reading is required
		virtual bool readData(Istream&);

		//- Read object
		virtual bool read();

		//- Return true if the object's file has been modified
		virtual bool modified() const;

		//- Read object if modified
		virtual bool readIfModified();


		// Writing

			//- Pure virtual writaData function.
			//  Must be defined in derived types
		virtual bool writeData(Ostream&) const = 0;

		//- Write using given format, version and compression
		virtual bool writeObject
		(
			IOstream::streamFormat,
			IOstream::versionNumber,
			IOstream::compressionType
		) const;

		//- Write using setting from DB
		virtual bool write() const;


		// Member operators

		void operator=(const IOobject&);
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
}

#include <regIOobjectI.hxx>

#endif // !_regIOobject_Header
