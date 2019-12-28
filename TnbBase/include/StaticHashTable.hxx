#pragma once
#ifndef _StaticHashTable_Header
#define _StaticHashTable_Header

#include <label.hxx>
#include <uLabel.hxx>
#include <word.hxx>
#include <Xfer.hxx>
#include <className.hxx>

namespace tnbLib
{

	// Forward declaration of friend functions and operators

	template<class T> class List;
	template<class T, class Key, class Hash> class StaticHashTable;

	template<class T, class Key, class Hash> Istream& operator>>
		(
			Istream&,
			StaticHashTable<T, Key, Hash>&
			);

	template<class T, class Key, class Hash> Ostream& operator<<
		(
			Ostream&,
			const StaticHashTable<T, Key, Hash>&
			);


	/*---------------------------------------------------------------------------*\
						 Class StaticHashTableCore Declaration
	\*---------------------------------------------------------------------------*/

	//- Template-invariant bits for StaticHashTable
	struct StaticHashTableCore
	{
		//- Return a canonical (power-of-two) size
		static label canonicalSize(const label);

		//- Construct null
		StaticHashTableCore()
		{}

		//- Define template name and debug
		ClassName("StaticHashTable");

		//- A zero-sized end iterator
		struct iteratorEnd
		{
			//- Construct null
			iteratorEnd()
			{}
		};
	};



	/*---------------------------------------------------------------------------*\
						   Class StaticHashTable Declaration
	\*---------------------------------------------------------------------------*/

