#pragma once
#ifndef _scalarField_Header
#define _scalarField_Header

#include <Field.hxx>
#include <scalar.hxx>

#define TEMPLATE
#include <FieldFunctionsM.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	typedef Field<scalar> scalarField;

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	template<>
	tmp<scalarField> scalarField::component(const direction) const;

	void component
	(
		scalarField& sf,
		const UList<scalar>& f,
		const direction
	);

	template<>
	void scalarField::replace(const direction, const UList<scalar>& sf);

	template<>
	void scalarField::replace(const direction, const scalar& s);


	void stabilise(scalarField& Res, const UList<scalar>& sf, const scalar s);
	tmp<scalarField> stabilise(const UList<scalar>&, const scalar s);
	tmp<scalarField> stabilise(const tmp<scalarField>&, const scalar s);


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	BINARY_TYPE_OPERATOR(scalar, scalar, scalar, +, add)
		BINARY_TYPE_OPERATOR(scalar, scalar, scalar, -, subtract)

		BINARY_OPERATOR(scalar, scalar, scalar, *, multiply)
		BINARY_OPERATOR(scalar, scalar, scalar, / , divide)

		BINARY_TYPE_OPERATOR_SF(scalar, scalar, scalar, / , divide)

		BINARY_FUNCTION(scalar, scalar, scalar, pow)
		BINARY_TYPE_FUNCTION(scalar, scalar, scalar, pow)

		BINARY_FUNCTION(scalar, scalar, scalar, atan2)
		BINARY_TYPE_FUNCTION(scalar, scalar, scalar, atan2)


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

		UNARY_FUNCTION(scalar, scalar, pow3)
		UNARY_FUNCTION(scalar, scalar, pow4)
		UNARY_FUNCTION(scalar, scalar, pow5)
		UNARY_FUNCTION(scalar, scalar, pow6)
		UNARY_FUNCTION(scalar, scalar, sqrt)
		UNARY_FUNCTION(scalar, scalar, sign)
		UNARY_FUNCTION(scalar, scalar, pos)
		UNARY_FUNCTION(scalar, scalar, neg)
		UNARY_FUNCTION(scalar, scalar, exp)
		UNARY_FUNCTION(scalar, scalar, log)
		UNARY_FUNCTION(scalar, scalar, log10)
		UNARY_FUNCTION(scalar, scalar, sin)
		UNARY_FUNCTION(scalar, scalar, cos)
		UNARY_FUNCTION(scalar, scalar, tan)
		UNARY_FUNCTION(scalar, scalar, asin)
		UNARY_FUNCTION(scalar, scalar, acos)
		UNARY_FUNCTION(scalar, scalar, atan)
		UNARY_FUNCTION(scalar, scalar, sinh)
		UNARY_FUNCTION(scalar, scalar, cosh)
		UNARY_FUNCTION(scalar, scalar, tanh)
		UNARY_FUNCTION(scalar, scalar, asinh)
		UNARY_FUNCTION(scalar, scalar, acosh)
		UNARY_FUNCTION(scalar, scalar, atanh)
		UNARY_FUNCTION(scalar, scalar, erf)
		UNARY_FUNCTION(scalar, scalar, erfc)
		UNARY_FUNCTION(scalar, scalar, lgamma)
		UNARY_FUNCTION(scalar, scalar, j0)
		UNARY_FUNCTION(scalar, scalar, j1)
		UNARY_FUNCTION(scalar, scalar, y0)
		UNARY_FUNCTION(scalar, scalar, y1)


#define BesselFunc(func)                                            \
void func(scalarField& Res, const int n, const UList<scalar>& sf);  \
tmp<scalarField> func(const int n, const UList<scalar>&);           \
tmp<scalarField> func(const int n, const tmp<scalarField>&);

		BesselFunc(jn)
		BesselFunc(yn)

#undef BesselFunc


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <undefFieldFunctionsM.hxx>

#endif // !_scalarField_Header
