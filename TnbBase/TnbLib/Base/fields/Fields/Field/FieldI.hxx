#pragma once
#include <FieldMapper.hxx>
#include <FieldM.hxx>
#include <dictionary.hxx>
#include <contiguous.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// * * * * * * * * * * * * * * * Static Members  * * * * * * * * * * * * * * //

	template<class Type>
	const char* const Field<Type>::typeName("Field");

	template<class Type>
	const Field<Type> Field<Type>::zero;


	// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

	template<class Type>
	Field<Type>::Field()
		:
		List<Type>()
	{}


	template<class Type>
	Field<Type>::Field(const label size)
		:
		List<Type>(size)
	{}


	template<class Type>
	Field<Type>::Field(const label size, const Type& t)
		:
		List<Type>(size, t)
	{}


	template<class Type>
	Field<Type>::Field
	(
		const UList<Type>& mapF,
		const unallocLabelList& mapAddressing
	)
		:
		List<Type>(mapAddressing.size())
	{
		map(mapF, mapAddressing);
	}

	template<class Type>
	Field<Type>::Field
	(
		const tmp<Field<Type> >& tmapF,
		const unallocLabelList& mapAddressing
	)
		:
		List<Type>(mapAddressing.size())
	{
		map(tmapF, mapAddressing);
	}


	template<class Type>
	Field<Type>::Field
	(
		const UList<Type>& mapF,
		const labelListList& mapAddressing,
		const scalarListList& mapWeights
	)
		:
		List<Type>(mapAddressing.size())
	{
		map(mapF, mapAddressing, mapWeights);
	}

	template<class Type>
	Field<Type>::Field
	(
		const tmp<Field<Type> >& tmapF,
		const labelListList& mapAddressing,
		const scalarListList& mapWeights
	)
		:
		List<Type>(mapAddressing.size())
	{
		map(tmapF, mapAddressing, mapWeights);
	}


	template<class Type>
	Field<Type>::Field
	(
		const UList<Type>& mapF,
		const FieldMapper& mapper
	)
		:
		List<Type>(mapper.size())
	{
		map(mapF, mapper);
	}

	template<class Type>
	Field<Type>::Field
	(
		const tmp<Field<Type> >& tmapF,
		const FieldMapper& mapper
	)
		:
		List<Type>(mapper.size())
	{
		map(tmapF, mapper);
	}


	template<class Type>
	Field<Type>::Field(const Field<Type>& f)
		:
		refCount(),
		List<Type>(f)
	{}


	template<class Type>
	Field<Type>::Field(Field<Type>& f, bool reUse)
		:
		List<Type>(f, reUse)
	{}


	template<class Type>
	Field<Type>::Field(const Xfer<List<Type> >& f)
		:
		List<Type>(f)
	{}


	template<class Type>
	Field<Type>::Field(const Xfer<Field<Type> >& f)
		:
		List<Type>(f)
	{}


	template<class Type>
	Field<Type>::Field(const typename Field<Type>::subField& sf)
		:
		List<Type>(sf)
	{}


	template<class Type>
	Field<Type>::Field(const UList<Type>& list)
		:
		List<Type>(list)
	{}


	// Construct as copy of tmp<Field>
#ifdef ConstructFromTmp
	template<class Type>
	Field<Type>::Field(const tmp<Field<Type> >& tf)
		:
		List<Type>(const_cast<Field<Type>&>(tf()), tf.isTmp())
	{
		const_cast<Field<Type>&>(tf()).resetRefCount();
	}
