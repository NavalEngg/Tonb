#pragma once
#include <PstreamReduceOps.hxx>
#include <FieldReuseFunctions.hxx>
#include <FieldM.hxx>
#include <products.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/* * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * */

	template<class Type>
	void component
	(
		Field<typename Field<Type>::cmptType>& res,
		const UList<Type>& f,
		const direction d
	)
	{
		typedef typename Field<Type>::cmptType cmptType;
		TFOR_ALL_F_OP_F_FUNC_S
		(
			cmptType, res, =, Type, f, .component, const direction, d
		)
	}


	template<class Type>
	void T(Field<Type>& res, const UList<Type>& f)
	{
		TFOR_ALL_F_OP_F_FUNC(Type, res, =, Type, f, T)
	}


	template<class Type, int r>
	void pow
	(
		Field<typename powProduct<Type, r>::type>& res,
		const UList<Type>& vf
	)
	{
		typedef typename powProduct<Type, r>::type powProductType;
		TFOR_ALL_F_OP_FUNC_F_S
		(
			powProductType, res, =, pow, Type, vf, powProductType,
			pTraits<powProductType>::zero
		)
	}

	template<class Type, int r>
	tmp<Field<typename powProduct<Type, r>::type>>
		pow
		(
			const UList<Type>& f,
			typename powProduct<Type, r>::type
		)
	{
		typedef typename powProduct<Type, r>::type powProductType;
		tmp<Field<powProductType>> tRes
		(
			new Field<powProductType>(f.size())
		);
		pow<Type, r>(tRes(), f);
		return tRes;
	}

	template<class Type, int r>
	tmp<Field<typename powProduct<Type, r>::type>>
		pow
		(
			const tmp<Field<Type>>& tf,
			typename powProduct<Type, r>::type
		)
	{
		typedef typename powProduct<Type, r>::type powProductType;
		tmp<Field<powProductType>> tRes = reuseTmp<powProductType, Type>::New(tf);
		pow<Type, r>(tRes(), tf());
		reuseTmp<powProductType, Type>::clear(tf);
		return tRes;
	}


	template<class Type>
	void sqr
	(
		Field<typename outerProduct<Type, Type>::type>& res,
		const UList<Type>& vf
	)
	{
		typedef typename outerProduct<Type, Type>::type outerProductType;
		TFOR_ALL_F_OP_FUNC_F(outerProductType, res, =, sqr, Type, vf)
	}

	template<class Type>
	tmp<Field<typename outerProduct<Type, Type>::type>>
		sqr(const UList<Type>& f)
	{
		typedef typename outerProduct<Type, Type>::type outerProductType;
		tmp<Field<outerProductType>> tRes
		(
			new Field<outerProductType>(f.size())
		);
		sqr(tRes(), f);
		return tRes;
	}

	template<class Type>
	tmp<Field<typename outerProduct<Type, Type>::type>>
		sqr(const tmp<Field<Type>>& tf)
	{
		typedef typename outerProduct<Type, Type>::type outerProductType;
		tmp<Field<outerProductType>> tRes =
			reuseTmp<outerProductType, Type>::New(tf);
		sqr(tRes(), tf());
		reuseTmp<outerProductType, Type>::clear(tf);
		return tRes;
	}


	template<class Type>
	void magSqr(Field<scalar>& res, const UList<Type>& f)
	{
		TFOR_ALL_F_OP_FUNC_F(scalar, res, =, magSqr, Type, f)
	}

	template<class Type>
	tmp<Field<scalar>> magSqr(const UList<Type>& f)
	{
		tmp<Field<scalar>> tRes(new Field<scalar>(f.size()));
		magSqr(tRes(), f);
		return tRes;
	}

	template<class Type>
	tmp<Field<scalar>> magSqr(const tmp<Field<Type>>& tf)
	{
		tmp<Field<scalar>> tRes = reuseTmp<scalar, Type>::New(tf);
		magSqr(tRes(), tf());
		reuseTmp<scalar, Type>::clear(tf);
		return tRes;
	}


	template<class Type>
	void mag(Field<scalar>& res, const UList<Type>& f)
	{
		TFOR_ALL_F_OP_FUNC_F(scalar, res, =, mag, Type, f)
	}

	template<class Type>
	tmp<Field<scalar>> mag(const UList<Type>& f)
	{
		tmp<Field<scalar>> tRes(new Field<scalar>(f.size()));
		mag(tRes(), f);
		return tRes;
	}

	template<class Type>
	tmp<Field<scalar>> mag(const tmp<Field<Type>>& tf)
	{
		tmp<Field<scalar>> tRes = reuseTmp<scalar, Type>::New(tf);
		mag(tRes(), tf());
		reuseTmp<scalar, Type>::clear(tf);
		return tRes;
	}


	template<class Type>
	void cmptMax(Field<typename Field<Type>::cmptType>& res, const UList<Type>& f)
	{
		typedef typename Field<Type>::cmptType cmptType;
		TFOR_ALL_F_OP_FUNC_F(cmptType, res, =, cmptMax, Type, f)
	}

	template<class Type>
	tmp<Field<typename Field<Type>::cmptType>> cmptMax(const UList<Type>& f)
	{
		typedef typename Field<Type>::cmptType cmptType;
		tmp<Field<cmptType>> tRes(new Field<cmptType>(f.size()));
		cmptMax(tRes(), f);
		return tRes;
	}

	template<class Type>
	tmp<Field<typename Field<Type>::cmptType>> cmptMax(const tmp<Field<Type>>& tf)
	{
		typedef typename Field<Type>::cmptType cmptType;
		tmp<Field<cmptType>> tRes = reuseTmp<cmptType, Type>::New(tf);
		cmptMax(tRes(), tf());
		reuseTmp<cmptType, Type>::clear(tf);
		return tRes;
	}


	template<class Type>
	void cmptMin(Field<typename Field<Type>::cmptType>& res, const UList<Type>& f)
	{
		typedef typename Field<Type>::cmptType cmptType;
		TFOR_ALL_F_OP_FUNC_F(cmptType, res, =, cmptMin, Type, f)
	}

	template<class Type>
	tmp<Field<typename Field<Type>::cmptType>> cmptMin(const UList<Type>& f)
	{
		typedef typename Field<Type>::cmptType cmptType;
		tmp<Field<cmptType>> tRes(new Field<cmptType>(f.size()));
		cmptMin(tRes(), f);
		return tRes;
	}

	template<class Type>
	tmp<Field<typename Field<Type>::cmptType>> cmptMin(const tmp<Field<Type>>& tf)
	{
		typedef typename Field<Type>::cmptType cmptType;
		tmp<Field<cmptType>> tRes = reuseTmp<cmptType, Type>::New(tf);
		cmptMin(tRes(), tf());
		reuseTmp<cmptType, Type>::clear(tf);
		return tRes;
	}


	template<class Type>
	void cmptAv(Field<typename Field<Type>::cmptType>& res, const UList<Type>& f)
	{
		typedef typename Field<Type>::cmptType cmptType;
		TFOR_ALL_F_OP_FUNC_F(cmptType, res, =, cmptAv, Type, f)
	}

	template<class Type>
	tmp<Field<typename Field<Type>::cmptType>> cmptAv(const UList<Type>& f)
	{
		typedef typename Field<Type>::cmptType cmptType;
		tmp<Field<cmptType>> tRes(new Field<cmptType>(f.size()));
		cmptAv(tRes(), f);
		return tRes;
	}

	template<class Type>
	tmp<Field<typename Field<Type>::cmptType>> cmptAv(const tmp<Field<Type>>& tf)
	{
		typedef typename Field<Type>::cmptType cmptType;
		tmp<Field<cmptType>> tRes = reuseTmp<cmptType, Type>::New(tf);
		cmptAv(tRes(), tf());
		reuseTmp<cmptType, Type>::clear(tf);
		return tRes;
	}


	template<class Type>
	void cmptMag(Field<Type>& res, const UList<Type>& f)
	{
		TFOR_ALL_F_OP_FUNC_F(Type, res, =, cmptMag, Type, f)
	}

	template<class Type>
	tmp<Field<Type>> cmptMag(const UList<Type>& f)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f.size()));
		cmptMag(tRes(), f);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> cmptMag(const tmp<Field<Type>>& tf)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf);
		cmptMag(tRes(), tf());
		reuseTmp<Type, Type>::clear(tf);
		return tRes;
	}


	template<class Type>
	void cmptSign(Field<Type>& res, const UList<Type>& f)
	{
		TFOR_ALL_F_OP_FUNC_F(Type, res, =, cmptSign, Type, f)
	}

	template<class Type>
	tmp<Field<Type>> cmptSign(const UList<Type>& f)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f.size()));
		cmptSign(tRes(), f);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> cmptSign(const tmp<Field<Type>>& tf)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf);
		cmptSign(tRes(), tf());
		reuseTmp<Type, Type>::clear(tf);
		return tRes;
	}

	template<class Type>
	Type max(const UList<Type>& f)
	{
		if (f.size())
		{
			Type Max(f[0]);
			TFOR_ALL_S_OP_FUNC_F_S(Type, Max, =, max, Type, f, Type, Max)
				return Max;
		}
		else
		{
			return pTraits<Type>::min;
		}
	}

	template<class Type>
	Type max(const tmp<Field<Type>>& tf1) { Type res = max(tf1()); tf1.clear(); return res; }

	template<class Type>
	Type min(const UList<Type>& f)
	{
		if (f.size())
		{
			Type Min(f[0]);
			TFOR_ALL_S_OP_FUNC_F_S(Type, Min, =, min, Type, f, Type, Min)
				return Min;
		}
		else
		{
			return pTraits<Type>::max;
		}
	}

	template<class Type>
	Type min(const tmp<Field<Type>>& tf1) { Type res = min(tf1()); tf1.clear(); return res; }

	template<class Type>
	Type sum(const UList<Type>& f)
	{
		if (f.size())
		{
			Type Sum = pTraits<Type>::zero;
			TFOR_ALL_S_OP_F(Type, Sum, +=, Type, f)
				return Sum;
		}
		else
		{
			return pTraits<Type>::zero;
		}
	}

	template<class Type>
	Type sum(const tmp<Field<Type>>& tf1) { Type res = sum(tf1()); tf1.clear(); return res; }


	template<class Type>
	scalar sumProd(const UList<Type>& f1, const UList<Type>& f2)
	{
		if (f1.size() && (f1.size() == f2.size()))
		{
			scalar sp = 0.0;
			TFOR_ALL_S_OP_FUNC_F_F(scalar, sp, +=, cmptSumMultiply, Type, f1, Type, f2)
				return sp;
		}
		else
		{
			return 0.0;
		}
	}


	template<class Type>
	Type sumCmptProd(const UList<Type>& f1, const UList<Type>& f2)
	{
		if (f1.size() && (f1.size() == f2.size()))
		{
			Type SumProd = pTraits<Type>::zero;
			TFOR_ALL_S_OP_FUNC_F_F(Type, SumProd, +=, cmptMultiply, Type, f1, Type, f2)
				return SumProd;
		}
		else
		{
			return pTraits<Type>::zero;
		}
	}


	template<class Type>
	scalar sumSqr(const UList<Type>& f)
	{
		if (f.size())
		{
			scalar SumSqr = 0.0;
			TFOR_ALL_S_OP_FUNC_F(scalar, SumSqr, +=, sqr, Type, f)
				return SumSqr;
		}
		else
		{
			return 0.0;
		}
	}

	template <class Type>
	scalar sumSqr(const tmp<Field<Type>>& tf1)
	{
		scalar res = sumSqr(tf1());
		tf1.clear();
		return res;
	}

	template<class Type>
	scalar sumMag(const UList<Type>& f)
	{
		if (f.size())
		{
			scalar SumMag = 0.0;
			TFOR_ALL_S_OP_FUNC_F(scalar, SumMag, +=, mag, Type, f)
				return SumMag;
		}
		else
		{
			return 0.0;
		}
	}

	template <class Type>
	scalar sumMag(const tmp<Field<Type>>& tf1)
	{
		scalar res = sumMag(tf1());
		tf1.clear();
		return res;
	}


	template<class Type>
	Type sumCmptMag(const UList<Type>& f)
	{
		if (f.size())
		{
			Type SumMag = pTraits<Type>::zero;
			TFOR_ALL_S_OP_FUNC_F(scalar, SumMag, +=, cmptMag, Type, f)
				return SumMag;
		}
		else
		{
			return pTraits<Type>::zero;
		}
	}

	template <class Type>
	Type sumCmptMag(const tmp<Field<Type>>& tf1)
	{
		Type res = sumCmptMag(tf1());
		tf1.clear();
		return res;
	}

	template<class Type>
	Type average(const UList<Type>& f)
	{
		if (f.size())
		{
			Type avrg = sum(f) / f.size();

			return avrg;
		}
		else
		{
			WarningIn("average(const UList<Type>&)")
				<< "empty field, returning zero" << endl;

			return pTraits<Type>::zero;
		}
	}

	template<class Type>
	Type average(const tmp<Field<Type>>& tf1)
	{
		Type res = average(tf1()); tf1.clear(); return res;
	}


	template <class Type>
	Type gMax(const UList<Type>& f)
	{
		Type res = max(f);
		reduce(res, maxOp<Type>());
		return res;
	}

	template<class Type>
	Type gMax(const tmp<Field<Type>>& tf1)
	{
		Type res = gMax(tf1()); tf1.clear(); return res;
	}

	template <class Type>
	Type gMin(const UList<Type>& f)
	{
		Type res = min(f);
		reduce(res, minOp<Type>());
		return res;
	}

	template<class Type>
	Type gMin(const tmp<Field<Type>>& tf1)
	{
		Type res = gMin(tf1()); tf1.clear(); return res;
	}

	template <class Type>
	Type gSum(const UList<Type>& f)
	{
		Type res = sum(f);
		reduce(res, sumOp<Type>());
		return res;
	}

	template<class Type>
	Type gSum(const tmp<Field<Type>>& tf1)
	{
		Type res = gSum(tf1()); tf1.clear(); return res;
	}

	template <class Type>
	scalar gSumSqr(const UList<Type>& f)
	{
		scalar res = sumSqr(f);
		reduce(res, sumOp<scalar>());
		return res;
	}

	template <class Type>
	scalar gSumSqr(const tmp<Field<Type>>& tf1)
	{
		scalar res = gSumSqr(tf1());
		tf1.clear();
		return res;
	}

	template <class Type>
	scalar gSumMag(const UList<Type>& f)
	{
		scalar res = sumMag(f);
		reduce(res, sumOp<scalar>());
		return res;
	}

	template <class Type>
	scalar gSumMag(const tmp<Field<Type>>& tf1)
	{
		scalar res = gSumMag(tf1());
		tf1.clear();
		return res;
	}

	template <class Type>
	Type gSumCmptMag(const UList<Type>& f)
	{
		Type res = sumCmptMag(f);
		reduce(res, sumOp<Type>());
		return res;
	}

	template <class Type>
	Type gSumCmptMag(const tmp<Field<Type>>& tf1)
	{
		Type res = gSumCmptMag(tf1());
		tf1.clear();
		return res;
	}

	template<class Type>
	scalar gSumProd(const UList<Type>& f1, const UList<Type>& f2)
	{
		scalar SumProd = sumProd(f1, f2);
		reduce(SumProd, sumOp<scalar>());
		return SumProd;
	}

	template<class Type>
	Type gSumCmptProd(const UList<Type>& f1, const UList<Type>& f2)
	{
		scalar SumProd = sumCmptProd(f1, f2);
		reduce(SumProd, sumOp<scalar>());
		return SumProd;
	}

	template<class Type>
	Type gAverage(const UList<Type>& f)
	{
		label n = f.size();
		reduce(n, sumOp<label>());

		if (n > 0)
		{
			Type avrg = gSum(f) / n;

			return avrg;
		}
		else
		{
			WarningIn("gAverage(const UList<Type>&)")
				<< "empty field, returning zero." << endl;

			return pTraits<Type>::zero;
		}
	}

	template <class Type>
	Type gAverage(const tmp<Field<Type>>& tf1)
	{
		Type res = gAverage(tf1());
		tf1.clear();
		return res;
	}

	template<class Type>
	void max(Field<Type>& res, const UList<Type>& f1, const UList<Type>& f2)
	{
		TFOR_ALL_F_OP_FUNC_F_F(Type, res, =, ::tnbLib::max, Type, f1, Type, f2)
	}

	template<class Type>
	tmp<Field<Type>> max(const UList<Type>& f1, const UList<Type>& f2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f1.size()));
		max(tRes(), f1, f2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> max(const UList<Type>& f1, const tmp<Field<Type>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf2);
		max(tRes(), f1, tf2());
		reuseTmp<Type, Type>::clear(tf2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> max(const tmp<Field<Type>>& tf1, const UList<Type>& f2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf1);
		max(tRes(), tf1(), f2);
		reuseTmp<Type, Type>::clear(tf1);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> max(const tmp<Field<Type>>& tf1, const tmp<Field<Type>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmpTmp<Type, Type, Type, Type>::New(tf1, tf2);
		max(tRes(), tf1(), tf2());
		reuseTmpTmp<Type, Type, Type, Type>::clear(tf1, tf2);
		return tRes;
	}

	template<class Type>
	void min(Field<Type>& res, const UList<Type>& f1, const UList<Type>& f2)
	{
		TFOR_ALL_F_OP_FUNC_F_F(Type, res, =, ::tnbLib::min, Type, f1, Type, f2)
	}

	template<class Type>
	tmp<Field<Type>> min(const UList<Type>& f1, const UList<Type>& f2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f1.size()));
		min(tRes(), f1, f2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> min(const UList<Type>& f1, const tmp<Field<Type>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf2);
		min(tRes(), f1, tf2());
		reuseTmp<Type, Type>::clear(tf2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> min(const tmp<Field<Type>>& tf1, const UList<Type>& f2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf1);
		min(tRes(), tf1(), f2);
		reuseTmp<Type, Type>::clear(tf1);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> min(const tmp<Field<Type>>& tf1, const tmp<Field<Type>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmpTmp<Type, Type, Type, Type>::New(tf1, tf2);
		min(tRes(), tf1(), tf2());
		reuseTmpTmp<Type, Type, Type, Type>::clear(tf1, tf2);
		return tRes;
	}

	template<class Type>
	void cmptMultiply(Field<Type>& res, const UList<Type>& f1, const UList<Type>& f2)
	{
		TFOR_ALL_F_OP_FUNC_F_F(Type, res, =, ::tnbLib::cmptMultiply, Type, f1, Type, f2)
	}

	template<class Type>
	tmp<Field<Type>> cmptMultiply(const UList<Type>& f1, const UList<Type>& f2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f1.size()));
		cmptMultiply(tRes(), f1, f2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> cmptMultiply(const UList<Type>& f1, const tmp<Field<Type>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf2);
		cmptMultiply(tRes(), f1, tf2());
		reuseTmp<Type, Type>::clear(tf2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> cmptMultiply(const tmp<Field<Type>>& tf1, const UList<Type>& f2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf1);
		cmptMultiply(tRes(), tf1(), f2);
		reuseTmp<Type, Type>::clear(tf1);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> cmptMultiply(const tmp<Field<Type>>& tf1, const tmp<Field<Type>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmpTmp<Type, Type, Type, Type>::New(tf1, tf2);
		cmptMultiply(tRes(), tf1(), tf2());
		reuseTmpTmp<Type, Type, Type, Type>::clear(tf1, tf2);
		return tRes;
	}

	template<class Type>
	void cmptDivide(Field<Type>& res, const UList<Type>& f1, const UList<Type>& f2)
	{
		TFOR_ALL_F_OP_FUNC_F_F(Type, res, =, ::tnbLib::cmptDivide, Type, f1, Type, f2)
	}

	template<class Type>
	tmp<Field<Type>> cmptDivide(const UList<Type>& f1, const UList<Type>& f2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f1.size()));
		cmptDivide(tRes(), f1, f2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> cmptDivide(const UList<Type>& f1, const tmp<Field<Type>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf2);
		cmptDivide(tRes(), f1, tf2());
		reuseTmp<Type, Type>::clear(tf2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> cmptDivide(const tmp<Field<Type>>& tf1, const UList<Type>& f2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf1);
		cmptDivide(tRes(), tf1(), f2);
		reuseTmp<Type, Type>::clear(tf1);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> cmptDivide(const tmp<Field<Type>>& tf1, const tmp<Field<Type>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmpTmp<Type, Type, Type, Type>::New(tf1, tf2);
		cmptDivide(tRes(), tf1(), tf2());
		reuseTmpTmp<Type, Type, Type, Type>::clear(tf1, tf2);
		return tRes;
	}


	template<class Type>
	void max(Field<Type>& res, const Type& s1, const UList<Type>& f2)
	{
		TFOR_ALL_F_OP_FUNC_S_F(Type, res, =, ::tnbLib::max, Type, s1, Type, f2)
	}

	template<class Type>
	tmp<Field<Type>> max(const Type& s1, const UList<Type>& f2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f2.size()));
		max(tRes(), s1, f2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> max(const Type& s1, const tmp<Field<Type>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf2);
		max(tRes(), s1, tf2());
		reuseTmp<Type, Type>::clear(tf2);
		return tRes;
	}

	template<class Type>
	void max(Field<Type>& res, const UList<Type>& f1, const Type& s2)
	{
		TFOR_ALL_F_OP_FUNC_F_S(Type, res, =, ::tnbLib::max, Type, f1, Type, s2)
	}

	template<class Type>
	tmp<Field<Type>> max(const UList<Type>& f1, const Type& s2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f1.size()));
		max(tRes(), f1, s2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> max(const tmp<Field<Type>>& tf1, const Type& s2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf1);
		max(tRes(), tf1(), s2);
		reuseTmp<Type, Type>::clear(tf1);
		return tRes;
	}

	template<class Type>
	void min(Field<Type>& res, const Type& s1, const UList<Type>& f2)
	{
		TFOR_ALL_F_OP_FUNC_S_F(Type, res, =, ::tnbLib::min, Type, s1, Type, f2)
	}

	template<class Type>
	tmp<Field<Type>> min(const Type& s1, const UList<Type>& f2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f2.size()));
		min(tRes(), s1, f2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> min(const Type& s1, const tmp<Field<Type>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf2);
		min(tRes(), s1, tf2());
		reuseTmp<Type, Type>::clear(tf2);
		return tRes;
	}

	template<class Type>
	void min(Field<Type>& res, const UList<Type>& f1, const Type& s2)
	{
		TFOR_ALL_F_OP_FUNC_F_S(Type, res, =, ::tnbLib::min, Type, f1, Type, s2)
	}

	template<class Type>
	tmp<Field<Type>> min(const UList<Type>& f1, const Type& s2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f1.size()));
		min(tRes(), f1, s2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> min(const tmp<Field<Type>>& tf1, const Type& s2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf1);
		min(tRes(), tf1(), s2);
		reuseTmp<Type, Type>::clear(tf1);
		return tRes;
	}

	template<class Type>
	void cmptMultiply(Field<Type>& res, const Type& s1, const UList<Type>& f2)
	{
		TFOR_ALL_F_OP_FUNC_S_F(Type, res, =, ::tnbLib::cmptMultiply, Type, s1, Type, f2)
	}

	template<class Type>
	tmp<Field<Type>> cmptMultiply(const Type& s1, const UList<Type>& f2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f2.size()));
		cmptMultiply(tRes(), s1, f2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> cmptMultiply(const Type& s1, const tmp<Field<Type>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf2);
		cmptMultiply(tRes(), s1, tf2());
		reuseTmp<Type, Type>::clear(tf2);
		return tRes;
	}

	template<class Type>
	void cmptMultiply(Field<Type>& res, const UList<Type>& f1, const Type& s2)
	{
		TFOR_ALL_F_OP_FUNC_F_S(Type, res, =, ::tnbLib::cmptMultiply, Type, f1, Type, s2)
	}

	template<class Type>
	tmp<Field<Type>> cmptMultiply(const UList<Type>& f1, const Type& s2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f1.size()));
		cmptMultiply(tRes(), f1, s2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> cmptMultiply(const tmp<Field<Type>>& tf1, const Type& s2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf1);
		cmptMultiply(tRes(), tf1(), s2);
		reuseTmp<Type, Type>::clear(tf1);
		return tRes;
	}

	template<class Type>
	void cmptDivide(Field<Type>& res, const Type& s1, const UList<Type>& f2)
	{
		TFOR_ALL_F_OP_FUNC_S_F(Type, res, =, ::tnbLib::cmptDivide, Type, s1, Type, f2)
	}

	template<class Type>
	tmp<Field<Type>> cmptDivide(const Type& s1, const UList<Type>& f2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f2.size()));
		cmptDivide(tRes(), s1, f2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> cmptDivide(const Type& s1, const tmp<Field<Type>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf2);
		cmptDivide(tRes(), s1, tf2());
		reuseTmp<Type, Type>::clear(tf2);
		return tRes;
	}

	template<class Type>
	void cmptDivide(Field<Type>& res, const UList<Type>& f1, const Type& s2)
	{
		TFOR_ALL_F_OP_FUNC_F_S(Type, res, =, ::tnbLib::cmptDivide, Type, f1, Type, s2)
	}

	template<class Type>
	tmp<Field<Type>> cmptDivide(const UList<Type>& f1, const Type& s2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f1.size()));
		cmptDivide(tRes(), f1, s2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> cmptDivide(const tmp<Field<Type>>& tf1, const Type& s2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf1);
		cmptDivide(tRes(), tf1(), s2);
		reuseTmp<Type, Type>::clear(tf1);
		return tRes;
	}


	/* * * * * * * * * * * * * * * * Global operators  * * * * * * * * * * * * * */


	template<class Type>
	void negate(Field<Type>& res, const UList<Type>& f)
	{
		TFOR_ALL_F_OP_OP_F(Type, res, =, -, Type, f)
	}

	template<class Type>
	tmp<Field<Type>> operator -(const UList<Type>& f)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f.size()));
		negate(tRes(), f);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> operator -(const tmp<Field<Type>>& tf)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf);
		negate(tRes(), tf());
		reuseTmp<Type, Type>::clear(tf);
		return tRes;
	}


	template<class Type>
	void multiply(Field<Type>& res, const UList<Type>& f1, const UList<scalar>& f2)
	{
		TFOR_ALL_F_OP_F_OP_F(Type, res, =, Type, f1, *, scalar, f2)
	}

	template<class Type>
	tmp<Field<Type>> operator *(const UList<Type>& f1, const UList<scalar>& f2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f1.size()));
		multiply(tRes(), f1, f2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> operator *(const UList<Type>& f1, const tmp<Field<scalar>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, scalar>::New(tf2);
		multiply(tRes(), f1, tf2());
		reuseTmp<Type, scalar>::clear(tf2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> operator *(const tmp<Field<Type>>& tf1, const UList<scalar>& f2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf1);
		multiply(tRes(), tf1(), f2);
		reuseTmp<Type, Type>::clear(tf1);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> operator *(const tmp<Field<Type>>& tf1, const tmp<Field<scalar>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmpTmp<Type, Type, Type, scalar>::New(tf1, tf2);
		multiply(tRes(), tf1(), tf2());
		reuseTmpTmp<Type, Type, Type, scalar>::clear(tf1, tf2);
		return tRes;
	}

	template<class Type>
	void multiply(Field<Type>& res, const UList<scalar>& f1, const UList<Type>& f2)
	{
		TFOR_ALL_F_OP_F_OP_F(Type, res, =, scalar, f1, *, Type, f2)
	}

	template<class Type>
	tmp<Field<Type>> operator *(const UList<scalar>& f1, const UList<Type>& f2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f1.size()));
		multiply(tRes(), f1, f2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> operator *(const UList<scalar>& f1, const tmp<Field<Type>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf2);
		multiply(tRes(), f1, tf2());
		reuseTmp<Type, Type>::clear(tf2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> operator *(const tmp<Field<scalar>>& tf1, const UList<Type>& f2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, scalar>::New(tf1);
		multiply(tRes(), tf1(), f2);
		reuseTmp<Type, scalar>::clear(tf1);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> operator *(const tmp<Field<scalar>>& tf1, const tmp<Field<Type>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmpTmp<Type, scalar, scalar, Type>::New(tf1, tf2);
		multiply(tRes(), tf1(), tf2());
		reuseTmpTmp<Type, scalar, scalar, Type>::clear(tf1, tf2);
		return tRes;
	}

	template<class Type>
	void divide(Field<Type>& res, const UList<Type>& f1, const UList<scalar>& f2)
	{
		TFOR_ALL_F_OP_F_OP_F(Type, res, =, Type, f1, / , scalar, f2)
	}

	template<class Type>
	tmp<Field<Type>> operator /(const UList<Type>& f1, const UList<scalar>& f2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f1.size()));
		divide(tRes(), f1, f2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> operator /(const UList<Type>& f1, const tmp<Field<scalar>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, scalar>::New(tf2);
		divide(tRes(), f1, tf2());
		reuseTmp<Type, scalar>::clear(tf2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> operator /(const tmp<Field<Type>>& tf1, const UList<scalar>& f2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf1);
		divide(tRes(), tf1(), f2);
		reuseTmp<Type, Type>::clear(tf1);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> operator /(const tmp<Field<Type>>& tf1, const tmp<Field<scalar>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmpTmp<Type, Type, Type, scalar>::New(tf1, tf2);
		divide(tRes(), tf1(), tf2());
		reuseTmpTmp<Type, Type, Type, scalar>::clear(tf1, tf2);
		return tRes;
	}


	template<class Type>
	void multiply(Field<Type>& res, const scalar& s1, const UList<Type>& f2)
	{
		TFOR_ALL_F_OP_S_OP_F(Type, res, =, scalar, s1, *, Type, f2)
	}

	template<class Type>
	tmp<Field<Type>> operator *(const scalar& s1, const UList<Type>& f2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f2.size()));
		multiply(tRes(), s1, f2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> operator *(const scalar& s1, const tmp<Field<Type>>& tf2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf2);
		multiply(tRes(), s1, tf2());
		reuseTmp<Type, Type>::clear(tf2);
		return tRes;
	}

	template<class Type>
	void multiply(Field<Type>& res, const UList<Type>& f1, const scalar& s2)
	{
		TFOR_ALL_F_OP_F_OP_S(Type, res, =, Type, f1, *, scalar, s2)
	}

	template<class Type>
	tmp<Field<Type>> operator *(const UList<Type>& f1, const scalar& s2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f1.size()));
		multiply(tRes(), f1, s2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> operator *(const tmp<Field<Type>>& tf1, const scalar& s2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf1);
		multiply(tRes(), tf1(), s2);
		reuseTmp<Type, Type>::clear(tf1);
		return tRes;
	}


	template<class Type>
	void divide(Field<Type>& res, const UList<Type>& f1, const scalar& s2)
	{
		TFOR_ALL_F_OP_F_OP_S(Type, res, =, Type, f1, / , scalar, s2)
	}

	template<class Type>
	tmp<Field<Type>> operator /(const UList<Type>& f1, const scalar& s2)
	{
		tmp<Field<Type>> tRes(new Field<Type>(f1.size()));
		divide(tRes(), f1, s2);
		return tRes;
	}

	template<class Type>
	tmp<Field<Type>> operator /(const tmp<Field<Type>>& tf1, const scalar& s2)
	{
		tmp<Field<Type>> tRes = reuseTmp<Type, Type>::New(tf1);
		divide(tRes(), tf1(), s2);
		reuseTmp<Type, Type>::clear(tf1);
		return tRes;
	}


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


	template <class Type1, class Type2>
	void add(Field<typename typeOfSum<Type1, Type2>::type>& res, const UList<Type1>& f1, const UList<Type2>& f2)
	{
		typedef typename typeOfSum<Type1, Type2>::type productType;
		TFOR_ALL_F_OP_F_OP_F(productType, res, =, Type1, f1, +, Type2, f2)
	}

	template <class Type1, class Type2>
	tmp<Field<typename typeOfSum<Type1, Type2>::type>> operator +(const UList<Type1>& f1, const UList<Type2>& f2)
	{
		typedef typename typeOfSum<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		add(tRes(), f1, f2);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename typeOfSum<Type1, Type2>::type>> operator +(const UList<Type1>& f1, const tmp<Field<Type2>>& tf2)
	{
		typedef typename typeOfSum<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type2>::New(tf2);
		add(tRes(), f1, tf2());
		reuseTmp<productType, Type2>::clear(tf2);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename typeOfSum<Type1, Type2>::type>> operator +(const tmp<Field<Type1>>& tf1, const UList<Type2>& f2)
	{
		typedef typename typeOfSum<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type1>::New(tf1);
		add(tRes(), tf1(), f2);
		reuseTmp<productType, Type1>::clear(tf1);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename typeOfSum<Type1, Type2>::type>> operator +(const tmp<Field<Type1>>& tf1,
		const tmp<Field<Type2>>& tf2)
	{
		typedef typename typeOfSum<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmpTmp<productType, Type1, Type1, Type2>::New(tf1, tf2);
		add(tRes(), tf1(), tf2());
		reuseTmpTmp<productType, Type1, Type1, Type2>::clear(tf1, tf2);
		return tRes;
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	void add(Field<typename typeOfSum<Type, Form>::type>& res, const UList<Type>& f1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename typeOfSum<Type, Form>::type productType;
		TFOR_ALL_F_OP_F_OP_S(productType, res, =, Type, f1, +, Form, static_cast<const Form&>(vs))
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	tmp<Field<typename typeOfSum<Type, Form>::type>> operator +(const UList<Type>& f1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename typeOfSum<Type, Form>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		add(tRes(), f1, static_cast<const Form&>(vs));
		return tRes;
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	tmp<Field<typename typeOfSum<Type, Form>::type>> operator +(const tmp<Field<Type>>& tf1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename typeOfSum<Type, Form>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type>::New(tf1);
		add(tRes(), tf1(), static_cast<const Form&>(vs));
		reuseTmp<productType, Type>::clear(tf1);
		return tRes;
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	void add(Field<typename typeOfSum<Form, Type>::type>& res, const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const UList<Type>& f1)
	{
		typedef typename typeOfSum<Form, Type>::type productType;
		TFOR_ALL_F_OP_S_OP_F(productType, res, =, Form, static_cast<const Form&>(vs), +, Type, f1)
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	tmp<Field<typename typeOfSum<Form, Type>::type>> operator +(const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const UList<Type>& f1)
	{
		typedef typename typeOfSum<Form, Type>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		add(tRes(), static_cast<const Form&>(vs), f1);
		return tRes;
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	tmp<Field<typename typeOfSum<Form, Type>::type>> operator +(const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const tmp<Field<Type>>& tf1)
	{
		typedef typename typeOfSum<Form, Type>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type>::New(tf1);
		add(tRes(), static_cast<const Form&>(vs), tf1());
		reuseTmp<productType, Type>::clear(tf1);
		return tRes;
	}


	template <class Type1, class Type2>
	void subtract(Field<typename typeOfSum<Type1, Type2>::type>& res, const UList<Type1>& f1, const UList<Type2>& f2)
	{
		typedef typename typeOfSum<Type1, Type2>::type productType;
		TFOR_ALL_F_OP_F_OP_F(productType, res, =, Type1, f1, -, Type2, f2)
	}

	template <class Type1, class Type2>
	tmp<Field<typename typeOfSum<Type1, Type2>::type>> operator -(const UList<Type1>& f1, const UList<Type2>& f2)
	{
		typedef typename typeOfSum<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		subtract(tRes(), f1, f2);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename typeOfSum<Type1, Type2>::type>> operator -(const UList<Type1>& f1, const tmp<Field<Type2>>& tf2)
	{
		typedef typename typeOfSum<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type2>::New(tf2);
		subtract(tRes(), f1, tf2());
		reuseTmp<productType, Type2>::clear(tf2);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename typeOfSum<Type1, Type2>::type>> operator -(const tmp<Field<Type1>>& tf1, const UList<Type2>& f2)
	{
		typedef typename typeOfSum<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type1>::New(tf1);
		subtract(tRes(), tf1(), f2);
		reuseTmp<productType, Type1>::clear(tf1);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename typeOfSum<Type1, Type2>::type>> operator -(const tmp<Field<Type1>>& tf1,
		const tmp<Field<Type2>>& tf2)
	{
		typedef typename typeOfSum<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmpTmp<productType, Type1, Type1, Type2>::New(tf1, tf2);
		subtract(tRes(), tf1(), tf2());
		reuseTmpTmp<productType, Type1, Type1, Type2>::clear(tf1, tf2);
		return tRes;
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	void subtract(Field<typename typeOfSum<Type, Form>::type>& res, const UList<Type>& f1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename typeOfSum<Type, Form>::type productType;
		TFOR_ALL_F_OP_F_OP_S(productType, res, =, Type, f1, -, Form, static_cast<const Form&>(vs))
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	tmp<Field<typename typeOfSum<Type, Form>::type>> operator -(const UList<Type>& f1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename typeOfSum<Type, Form>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		subtract(tRes(), f1, static_cast<const Form&>(vs));
		return tRes;
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	tmp<Field<typename typeOfSum<Type, Form>::type>> operator -(const tmp<Field<Type>>& tf1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename typeOfSum<Type, Form>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type>::New(tf1);
		subtract(tRes(), tf1(), static_cast<const Form&>(vs));
		reuseTmp<productType, Type>::clear(tf1);
		return tRes;
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	void subtract(Field<typename typeOfSum<Form, Type>::type>& res, const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const UList<Type>& f1)
	{
		typedef typename typeOfSum<Form, Type>::type productType;
		TFOR_ALL_F_OP_S_OP_F(productType, res, =, Form, static_cast<const Form&>(vs), -, Type, f1)
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	tmp<Field<typename typeOfSum<Form, Type>::type>> operator -(const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const UList<Type>& f1)
	{
		typedef typename typeOfSum<Form, Type>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		subtract(tRes(), static_cast<const Form&>(vs), f1);
		return tRes;
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	tmp<Field<typename typeOfSum<Form, Type>::type>> operator -(const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const tmp<Field<Type>>& tf1)
	{
		typedef typename typeOfSum<Form, Type>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type>::New(tf1);
		subtract(tRes(), static_cast<const Form&>(vs), tf1());
		reuseTmp<productType, Type>::clear(tf1);
		return tRes;
	}


	template <class Type1, class Type2>
	void outer(Field<typename outerProduct<Type1, Type2>::type>& res, const UList<Type1>& f1, const UList<Type2>& f2)
	{
		typedef typename outerProduct<Type1, Type2>::type productType;
		TFOR_ALL_F_OP_F_OP_F(productType, res, =, Type1, f1, *, Type2, f2)
	}

	template <class Type1, class Type2>
	tmp<Field<typename outerProduct<Type1, Type2>::type>> operator *(const UList<Type1>& f1, const UList<Type2>& f2)
	{
		typedef typename outerProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		outer(tRes(), f1, f2);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename outerProduct<Type1, Type2>::type>> operator *(const UList<Type1>& f1, const tmp<Field<Type2>>& tf2)
	{
		typedef typename outerProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type2>::New(tf2);
		outer(tRes(), f1, tf2());
		reuseTmp<productType, Type2>::clear(tf2);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename outerProduct<Type1, Type2>::type>> operator *(const tmp<Field<Type1>>& tf1, const UList<Type2>& f2)
	{
		typedef typename outerProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type1>::New(tf1);
		outer(tRes(), tf1(), f2);
		reuseTmp<productType, Type1>::clear(tf1);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename outerProduct<Type1, Type2>::type>> operator *(const tmp<Field<Type1>>& tf1,
		const tmp<Field<Type2>>& tf2)
	{
		typedef typename outerProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmpTmp<productType, Type1, Type1, Type2>::New(tf1, tf2);
		outer(tRes(), tf1(), tf2());
		reuseTmpTmp<productType, Type1, Type1, Type2>::clear(tf1, tf2);
		return tRes;
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	void outer(Field<typename outerProduct<Type, Form>::type>& res, const UList<Type>& f1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename outerProduct<Type, Form>::type productType;
		TFOR_ALL_F_OP_F_OP_S(productType, res, =, Type, f1, *, Form, static_cast<const Form&>(vs))
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	tmp<Field<typename outerProduct<Type, Form>::type>> operator *(const UList<Type>& f1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename outerProduct<Type, Form>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		outer(tRes(), f1, static_cast<const Form&>(vs));
		return tRes;
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	tmp<Field<typename outerProduct<Type, Form>::type>> operator *(const tmp<Field<Type>>& tf1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename outerProduct<Type, Form>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type>::New(tf1);
		outer(tRes(), tf1(), static_cast<const Form&>(vs));
		reuseTmp<productType, Type>::clear(tf1);
		return tRes;
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	void outer(Field<typename outerProduct<Form, Type>::type>& res, const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const UList<Type>& f1)
	{
		typedef typename outerProduct<Form, Type>::type productType;
		TFOR_ALL_F_OP_S_OP_F(productType, res, =, Form, static_cast<const Form&>(vs), *, Type, f1)
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	tmp<Field<typename outerProduct<Form, Type>::type>> operator *(const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const UList<Type>& f1)
	{
		typedef typename outerProduct<Form, Type>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		outer(tRes(), static_cast<const Form&>(vs), f1);
		return tRes;
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	tmp<Field<typename outerProduct<Form, Type>::type>> operator *(const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const tmp<Field<Type>>& tf1)
	{
		typedef typename outerProduct<Form, Type>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type>::New(tf1);
		outer(tRes(), static_cast<const Form&>(vs), tf1());
		reuseTmp<productType, Type>::clear(tf1);
		return tRes;
	}

	template <class Type1, class Type2>
	void cross(Field<typename crossProduct<Type1, Type2>::type>& res, const UList<Type1>& f1, const UList<Type2>& f2)
	{
		typedef typename crossProduct<Type1, Type2>::type productType;
		TFOR_ALL_F_OP_F_OP_F(productType, res, =, Type1, f1, ^, Type2, f2)
	}

	template <class Type1, class Type2>
	tmp<Field<typename crossProduct<Type1, Type2>::type>> operator ^(const UList<Type1>& f1, const UList<Type2>& f2)
	{
		typedef typename crossProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		cross(tRes(), f1, f2);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename crossProduct<Type1, Type2>::type>> operator ^(const UList<Type1>& f1, const tmp<Field<Type2>>& tf2)
	{
		typedef typename crossProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type2>::New(tf2);
		cross(tRes(), f1, tf2());
		reuseTmp<productType, Type2>::clear(tf2);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename crossProduct<Type1, Type2>::type>> operator ^(const tmp<Field<Type1>>& tf1, const UList<Type2>& f2)
	{
		typedef typename crossProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type1>::New(tf1);
		cross(tRes(), tf1(), f2);
		reuseTmp<productType, Type1>::clear(tf1);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename crossProduct<Type1, Type2>::type>> operator ^(const tmp<Field<Type1>>& tf1,
		const tmp<Field<Type2>>& tf2)
	{
		typedef typename crossProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmpTmp<productType, Type1, Type1, Type2>::New(tf1, tf2);
		cross(tRes(), tf1(), tf2());
		reuseTmpTmp<productType, Type1, Type1, Type2>::clear(tf1, tf2);
		return tRes;
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	void cross(Field<typename crossProduct<Type, Form>::type>& res, const UList<Type>& f1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename crossProduct<Type, Form>::type productType;
		TFOR_ALL_F_OP_F_OP_S(productType, res, =, Type, f1, ^, Form, static_cast<const Form&>(vs))
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	tmp<Field<typename crossProduct<Type, Form>::type>> operator ^(const UList<Type>& f1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename crossProduct<Type, Form>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		cross(tRes(), f1, static_cast<const Form&>(vs));
		return tRes;
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	tmp<Field<typename crossProduct<Type, Form>::type>> operator ^(const tmp<Field<Type>>& tf1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename crossProduct<Type, Form>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type>::New(tf1);
		cross(tRes(), tf1(), static_cast<const Form&>(vs));
		reuseTmp<productType, Type>::clear(tf1);
		return tRes;
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	void cross(Field<typename crossProduct<Form, Type>::type>& res, const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const UList<Type>& f1)
	{
		typedef typename crossProduct<Form, Type>::type productType;
		TFOR_ALL_F_OP_S_OP_F(productType, res, =, Form, static_cast<const Form&>(vs), ^, Type, f1)
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	tmp<Field<typename crossProduct<Form, Type>::type>> operator ^(const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const UList<Type>& f1)
	{
		typedef typename crossProduct<Form, Type>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		cross(tRes(), static_cast<const Form&>(vs), f1);
		return tRes;
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	tmp<Field<typename crossProduct<Form, Type>::type>> operator ^(const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const tmp<Field<Type>>& tf1)
	{
		typedef typename crossProduct<Form, Type>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type>::New(tf1);
		cross(tRes(), static_cast<const Form&>(vs), tf1());
		reuseTmp<productType, Type>::clear(tf1);
		return tRes;
	}

	template <class Type1, class Type2>
	void dot(Field<typename innerProduct<Type1, Type2>::type>& res, const UList<Type1>& f1, const UList<Type2>& f2)
	{
		typedef typename innerProduct<Type1, Type2>::type productType;
		TFOR_ALL_F_OP_F_OP_F(productType, res, =, Type1, f1, &, Type2, f2)
	}

	template <class Type1, class Type2>
	tmp<Field<typename innerProduct<Type1, Type2>::type>> operator &(const UList<Type1>& f1, const UList<Type2>& f2)
	{
		typedef typename innerProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		dot(tRes(), f1, f2);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename innerProduct<Type1, Type2>::type>> operator &(const UList<Type1>& f1, const tmp<Field<Type2>>& tf2)
	{
		typedef typename innerProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type2>::New(tf2);
		dot(tRes(), f1, tf2());
		reuseTmp<productType, Type2>::clear(tf2);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename innerProduct<Type1, Type2>::type>> operator &(const tmp<Field<Type1>>& tf1, const UList<Type2>& f2)
	{
		typedef typename innerProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type1>::New(tf1);
		dot(tRes(), tf1(), f2);
		reuseTmp<productType, Type1>::clear(tf1);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename innerProduct<Type1, Type2>::type>> operator &(const tmp<Field<Type1>>& tf1,
		const tmp<Field<Type2>>& tf2)
	{
		typedef typename innerProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmpTmp<productType, Type1, Type1, Type2>::New(tf1, tf2);
		dot(tRes(), tf1(), tf2());
		reuseTmpTmp<productType, Type1, Type1, Type2>::clear(tf1, tf2);
		return tRes;
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	void dot(Field<typename innerProduct<Type, Form>::type>& res, const UList<Type>& f1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename innerProduct<Type, Form>::type productType;
		TFOR_ALL_F_OP_F_OP_S(productType, res, =, Type, f1, &, Form, static_cast<const Form&>(vs))
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	tmp<Field<typename innerProduct<Type, Form>::type>> operator &(const UList<Type>& f1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename innerProduct<Type, Form>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		dot(tRes(), f1, static_cast<const Form&>(vs));
		return tRes;
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	tmp<Field<typename innerProduct<Type, Form>::type>> operator &(const tmp<Field<Type>>& tf1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename innerProduct<Type, Form>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type>::New(tf1);
		dot(tRes(), tf1(), static_cast<const Form&>(vs));
		reuseTmp<productType, Type>::clear(tf1);
		return tRes;
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	void dot(Field<typename innerProduct<Form, Type>::type>& res, const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const UList<Type>& f1)
	{
		typedef typename innerProduct<Form, Type>::type productType;
		TFOR_ALL_F_OP_S_OP_F(productType, res, =, Form, static_cast<const Form&>(vs), &, Type, f1)
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	tmp<Field<typename innerProduct<Form, Type>::type>> operator &(const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const UList<Type>& f1)
	{
		typedef typename innerProduct<Form, Type>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		dot(tRes(), static_cast<const Form&>(vs), f1);
		return tRes;
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	tmp<Field<typename innerProduct<Form, Type>::type>> operator &(const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const tmp<Field<Type>>& tf1)
	{
		typedef typename innerProduct<Form, Type>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type>::New(tf1);
		dot(tRes(), static_cast<const Form&>(vs), tf1());
		reuseTmp<productType, Type>::clear(tf1);
		return tRes;
	}


	template <class Type1, class Type2>
	void dotdot(Field<typename scalarProduct<Type1, Type2>::type>& res, const UList<Type1>& f1, const UList<Type2>& f2)
	{
		typedef typename scalarProduct<Type1, Type2>::type productType;
		TFOR_ALL_F_OP_F_OP_F(productType, res, =, Type1, f1, &&, Type2, f2)
	}

	template <class Type1, class Type2>
	tmp<Field<typename scalarProduct<Type1, Type2>::type>> operator &&(const UList<Type1>& f1, const UList<Type2>& f2)
	{
		typedef typename scalarProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		dotdot(tRes(), f1, f2);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename scalarProduct<Type1, Type2>::type>> operator &&(const UList<Type1>& f1,
		const tmp<Field<Type2>>& tf2)
	{
		typedef typename scalarProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type2>::New(tf2);
		dotdot(tRes(), f1, tf2());
		reuseTmp<productType, Type2>::clear(tf2);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename scalarProduct<Type1, Type2>::type>> operator &&(const tmp<Field<Type1>>& tf1,
		const UList<Type2>& f2)
	{
		typedef typename scalarProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type1>::New(tf1);
		dotdot(tRes(), tf1(), f2);
		reuseTmp<productType, Type1>::clear(tf1);
		return tRes;
	}

	template <class Type1, class Type2>
	tmp<Field<typename scalarProduct<Type1, Type2>::type>> operator &&(const tmp<Field<Type1>>& tf1,
		const tmp<Field<Type2>>& tf2)
	{
		typedef typename scalarProduct<Type1, Type2>::type productType;
		tmp<Field<productType>> tRes = reuseTmpTmp<productType, Type1, Type1, Type2>::New(tf1, tf2);
		dotdot(tRes(), tf1(), tf2());
		reuseTmpTmp<productType, Type1, Type1, Type2>::clear(tf1, tf2);
		return tRes;
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	void dotdot(Field<typename scalarProduct<Type, Form>::type>& res, const UList<Type>& f1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename scalarProduct<Type, Form>::type productType;
		TFOR_ALL_F_OP_F_OP_S(productType, res, =, Type, f1, &&, Form, static_cast<const Form&>(vs))
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	tmp<Field<typename scalarProduct<Type, Form>::type>> operator &&(const UList<Type>& f1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename scalarProduct<Type, Form>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		dotdot(tRes(), f1, static_cast<const Form&>(vs));
		return tRes;
	}

	template <class Type, class Form, class Cmpt, int nCmpt>
	tmp<Field<typename scalarProduct<Type, Form>::type>> operator &&(const tmp<Field<Type>>& tf1,
		const VectorSpace<Form, Cmpt, nCmpt>& vs)
	{
		typedef typename scalarProduct<Type, Form>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type>::New(tf1);
		dotdot(tRes(), tf1(), static_cast<const Form&>(vs));
		reuseTmp<productType, Type>::clear(tf1);
		return tRes;
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	void dotdot(Field<typename scalarProduct<Form, Type>::type>& res, const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const UList<Type>& f1)
	{
		typedef typename scalarProduct<Form, Type>::type productType;
		TFOR_ALL_F_OP_S_OP_F(productType, res, =, Form, static_cast<const Form&>(vs), &&, Type, f1)
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	tmp<Field<typename scalarProduct<Form, Type>::type>> operator &&(const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const UList<Type>& f1)
	{
		typedef typename scalarProduct<Form, Type>::type productType;
		tmp<Field<productType>> tRes(new Field<productType>(f1.size()));
		dotdot(tRes(), static_cast<const Form&>(vs), f1);
		return tRes;
	}

	template <class Form, class Cmpt, int nCmpt, class Type>
	tmp<Field<typename scalarProduct<Form, Type>::type>> operator &&(const VectorSpace<Form, Cmpt, nCmpt>& vs,
		const tmp<Field<Type>>& tf1)
	{
		typedef typename scalarProduct<Form, Type>::type productType;
		tmp<Field<productType>> tRes = reuseTmp<productType, Type>::New(tf1);
		dotdot(tRes(), static_cast<const Form&>(vs), tf1());
		reuseTmp<productType, Type>::clear(tf1);
		return tRes;
	}

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //