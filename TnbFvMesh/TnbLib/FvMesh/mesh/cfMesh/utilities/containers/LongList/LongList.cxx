#include <LongList.hxx>

#include <Ostream.hxx>
#include <token.hxx>

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class T, tnbLib::label Offset>
void tnbLib::LongList<T, Offset>::writeEntry(Ostream& os) const
{
	if
		(
			size() &&
			token::compound::isCompound
			(
				"LongList<" + word(pTraits<T>::typeName) + '>'
			)
			)
	{
		os << word("LongList<" + word(pTraits<T>::typeName) + '>') << " ";
	}

	os << *this;
}

template<class T, tnbLib::label Offset>
void tnbLib::LongList<T, Offset>::writeEntry
(
	const word& keyword,
	Ostream& os
) const
{
	os.writeKeyword(keyword);
	writeEntry(os);
	os << token::END_STATEMENT << endl;
}

template<class T, tnbLib::label Offset>
tnbLib::Ostream& tnbLib::operator<<
(
	tnbLib::Ostream& os,
	const tnbLib::LongList<T, Offset>& DL
	)
{
	if ((os.format() == IOstream::ASCII) || !contiguous<T>())
	{
		if (DL.size() < 15)
		{
			// Write size of list and start contents delimiter
			os << DL.size() << token::BEGIN_LIST;

			// Write list contents
			forAll(DL, i)
			{
				if (i != 0) os << token::SPACE;
				os << DL[i];
			}

			// Write end of contents delimiter
			os << token::END_LIST;
		}
		else
		{
			// Write size of list and start contents delimiter
			os << nl << DL.size() << nl << token::BEGIN_LIST;

			// Write list contents
			forAll(DL, i)
			{
				os << nl << DL[i];
			}

			// Write end of contents delimiter
			os << nl << token::END_LIST << nl;
		}
	}
	else
	{
		os << nl << DL.nextFree_ << nl;
		if (DL.nextFree_)
		{
			const label blockSize = 1 << DL.shift_;

			label currBlock(0);
			label currPos(0);

			while (currPos < DL.nextFree_)
			{
				const label bs =
					tnbLib::min(DL.nextFree_ - currPos, blockSize);

				os.write
				(
					reinterpret_cast<const char*>(DL.dataPtr_[currBlock]),
					bs * sizeof(T)
				);

				currPos += bs;
				++currBlock;
			}
		}
	}

	// Check state of IOstream
	os.check("Ostream& operator<<(Ostream&, const LongList&)");

	return os;
}


template<class T, tnbLib::label Offset>
tnbLib::Istream& tnbLib::operator>>
(
	tnbLib::Istream& is,
	tnbLib::LongList<T, Offset>& DL
	)
{
	// Anull list
	DL.setSize(0);

	is.fatalCheck("operator>>(Istream&, LongList<T, Offset>&)");

	token firstToken(is);

	is.fatalCheck
	(
		"operator>>(Istream&, LongList<T, Offset>&) : reading first token"
	);

	if (firstToken.isLabel())
	{
		const label size = firstToken.labelToken();

		// Set list length to that read
		DL.setSize(size);

		// Read list contents depending on data format
		if ((is.format() == IOstream::ASCII) || !contiguous<T>())
		{
			// Read beginning of contents
			char listDelimiter = is.readBeginList("List");

			if (size == 0)
			{
				if (listDelimiter != token::BEGIN_LIST)
				{
					WarningIn
					(
						"template<class T, tnbLib::label Offset>"

						"tnbLib::Istream& tnbLib::operator>>"
						"("
						"tnbLib::Istream& ,"
						"tnbLib::LongList<T, Offset>& DL"
						")"
					) << "Missing ( after 0" << endl;

					return is;
				}

				listDelimiter = is.readEndList("List");
				if (listDelimiter != token::END_LIST)
				{
					WarningIn
					(
						"template<class T, tnbLib::label Offset>"

						"tnbLib::Istream& tnbLib::operator>>"
						"("
						"tnbLib::Istream& ,"
						"tnbLib::LongList<T, Offset>& DL"
						")"
					) << "Missing ) after 0(" << endl;
				}

				return is;
			}

			if (listDelimiter == token::BEGIN_LIST)
			{
				for (register label i = 0; i < size; ++i)
				{
					is >> DL[i];

					is.fatalCheck
					(
						"operator>>(Istream&, List<T>&) : reading entry"
					);
				}
			}
			else
			{
				T element;
				is >> element;

				is.fatalCheck
				(
					"operator>>(Istream&, List<T>&) : "
					"reading the single entry"
				);

				for (register label i = 0; i < size; ++i)
				{
					DL[i] = element;
				}
			}

			// Read end of contents
			is.readEndList("List");
		}
		else
		{
			const label blockSize = (1 << DL.shift_);

			label currBlock(0);
			label currPos(0);

			while (currPos < size)
			{
				const label bs = tnbLib::min(size - currPos, blockSize);

				is.read
				(
					reinterpret_cast<char*>(DL.dataPtr_[currBlock]),
					bs * sizeof(T)
				);

				currPos += bs;
				++currBlock;
			}

			is.fatalCheck
			(
				"operator>>(Istream&, LongList<T, Offset>&)"
				": reading the binary block"
			);
		}
	}
	else
	{
		FatalIOErrorIn("operator>>(Istream&, LongList<T, Offset>&)", is)
			<< "incorrect first token, expected <int>, found "
			<< firstToken.info()
			<< exit(FatalIOError);
	}

	return is;
}

template<class T, tnbLib::label Offset>
void tnbLib::LongList<T, Offset>::appendFromStream(Istream& is)
{
	is.fatalCheck("appendFromStream(Istream& is)");

	token firstToken(is);

	is.fatalCheck
	(
		"appendFromStream(Istream& is) : reading first token"
	);

	if (firstToken.isLabel())
	{
		const label size = firstToken.labelToken();

		if (size == 0)
		{
			Pout << "Appending empty stream" << endl;
			return;
		}

		label origSize(this->size());

		// Set list length to that read
		setSize(origSize + size);

		// Read list contents depending on data format
		if ((is.format() == IOstream::ASCII) || !contiguous<T>())
		{
			// Read beginning of contents
			char listDelimiter = is.readBeginList("List");

			if (listDelimiter == token::BEGIN_LIST)
			{
				for (register label i = 0; i < size; ++i)
				{
					is >> this->operator[](origSize);
					++origSize;

					is.fatalCheck
					(
						"appendFromStream(Istream& is) : reading entry"
					);
				}
			}
			else
			{
				T element;
				is >> element;

				is.fatalCheck
				(
					"appendFromStream(Istream& is) : "
					"reading the single entry"
				);

				for (register label i = 0; i < size; ++i)
				{
					this->operator[](origSize) = element;
					++origSize;
				}
			}

			// Read end of contents
			is.readEndList("List");
		}
		else
		{
			List<T> buf(size);
			is.read(reinterpret_cast<char*>(buf.begin()), size * sizeof(T));

			forAll(buf, i)
				this->operator[](origSize++) = buf[i];

			/*const label blockSize = 1<<shift_;

			Info << "nextFree_ " << nextFree_ << endl;

			//- append elements by reading binary block
			while( origSize < nextFree_ )
			{
				const label currBlock = origSize >> shift_;
				const label currPos = origSize & mask_;

				Info << "Orig size " << origSize
					<< nl << "currBlock " << currBlock
					<< nl << "currPos " << currPos << endl;

				T* data = &dataPtr_[currBlock][currPos];

				label bs = tnbLib::min(nextFree_-origSize, blockSize);
				bs = tnbLib::min(blockSize - currPos, bs);

				Info << "bs " << bs << endl;

				is.read(reinterpret_cast<char*>(data), bs * sizeof(T));
				origSize += bs;
			} */

			is.fatalCheck
			(
				"appendFromStream(Istream& is)"
				": reading the binary block"
			);
		}
	}
	else
	{
		FatalIOErrorIn("appendFromStream(Istream& is)", is)
			<< "incorrect first token, expected <int>, found "
			<< firstToken.info()
			<< exit(FatalIOError);
	}
}


// ************************************************************************* //