#endif


	template<class Type>
	Field<Type>::Field(Istream& is)
		:
		List<Type>(is)
	{}


	template<class Type>
	Field<Type>::Field
	(
		const word& keyword,
		const dictionary& dict,
		const label s
	)
	{
		if (s)
		{
			ITstream& is = dict.lookup(keyword);

			// Read first token
			token firstToken(is);

			if (firstToken.isWord())
			{
				if (firstToken.wordToken() == "uniform")
				{
					this->setSize(s);
					operator=(pTraits<Type>(is));
				}
				else if (firstToken.wordToken() == "nonuniform")
				{
					is >> static_cast<List<Type>&>(*this);
					if (this->size() != s)
					{
						FatalIOErrorIn
						(
							"Field<Type>::Field"
							"(const word& keyword, const dictionary&, const label)",
							dict
						) << "size " << this->size()
							<< " is not equal to the given value of " << s
							<< exit(FatalIOError);
					}
				}
				else
				{
					FatalIOErrorIn
					(
						"Field<Type>::Field"
						"(const word& keyword, const dictionary&, const label)",
						dict
					) << "expected keyword 'uniform' or 'nonuniform', found "
						<< firstToken.wordToken()
						<< exit(FatalIOError);
				}
			}
			else
			{
				if (is.version() == 2.0)
				{
					IOWarningIn
					(
						"Field<Type>::Field"
						"(const word& keyword, const dictionary&, const label)",
						dict
					) << "expected keyword 'uniform' or 'nonuniform', "
						"assuming deprecated Field format from "
						"tnbLib version 2.0." << endl;

					this->setSize(s);

					is.putBack(firstToken);
					operator=(pTraits<Type>(is));
				}
				else
				{
					FatalIOErrorIn
					(
						"Field<Type>::Field"
						"(const word& keyword, const dictionary&, const label)",
						dict
					) << "expected keyword 'uniform' or 'nonuniform', found "
						<< firstToken.info()
						<< exit(FatalIOError);
				}
			}
		}
	}


	template<class Type>
	tmp<Field<Type> > Field<Type>::clone() const
	{
		return tmp<Field<Type> >(new Field<Type>(*this));
	}


	// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

	template<class Type>
	void Field<Type>::map
	(
		const UList<Type>& mapF,
		const unallocLabelList& mapAddressing
	)
	{
		Field<Type>& f = *this;

		if (f.size() != mapAddressing.size())
		{
			f.setSize(mapAddressing.size());
		}

		if (mapF.size() > 0)
		{
			forAll(f, i)
			{
				label mapI = mapAddressing[i];

				if (mapI >= 0)
				{
					f[i] = mapF[mapI];
				}
			}
		}
	}


	template<class Type>
	void Field<Type>::map
	(
		const tmp<Field<Type> >& tmapF,
		const unallocLabelList& mapAddressing
	)
	{
		map(tmapF(), mapAddressing);
		tmapF.clear();
	}


	template<class Type>
	void Field<Type>::map
	(
		const UList<Type>& mapF,
		const labelListList& mapAddressing,
		const scalarListList& mapWeights
	)
	{
		Field<Type>& f = *this;

		if (f.size() != mapAddressing.size())
		{
			f.setSize(mapAddressing.size());
		}

		if (mapWeights.size() != mapAddressing.size())
		{
			FatalErrorIn
			(
				"void Field<Type>::map\n"
				"(\n"
				"    const UList<Type>& mapF,\n"
				"    const labelListList& mapAddressing,\n"
				"    const scalarListList& mapWeights\n"
				")"
			) << "Weights and addressing map have different sizes.  Weights size: "
				<< mapWeights.size() << " map size: " << mapAddressing.size()
				<< abort(FatalError);
		}

		forAll(f, i)
		{
			const labelList& localAddrs = mapAddressing[i];
			const scalarList& localWeights = mapWeights[i];

			f[i] = pTraits<Type>::zero;

			forAll(localAddrs, j)
			{
				f[i] += localWeights[j] * mapF[localAddrs[j]];
			}
		}
	}

	template<class Type>
	void Field<Type>::map
	(
		const tmp<Field<Type> >& tmapF,
		const labelListList& mapAddressing,
		const scalarListList& mapWeights
	)
	{
		map(tmapF(), mapAddressing, mapWeights);
		tmapF.clear();
	}


	template<class Type>
	void Field<Type>::map
	(
		const UList<Type>& mapF,
		const FieldMapper& mapper
	)
	{
		if (mapper.sizeBeforeMapping() > 0)
		{
			if
				(
					mapper.direct()
					&& mapper.directAddressing().size()
					)
			{
				map(mapF, mapper.directAddressing());
			}
			else if (!mapper.direct() && mapper.addressing().size())
			{
				map(mapF, mapper.addressing(), mapper.weights());
			}
		}
		else
		{
			// No data to map from, resize only
			this->setSize(mapper.size());

			// In order to avoid use of uninitialised memory, the field
			// will artificially be set to zero.
			// HJ, 22/Aug/2007
			this->operator=(pTraits<Type>::zero);
		}
	}

	template<class Type>
	void Field<Type>::map
	(
		const tmp<Field<Type> >& tmapF,
		const FieldMapper& mapper
	)
	{
		map(tmapF(), mapper);
		tmapF.clear();
	}


	template<class Type>
	void Field<Type>::autoMap
	(
		const FieldMapper& mapper
	)
	{
		if
			(
			(
				mapper.direct()
				&& mapper.directAddressing().size()
				)
				|| (!mapper.direct() && mapper.addressing().size())
				)
		{
			Field<Type> fCpy(*this);
			map(fCpy, mapper);
		}
		else
		{
			this->setSize(mapper.size());
		}
	}


	template<class Type>
	void Field<Type>::rmap
	(
		const UList<Type>& mapF,
		const unallocLabelList& mapAddressing
	)
	{
		Field<Type>& f = *this;

		forAll(mapF, i)
		{
			label mapI = mapAddressing[i];

			if (mapI >= 0)
			{
				f[mapI] = mapF[i];
			}
		}
	}

	template<class Type>
	void Field<Type>::rmap
	(
		const tmp<Field<Type> >& tmapF,
		const unallocLabelList& mapAddressing
	)
	{
		rmap(tmapF(), mapAddressing);
		tmapF.clear();
	}


	template<class Type>
	void Field<Type>::rmap
	(
		const UList<Type>& mapF,
		const unallocLabelList& mapAddressing,
		const scalarList& mapWeights
	)
	{
		Field<Type>& f = *this;

		f = pTraits<Type>::zero;

		forAll(mapF, i)
		{
			f[mapAddressing[i]] += mapF[i] * mapWeights[i];
		}
	}

	template<class Type>
	void Field<Type>::rmap
	(
		const tmp<Field<Type> >& tmapF,
		const unallocLabelList& mapAddressing,
		const scalarList& mapWeights
	)
	{
		rmap(tmapF(), mapAddressing, mapWeights);
		tmapF.clear();
	}


	template<class Type>
	void Field<Type>::negate()
	{
		TFOR_ALL_F_OP_OP_F(Type, *this, =, -, Type, *this)
	}


	template<class Type>
	tmp<Field<typename Field<Type>::cmptType> > Field<Type>::component
	(
		const direction d
	) const
	{
		tmp<Field<cmptType> > Component(new Field<cmptType>(this->size()));
		::tnbLib::component(Component(), *this, d);
		return Component;
	}


	template<class Type>
	void Field<Type>::replace
	(
		const direction d,
		const UList<cmptType>& sf
	)
	{
		TFOR_ALL_F_OP_FUNC_S_F(Type, *this, ., replace, const direction, d,
			cmptType, sf)
	}


	template<class Type>
	void Field<Type>::replace
	(
		const direction d,
		const tmp<Field<cmptType> >& tsf
	)
	{
		replace(d, tsf());
		tsf.clear();
	}


	template<class Type>
	void Field<Type>::replace
	(
		const direction d,
		const cmptType& c
	)
	{
		TFOR_ALL_F_OP_FUNC_S_S(Type, *this, ., replace, const direction, d,
			cmptType, c)
	}


	template<class Type>
	tmp<Field<Type> > Field<Type>::T() const
	{
		tmp<Field<Type> > transpose(new Field<Type>(this->size()));
		::tnbLib::T(transpose(), *this);
		return transpose;
	}


	template<class Type>
	void Field<Type>::writeEntry(const word& keyword, Ostream& os) const
	{
		os.writeKeyword(keyword);

		bool uniform = false;

		if (this->size() && contiguous<Type>())
		{
			uniform = true;

			forAll(*this, i)
			{
				if (this->operator[](i) != this->operator[](0))
				{
					uniform = false;
					break;
				}
			}
		}

		if (uniform)
		{
			os << "uniform " << this->operator[](0) << token::END_STATEMENT;
		}
		else
		{
			os << "nonuniform ";
			List<Type>::writeEntry(os);
			os << token::END_STATEMENT;
		}

		os << endl;
	}


	// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

	template<class Type>
	void Field<Type>::operator=(const Field<Type>& rhs)
	{
		if (this == &rhs)
		{
			FatalErrorIn("Field<Type>::operator=(const Field<Type>&)")
				<< "attempted assignment to self"
				<< abort(FatalError);
		}

		List<Type>::operator=(rhs);
	}


	template<class Type>
	void Field<Type>::operator=(const SubField<Type>& rhs)
	{
		List<Type>::operator=(rhs);
	}


	template<class Type>
	void Field<Type>::operator=(const UList<Type>& rhs)
	{
		List<Type>::operator=(rhs);
	}


	template<class Type>
	void Field<Type>::operator=(const tmp<Field>& rhs)
	{
		if (this == &(rhs()))
		{
			FatalErrorIn("Field<Type>::operator=(const tmp<Field>&)")
				<< "attempted assignment to self"
				<< abort(FatalError);
		}

		// This is dodgy stuff, don't try it at home.
		Field* fieldPtr = rhs.ptr();
		List<Type>::transfer(*fieldPtr);
		delete fieldPtr;
	}


	template<class Type>
	void Field<Type>::operator=(const Type& t)
	{
		List<Type>::operator=(t);
	}


	template<class Type>
	template<class Form, class Cmpt, int nCmpt>
	void Field<Type>::operator=(const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		TFOR_ALL_F_OP_S(Type, *this, =, (VectorSpace<Form, Cmpt, nCmpt>), vs)
	}

	template <class Type>
	void Field<Type>::operator +=(const UList<Type>& f)
	{
		TFOR_ALL_F_OP_F(Type, *this, +=, Type, f)
	}

	template <class Type>
	void Field<Type>::operator +=(const tmp<Field<Type>>& tf)
	{
		operator +=(tf());
		tf.clear();
	}

	template <class Type>
	void Field<Type>::operator +=(const Type& t)
	{
		TFOR_ALL_F_OP_S(Type, *this, +=, Type, t)
	}
		
	template <class Type>
	void Field<Type>::operator -=(const UList<Type>& f)
	{
		TFOR_ALL_F_OP_F(Type, *this, -=, Type, f)
	}

	template <class Type>
	void Field<Type>::operator -=(const tmp<Field<Type>>& tf)
	{
		operator -=(tf());
		tf.clear();
	}

	template <class Type>
	void Field<Type>::operator -=(const Type& t)
	{
		TFOR_ALL_F_OP_S(Type, *this, -=, Type, t)
	}
		
	template <class Type>
	void Field<Type>::operator *=(const UList<scalar>& f)
	{
		TFOR_ALL_F_OP_F(Type, *this, *=, scalar, f)
	}

	template <class Type>
	void Field<Type>::operator *=(const tmp<Field<scalar>>& tf)
	{
		operator *=(tf());
		tf.clear();
	}

	template <class Type>
	void Field<Type>::operator *=(const scalar& t)
	{
		TFOR_ALL_F_OP_S(Type, *this, *=, scalar, t)
	}
		
	template <class Type>
	void Field<Type>::operator /=(const UList<scalar>& f)
	{
		TFOR_ALL_F_OP_F(Type, *this, /=, scalar, f)
	}

	template <class Type>
	void Field<Type>::operator /=(const tmp<Field<scalar>>& tf)
	{
		operator /=(tf());
		tf.clear();
	}

	template <class Type>
	void Field<Type>::operator /=(const scalar& t)
	{
		TFOR_ALL_F_OP_S(Type, *this, /=, scalar, t)
	}


	// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

		
	template<class Type>
	Ostream& operator<<(Ostream& os, const Field<Type>& f)
	{
		os << static_cast<const List<Type>&>(f);
		return os;
	}


	template<class Type>
	Ostream& operator<<(Ostream& os, const tmp<Field<Type> >& tf)
	{
		os << tf();
		tf.clear();
		return os;
	}


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //



// ************************************************************************* //
