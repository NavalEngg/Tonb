#pragma once
#include <contiguous.hxx>

template<class T, tnbLib::label Offset>
void tnbLib::LongList<T, Offset>::checkIndex(const label i) const
{
	if ((i < 0) || (i >= nextFree_))
	{
		FatalErrorIn
		(
			"void tnbLib::LongList<T, label>::"
			"checkIndex(const label i) const"
		) << "Index " << i << " is not in range " << 0
			<< " and " << nextFree_ << abort(FatalError);
	}
}

template<class T, tnbLib::label Offset>
void tnbLib::LongList<T, Offset>::initializeParameters()
{
	unsigned int t = sizeof(T);
	label it(0);

	while (t > 1)
	{
		t >>= 1;
		++it;
	}

	shift_ = tnbLib::max(10, Offset - it);
	mask_ = 1 << shift_;
	mask_ -= 1;
}

template<class T, tnbLib::label Offset>
inline void tnbLib::LongList<T, Offset>::allocateSize(const label s)
{
	if (s == 0)
	{
		clearOut();
		return;
	}
	else if (s < 0)
	{
		FatalErrorIn
		(
			"template<class T, tnbLib::label Offset>\n"
			"inline void tnbLib::LongList<T, Offset>::allocateSize(const label)"
		) << "Negative size requested." << abort(FatalError);
	}

	const label numblock1 = ((s - 1) >> shift_) + 1;
	const label blockSize = 1 << shift_;

	if (numblock1 < numBlocks_)
	{
		for (register label i = numblock1; i < numBlocks_; ++i)
			delete[] dataPtr_[i];
	}
	else if (numblock1 > numBlocks_)
	{
		if (numblock1 >= numAllocatedBlocks_)
		{
			do
			{
				numAllocatedBlocks_ += 64;
			} while (numblock1 > numAllocatedBlocks_);

			T** dataptr1 = new T*[numAllocatedBlocks_];
			for (register label i = 0; i < numBlocks_; ++i)
				dataptr1[i] = dataPtr_[i];

			if (dataPtr_)
				delete[] dataPtr_;
			dataPtr_ = dataptr1;
		}

		for (register label i = numBlocks_; i < numblock1; ++i)
			dataPtr_[i] = new T[blockSize];
	}

	numBlocks_ = numblock1;
	N_ = numBlocks_ * blockSize;
}