	template<class T, class Key = word, class Hash = string::hash>
	class StaticHashTable
		:
		public StaticHashTableCore
	{
		// Private data type for table entries

			//- The lookup keys, ordered per hash value
		List<List<Key> > keys_;

		//- For each key the corresponding object.
		List<List<T> > objects_;

		//- The current number of elements in table
		label nElmts_;

		//- Return a canonical (power-of-two) size
		static label canonicalSize(const label);

		//- Return the hash index of the Key within the current table size.
		//  No checks for zero-sized tables.
		label hashKeyIndex(const Key&) const;

		//- Assign a new hashed entry to a possibly already existing key
		bool set(const Key&, const T& newElmt, bool protect);


	public:


		// Forward declaration of STL iterators

		template<class TRef, class TableRef>
		class Iterator;

		typedef Iterator
			<
			T&,
			StaticHashTable<T, Key, Hash>&
			> iterator;

		typedef Iterator
			<
			const T&,
			const StaticHashTable<T, Key, Hash>&
			> const_iterator;


		// Declare friendship with the iterators

		friend class Iterator
			<
			T&,
			StaticHashTable<T, Key, Hash>&
			>;

		friend class Iterator
			<
			const T&,
			const StaticHashTable<T, Key, Hash>&
			>;


		// Constructors

			//- Construct given initial table size
		StaticHashTable(const label size = 128);

		//- Construct from Istream
		StaticHashTable(Istream&, const label size = 128);

		//- Construct as copy
		StaticHashTable(const StaticHashTable<T, Key, Hash>&);

		//- Construct by transferring the parameter contents
		StaticHashTable(const Xfer<StaticHashTable<T, Key, Hash> >&);


		//- Destructor
		~StaticHashTable();


		// Member Functions

			// Access

				//- Return number of elements in table.
		label size() const;

		//- Return true if the hash table is empty
		bool empty() const;

		//- Return true if hashed entry is found in table
		bool found(const Key& key) const;

		//- Find and return an iterator set at the hashed entry
		//  If not found iterator = end()
		iterator find(const Key& key);

		//- Find and return an const_iterator set at the hashed entry
		//  If not found iterator = end()
		const_iterator find(const Key& key) const;

		//- Return the table of contents
		List<Key> toc() const;

		//- Print information
		Ostream& printInfo(Ostream&) const;


		// Edit

			//- Insert a new hashed entry
		bool insert(const Key& key, const T& newElmt);

		//- Assign a new hashed entry, overwriting existing entries
		bool set(const Key&, const T& newElmt);

		//- Erase an hashed entry specified by given iterator
		bool erase(const iterator& it);

		//- Erase an hashed entry specified by given key if in table
		bool erase(const Key& key);

		//- Resize the hash table for efficiency
		void resize(const label newSize);

		//- Remove entries in the given hash table from this hash table
		//  Return the number of elements removed
		label erase(const StaticHashTable<T, Key, Hash>&);

		//- Clear all entries from table
		void clear();

		//- Clear the table entries and the table itself.
		//  Equivalent to clear() followed by resize(1)
		void clearStorage();

		//- Transfer the contents of the argument table into this table
		//  and annul the argument table.
		void transfer(StaticHashTable<T, Key, Hash>&);

		//- Transfer contents to the Xfer container
		Xfer<StaticHashTable<T, Key, Hash> > xfer();


		// Member Operators

			//- Find and return an hashed entry
		T& operator[](const Key&);

		//- Find and return an hashed entry
		const T& operator[](const Key&) const;

		//- Find and return an hashed entry, create it null if not present.
		T& operator()(const Key&);

		//- Assignment
		void operator=(const StaticHashTable<T, Key, Hash>&);

		//- Equality. Two hash tables are equal if all contents of first are
		//  also in second and vice versa.
		bool operator==(const StaticHashTable<T, Key, Hash>&) const;

		//- The opposite of the equality operation.
		bool operator!=(const StaticHashTable<T, Key, Hash>&) const;


		// STL type definitions

			//- Type of values the StaticHashTable contains.
		typedef T value_type;

		//- Type that can be used for storing into StaticHashTable::value_type
		//  objects.  This type is usually List::value_type&.
		typedef T& reference;

		//- Type that can be used for storing into constant
		//  StaticHashTable::value_type objects.  This type is usually const
		//  StaticHashTable::value_type&.
		typedef const T& const_reference;

		//- The type that can represent the size of a StaticHashTable.
		typedef label size_type;


		// STL iterator

			//- An STL iterator
		template<class TRef, class TableRef>
		class Iterator
		{
			friend class StaticHashTable;

#           ifndef __INTEL_COMPILER
			template<class TRef2, class TableRef2>
			friend class Iterator;
#           endif

			// Private data

				//- Reference to the StaticHashTable this is an iterator for
			TableRef hashTable_;

			//- Current hash index
			label hashIndex_;

			//- Index of current element at hashIndex
			label elemIndex_;


		public:

			// Constructors

				//- Construct from hash table, hash index and element index
			Iterator
			(
				TableRef,
				label hashIndex_,
				label elemIndex_
			);

			//- Construct from the non-const iterator
			Iterator(const iterator&);


			// Member operators

			void operator=(const iterator&);

			bool operator==(const iterator&) const;
			bool operator==(const const_iterator&) const;

			bool operator!=(const iterator&) const;
			bool operator!=(const const_iterator&) const;

			TRef operator*();
			TRef operator()();

			Iterator& operator++();
			Iterator operator++(int);

			const Key& key() const;
		};


		//- iterator set to the beginning of the StaticHashTable
		iterator begin();

		//- iterator set to beyond the end of the StaticHashTable
		const iterator& end();

		//- const_iterator set to the beginning of the StaticHashTable
		const_iterator cbegin() const;

		//- const_iterator set to beyond the end of the StaticHashTable
		const const_iterator& cend() const;

		//- const_iterator set to the beginning of the StaticHashTable
		const_iterator begin() const;

		//- const_iterator set to beyond the end of the StaticHashTable
		const const_iterator& end() const;

		// IOstream Operator

		friend Istream& operator>> <T, Key, Hash>
			(
				Istream&,
				StaticHashTable<T, Key, Hash>&
				);

		friend Ostream& operator<< <T, Key, Hash>
			(
				Ostream&,
				const StaticHashTable<T, Key, Hash>&
				);


	private:

		//- iterator returned by end()
		iterator endIter_;

		//- const_iterator returned by end()
		const_iterator endConstIter_;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
}

#include <StaticHashTableI.hxx>

#ifndef NoStaticHashTableC
#ifdef NoRepository
#   include <StaticHashTable.cxx>
#endif
#endif

#endif // !_StaticHashTable_Header