template<class T, tnbLib::label Offset>
void tnbLib::LongList<T, Offset>::clearOut()
{
	for (register label i = 0; i < numBlocks_; ++i)
		delete[] dataPtr_[i];

	if (dataPtr_)
	{
		delete[] dataPtr_;
		dataPtr_ = NULL;
	}

	N_ = 0;
	numBlocks_ = 0;
	numAllocatedBlocks_ = 0;
	nextFree_ = 0;
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct null
template<class T, tnbLib::label Offset>
inline tnbLib::LongList<T, Offset>::LongList()
	:
	N_(0),
	nextFree_(0),
	numBlocks_(0),
	numAllocatedBlocks_(0),
	shift_(),
	mask_(),
	dataPtr_(NULL)
{
	initializeParameters();
}

//- Construct given size
template<class T, tnbLib::label Offset>
inline tnbLib::LongList<T, Offset>::LongList(const label s)
	:
	N_(0),
	nextFree_(0),
	numBlocks_(0),
	numAllocatedBlocks_(0),
	shift_(),
	mask_(),
	dataPtr_(NULL)
{
	initializeParameters();
	setSize(s);
}


//- Construct given size
template<class T, tnbLib::label Offset>
inline tnbLib::LongList<T, Offset>::LongList(const label s, const T& t)
	:
	N_(0),
	nextFree_(0),
	numBlocks_(0),
	numAllocatedBlocks_(0),
	shift_(),
	mask_(),
	dataPtr_(NULL)
{
	initializeParameters();
	setSize(s);
	*this = t;
}

template<class T, tnbLib::label Offset>
inline tnbLib::LongList<T, Offset>::LongList(const LongList<T, Offset>& ol)
	:
	N_(0),
	nextFree_(0),
	numBlocks_(0),
	numAllocatedBlocks_(0),
	shift_(ol.shift_),
	mask_(ol.mask_),
	dataPtr_(NULL)
{
	*this = ol;
}

template<class T, tnbLib::label Offset>
inline tnbLib::LongList<T, Offset>::~LongList()
{
	clearOut();
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T, tnbLib::label Offset>
inline tnbLib::label tnbLib::LongList<T, Offset>::size() const
{
	return nextFree_;
}

template<class T, tnbLib::label Offset>
inline tnbLib::label tnbLib::LongList<T, Offset>::byteSize() const
{
	if (!contiguous<T>())
	{
		FatalErrorIn("LongList<T, Offset>::byteSize()")
			<< "Cannot return the binary size of a list of "
			"non-primitive elements"
			<< abort(FatalError);
	}

	return nextFree_ * sizeof(T);
}

template<class T, tnbLib::label Offset>
inline void tnbLib::LongList<T, Offset>::setSize(const label i)
{
	allocateSize(i);
	nextFree_ = i;
}

template<class T, tnbLib::label Offset>
inline void tnbLib::LongList<T, Offset>::clear()
{
	nextFree_ = 0;
}


template<class T, tnbLib::label Offset>
inline tnbLib::LongList<T, Offset>&
tnbLib::LongList<T, Offset>::shrink()
{
	setSize(nextFree_);
	return *this;
}

template<class T, tnbLib::label Offset>
inline void tnbLib::LongList<T, Offset>::transfer(LongList<T, Offset>& ol)
{
	clearOut();
	dataPtr_ = ol.dataPtr_;
	N_ = ol.N_;
	nextFree_ = ol.nextFree_;
	numBlocks_ = ol.numBlocks_;
	numAllocatedBlocks_ = ol.numAllocatedBlocks_;
	shift_ = ol.shift_;
	mask_ = ol.mask_;

	ol.dataPtr_ = NULL;
	ol.N_ = 0;
	ol.nextFree_ = 0;
	ol.numBlocks_ = 0;
	ol.numAllocatedBlocks_ = 0;
}


template<class T, tnbLib::label Offset>
inline void tnbLib::LongList<T, Offset>::append(const T& e)
{
	if (nextFree_ >= N_)
	{
		allocateSize(nextFree_ + 1);
	}

	operator[](nextFree_++) = e;
}

template<class T, tnbLib::label Offset>
inline void tnbLib::LongList<T, Offset>::appendIfNotIn(const T& e)
{
	if (!contains(e))
		append(e);
}

template<class T, tnbLib::label Offset>
inline bool tnbLib::LongList<T, Offset>::contains(const T& e) const
{
	for (register label i = 0; i < nextFree_; ++i)
		if ((*this)[i] == e)
			return true;

	return false;
}

template<class T, tnbLib::label Offset>
inline tnbLib::label tnbLib::LongList<T, Offset>::containsAtPosition
(
	const T& e
) const
{
	for (register label i = 0; i < nextFree_; ++i)
		if ((*this)[i] == e)
			return i;

	return -1;
}

template<class T, tnbLib::label Offset>
inline T tnbLib::LongList<T, Offset>::remove(const label i)
{
	if (nextFree_ == 0)
	{
		FatalErrorIn
		(
			"void tnbLib::LongList<T, Offset>::remove()"
		) << "List is empty" << abort(FatalError);
	}

	T el = operator[](i);
	operator[](i) = operator[](nextFree_ - 1);
	--nextFree_;
	return el;
}

template<class T, tnbLib::label Offset>
inline T tnbLib::LongList<T, Offset>::removeLastElement()
{
	if (nextFree_ == 0)
	{
		FatalErrorIn
		(
			"void tnbLib::LongList<T, Offset>::remove()"
		) << "List is empty" << abort(FatalError);
	}

	T lastEl = operator[](nextFree_ - 1);
	--nextFree_;
	return lastEl;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T, tnbLib::label Offset>
inline const T& tnbLib::LongList<T, Offset>::operator[](const label i) const
{
#ifdef FULLDEBUG
	checkIndex(i);
#endif

	return dataPtr_[i >> shift_][i&mask_];
}

template<class T, tnbLib::label Offset>
inline T& tnbLib::LongList<T, Offset>::operator[](const label i)
{
#ifdef FULLDEBUG
	checkIndex(i);
#endif

	return dataPtr_[i >> shift_][i&mask_];
}

template<class T, tnbLib::label Offset>
inline T& tnbLib::LongList<T, Offset>::operator()(const label i)
{
	if (i >= nextFree_)
		setSize(i + 1);

	return operator[](i);
}


template<class T, tnbLib::label Offset>
inline T& tnbLib::LongList<T, Offset>::newElmt(const label i)
{
	return operator()(i);
}

template<class T, tnbLib::label Offset>
inline void tnbLib::LongList<T, Offset>::operator=(const T& t)
{
	for (register label i = 0; i < nextFree_; ++i)
		operator[](i) = t;
}

template<class T, tnbLib::label Offset>
inline void tnbLib::LongList<T, Offset>::operator=(const LongList<T, Offset>& l)
{
	setSize(l.size());
	for (register label i = 0; i < l.nextFree_; ++i)
		operator[](i) = l[i];
}


// ************************************************************************* //