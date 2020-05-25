#include <ExpandTensorNField.hxx>

#include <ExpandTensorN.hxx>
#include <Field.hxx>
#include <VectorNFieldTypes.hxx>
#include <FieldM.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define UNARY_FUNCTION(typeF1, typeF2, FUNC)                                 \
                                                                             \
void FUNC(Field<typeF1>& f1, const UList<typeF2>& f2)                        \
{                                                                            \
    checkFields(f1, f2, #FUNC "(f1,f2)");                                    \
                                                                             \
    List_ACCESS(typeF1, f1, f1P);                                            \
    List_CONST_ACCESS(typeF2, f2, f2P);                                      \
                                                                             \
    List_FOR_ALL(f1,i)                                                       \
        FUNC(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));                  \
    List_END_FOR_ALL                                                         \
}                                                                            \
                                                                             \
void FUNC(Field<typeF1>& f1, const tmp<Field<typeF2> >& tf2)                 \
{                                                                            \
     FUNC(f1,tf2());                                                         \
     tf2.clear();                                                            \
}

#define ExpandFieldFunctions(tensorType, diagTensorType, sphericalTensorType, \
        vectorType, cmptType, args)											 \
                                                                              \
UNARY_FUNCTION(cmptType, tensorType, contractScalar)                          \
UNARY_FUNCTION(cmptType, diagTensorType, contractScalar)                      \
UNARY_FUNCTION(cmptType, sphericalTensorType, contractScalar)                 \
UNARY_FUNCTION(cmptType, vectorType, contractScalar)                          \
                                                                              \
UNARY_FUNCTION(vectorType, tensorType, contractLinear)                        \
UNARY_FUNCTION(vectorType, diagTensorType, contractLinear)                    \
UNARY_FUNCTION(vectorType, sphericalTensorType, contractLinear)               \
                                                                              \
UNARY_FUNCTION(vectorType, cmptType, expandScalar)                            \
UNARY_FUNCTION(tensorType, cmptType, expandScalar)                            \
UNARY_FUNCTION(diagTensorType, cmptType, expandScalar)                        \
UNARY_FUNCTION(sphericalTensorType, cmptType, expandScalar)                   \
                                                                              \
UNARY_FUNCTION(tensorType, vectorType, expandLinear)                          \
UNARY_FUNCTION(diagTensorType, vectorType, expandLinear)                      \
UNARY_FUNCTION(sphericalTensorType, vectorType, expandLinear)                 \
                                                                              \
UNARY_FUNCTION(vectorType, tensorType, sumToDiag)                             \
UNARY_FUNCTION(vectorType, tensorType, sumMagToDiag)

namespace tnbLib
{
	void contractScalar(Field<scalar>& f1, const UList<tensor2>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(tensor2, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<tensor2>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractScalar(Field<scalar>& f1, const UList<diagTensor2>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(diagTensor2, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<diagTensor2>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractScalar(Field<scalar>& f1, const UList<sphericalTensor2>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(sphericalTensor2, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<sphericalTensor2>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractScalar(Field<scalar>& f1, const UList<vector2>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(vector2, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<vector2>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractLinear(Field<vector2>& f1, const UList<tensor2>& f2)
	{
		checkFields(f1, f2, "contractLinear" "(f1,f2)");
		List_ACCESS(vector2, f1, f1P);
		List_CONST_ACCESS(tensor2, f2, f2P);
		List_FOR_ALL(f1,i)
			contractLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractLinear(Field<vector2>& f1, const tmp<Field<tensor2>>& tf2)
	{
		contractLinear(f1, tf2());
		tf2.clear();
	}
	
	void contractLinear(Field<vector2>& f1, const UList<diagTensor2>& f2)
	{
		checkFields(f1, f2, "contractLinear" "(f1,f2)");
		List_ACCESS(vector2, f1, f1P);
		List_CONST_ACCESS(diagTensor2, f2, f2P);
		List_FOR_ALL(f1,i)
			contractLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractLinear(Field<vector2>& f1, const tmp<Field<diagTensor2>>& tf2)
	{
		contractLinear(f1, tf2());
		tf2.clear();
	}
	
	void contractLinear(Field<vector2>& f1, const UList<sphericalTensor2>& f2)
	{
		checkFields(f1, f2, "contractLinear" "(f1,f2)");
		List_ACCESS(vector2, f1, f1P);
		List_CONST_ACCESS(sphericalTensor2, f2, f2P);
		List_FOR_ALL(f1,i)
			contractLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractLinear(Field<vector2>& f1, const tmp<Field<sphericalTensor2>>& tf2)
	{
		contractLinear(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<vector2>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(vector2, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<vector2>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<tensor2>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(tensor2, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<tensor2>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<diagTensor2>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(diagTensor2, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<diagTensor2>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<sphericalTensor2>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(sphericalTensor2, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<sphericalTensor2>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandLinear(Field<tensor2>& f1, const UList<vector2>& f2)
	{
		checkFields(f1, f2, "expandLinear" "(f1,f2)");
		List_ACCESS(tensor2, f1, f1P);
		List_CONST_ACCESS(vector2, f2, f2P);
		List_FOR_ALL(f1,i)
			expandLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandLinear(Field<tensor2>& f1, const tmp<Field<vector2>>& tf2)
	{
		expandLinear(f1, tf2());
		tf2.clear();
	}
	
	void expandLinear(Field<diagTensor2>& f1, const UList<vector2>& f2)
	{
		checkFields(f1, f2, "expandLinear" "(f1,f2)");
		List_ACCESS(diagTensor2, f1, f1P);
		List_CONST_ACCESS(vector2, f2, f2P);
		List_FOR_ALL(f1,i)
			expandLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandLinear(Field<diagTensor2>& f1, const tmp<Field<vector2>>& tf2)
	{
		expandLinear(f1, tf2());
		tf2.clear();
	}
	
	void expandLinear(Field<sphericalTensor2>& f1, const UList<vector2>& f2)
	{
		checkFields(f1, f2, "expandLinear" "(f1,f2)");
		List_ACCESS(sphericalTensor2, f1, f1P);
		List_CONST_ACCESS(vector2, f2, f2P);
		List_FOR_ALL(f1,i)
			expandLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandLinear(Field<sphericalTensor2>& f1, const tmp<Field<vector2>>& tf2)
	{
		expandLinear(f1, tf2());
		tf2.clear();
	}
	
	void sumToDiag(Field<vector2>& f1, const UList<tensor2>& f2)
	{
		checkFields(f1, f2, "sumToDiag" "(f1,f2)");
		List_ACCESS(vector2, f1, f1P);
		List_CONST_ACCESS(tensor2, f2, f2P);
		List_FOR_ALL(f1,i)
			sumToDiag(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void sumToDiag(Field<vector2>& f1, const tmp<Field<tensor2>>& tf2)
	{
		sumToDiag(f1, tf2());
		tf2.clear();
	}
	
	void sumMagToDiag(Field<vector2>& f1, const UList<tensor2>& f2)
	{
		checkFields(f1, f2, "sumMagToDiag" "(f1,f2)");
		List_ACCESS(vector2, f1, f1P);
		List_CONST_ACCESS(tensor2, f2, f2P);
		List_FOR_ALL(f1,i)
			sumMagToDiag(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void sumMagToDiag(Field<vector2>& f1, const tmp<Field<tensor2>>& tf2)
	{
		sumMagToDiag(f1, tf2());
		tf2.clear();
	}

	void contractScalar(Field<scalar>& f1, const UList<tensor3>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(tensor3, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<tensor3>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractScalar(Field<scalar>& f1, const UList<diagTensor3>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(diagTensor3, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<diagTensor3>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractScalar(Field<scalar>& f1, const UList<sphericalTensor3>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(sphericalTensor3, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<sphericalTensor3>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractScalar(Field<scalar>& f1, const UList<vector3>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(vector3, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<vector3>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractLinear(Field<vector3>& f1, const UList<tensor3>& f2)
	{
		checkFields(f1, f2, "contractLinear" "(f1,f2)");
		List_ACCESS(vector3, f1, f1P);
		List_CONST_ACCESS(tensor3, f2, f2P);
		List_FOR_ALL(f1,i)
			contractLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractLinear(Field<vector3>& f1, const tmp<Field<tensor3>>& tf2)
	{
		contractLinear(f1, tf2());
		tf2.clear();
	}
	
	void contractLinear(Field<vector3>& f1, const UList<diagTensor3>& f2)
	{
		checkFields(f1, f2, "contractLinear" "(f1,f2)");
		List_ACCESS(vector3, f1, f1P);
		List_CONST_ACCESS(diagTensor3, f2, f2P);
		List_FOR_ALL(f1,i)
			contractLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractLinear(Field<vector3>& f1, const tmp<Field<diagTensor3>>& tf2)
	{
		contractLinear(f1, tf2());
		tf2.clear();
	}
	
	void contractLinear(Field<vector3>& f1, const UList<sphericalTensor3>& f2)
	{
		checkFields(f1, f2, "contractLinear" "(f1,f2)");
		List_ACCESS(vector3, f1, f1P);
		List_CONST_ACCESS(sphericalTensor3, f2, f2P);
		List_FOR_ALL(f1,i)
			contractLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractLinear(Field<vector3>& f1, const tmp<Field<sphericalTensor3>>& tf2)
	{
		contractLinear(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<vector3>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(vector3, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<vector3>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<tensor3>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(tensor3, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<tensor3>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<diagTensor3>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(diagTensor3, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<diagTensor3>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<sphericalTensor3>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(sphericalTensor3, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<sphericalTensor3>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandLinear(Field<tensor3>& f1, const UList<vector3>& f2)
	{
		checkFields(f1, f2, "expandLinear" "(f1,f2)");
		List_ACCESS(tensor3, f1, f1P);
		List_CONST_ACCESS(vector3, f2, f2P);
		List_FOR_ALL(f1,i)
			expandLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandLinear(Field<tensor3>& f1, const tmp<Field<vector3>>& tf2)
	{
		expandLinear(f1, tf2());
		tf2.clear();
	}
	
	void expandLinear(Field<diagTensor3>& f1, const UList<vector3>& f2)
	{
		checkFields(f1, f2, "expandLinear" "(f1,f2)");
		List_ACCESS(diagTensor3, f1, f1P);
		List_CONST_ACCESS(vector3, f2, f2P);
		List_FOR_ALL(f1,i)
			expandLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandLinear(Field<diagTensor3>& f1, const tmp<Field<vector3>>& tf2)
	{
		expandLinear(f1, tf2());
		tf2.clear();
	}
	
	void expandLinear(Field<sphericalTensor3>& f1, const UList<vector3>& f2)
	{
		checkFields(f1, f2, "expandLinear" "(f1,f2)");
		List_ACCESS(sphericalTensor3, f1, f1P);
		List_CONST_ACCESS(vector3, f2, f2P);
		List_FOR_ALL(f1,i)
			expandLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandLinear(Field<sphericalTensor3>& f1, const tmp<Field<vector3>>& tf2)
	{
		expandLinear(f1, tf2());
		tf2.clear();
	}
	
	void sumToDiag(Field<vector3>& f1, const UList<tensor3>& f2)
	{
		checkFields(f1, f2, "sumToDiag" "(f1,f2)");
		List_ACCESS(vector3, f1, f1P);
		List_CONST_ACCESS(tensor3, f2, f2P);
		List_FOR_ALL(f1,i)
			sumToDiag(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void sumToDiag(Field<vector3>& f1, const tmp<Field<tensor3>>& tf2)
	{
		sumToDiag(f1, tf2());
		tf2.clear();
	}
	
	void sumMagToDiag(Field<vector3>& f1, const UList<tensor3>& f2)
	{
		checkFields(f1, f2, "sumMagToDiag" "(f1,f2)");
		List_ACCESS(vector3, f1, f1P);
		List_CONST_ACCESS(tensor3, f2, f2P);
		List_FOR_ALL(f1,i)
			sumMagToDiag(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void sumMagToDiag(Field<vector3>& f1, const tmp<Field<tensor3>>& tf2)
	{
		sumMagToDiag(f1, tf2());
		tf2.clear();
	} 
	
	void contractScalar(Field<scalar>& f1, const UList<tensor4>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(tensor4, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<tensor4>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractScalar(Field<scalar>& f1, const UList<diagTensor4>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(diagTensor4, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<diagTensor4>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractScalar(Field<scalar>& f1, const UList<sphericalTensor4>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(sphericalTensor4, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<sphericalTensor4>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractScalar(Field<scalar>& f1, const UList<vector4>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(vector4, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<vector4>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractLinear(Field<vector4>& f1, const UList<tensor4>& f2)
	{
		checkFields(f1, f2, "contractLinear" "(f1,f2)");
		List_ACCESS(vector4, f1, f1P);
		List_CONST_ACCESS(tensor4, f2, f2P);
		List_FOR_ALL(f1,i)
			contractLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractLinear(Field<vector4>& f1, const tmp<Field<tensor4>>& tf2)
	{
		contractLinear(f1, tf2());
		tf2.clear();
	}
	
	void contractLinear(Field<vector4>& f1, const UList<diagTensor4>& f2)
	{
		checkFields(f1, f2, "contractLinear" "(f1,f2)");
		List_ACCESS(vector4, f1, f1P);
		List_CONST_ACCESS(diagTensor4, f2, f2P);
		List_FOR_ALL(f1,i)
			contractLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractLinear(Field<vector4>& f1, const tmp<Field<diagTensor4>>& tf2)
	{
		contractLinear(f1, tf2());
		tf2.clear();
	}
	
	void contractLinear(Field<vector4>& f1, const UList<sphericalTensor4>& f2)
	{
		checkFields(f1, f2, "contractLinear" "(f1,f2)");
		List_ACCESS(vector4, f1, f1P);
		List_CONST_ACCESS(sphericalTensor4, f2, f2P);
		List_FOR_ALL(f1,i)
			contractLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractLinear(Field<vector4>& f1, const tmp<Field<sphericalTensor4>>& tf2)
	{
		contractLinear(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<vector4>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(vector4, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<vector4>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<tensor4>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(tensor4, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<tensor4>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<diagTensor4>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(diagTensor4, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<diagTensor4>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<sphericalTensor4>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(sphericalTensor4, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<sphericalTensor4>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandLinear(Field<tensor4>& f1, const UList<vector4>& f2)
	{
		checkFields(f1, f2, "expandLinear" "(f1,f2)");
		List_ACCESS(tensor4, f1, f1P);
		List_CONST_ACCESS(vector4, f2, f2P);
		List_FOR_ALL(f1,i)
			expandLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandLinear(Field<tensor4>& f1, const tmp<Field<vector4>>& tf2)
	{
		expandLinear(f1, tf2());
		tf2.clear();
	}
	
	void expandLinear(Field<diagTensor4>& f1, const UList<vector4>& f2)
	{
		checkFields(f1, f2, "expandLinear" "(f1,f2)");
		List_ACCESS(diagTensor4, f1, f1P);
		List_CONST_ACCESS(vector4, f2, f2P);
		List_FOR_ALL(f1,i)
			expandLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandLinear(Field<diagTensor4>& f1, const tmp<Field<vector4>>& tf2)
	{
		expandLinear(f1, tf2());
		tf2.clear();
	}
	
	void expandLinear(Field<sphericalTensor4>& f1, const UList<vector4>& f2)
	{
		checkFields(f1, f2, "expandLinear" "(f1,f2)");
		List_ACCESS(sphericalTensor4, f1, f1P);
		List_CONST_ACCESS(vector4, f2, f2P);
		List_FOR_ALL(f1,i)
			expandLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandLinear(Field<sphericalTensor4>& f1, const tmp<Field<vector4>>& tf2)
	{
		expandLinear(f1, tf2());
		tf2.clear();
	}
	
	void sumToDiag(Field<vector4>& f1, const UList<tensor4>& f2)
	{
		checkFields(f1, f2, "sumToDiag" "(f1,f2)");
		List_ACCESS(vector4, f1, f1P);
		List_CONST_ACCESS(tensor4, f2, f2P);
		List_FOR_ALL(f1,i)
			sumToDiag(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void sumToDiag(Field<vector4>& f1, const tmp<Field<tensor4>>& tf2)
	{
		sumToDiag(f1, tf2());
		tf2.clear();
	}
	
	void sumMagToDiag(Field<vector4>& f1, const UList<tensor4>& f2)
	{
		checkFields(f1, f2, "sumMagToDiag" "(f1,f2)");
		List_ACCESS(vector4, f1, f1P);
		List_CONST_ACCESS(tensor4, f2, f2P);
		List_FOR_ALL(f1,i)
			sumMagToDiag(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void sumMagToDiag(Field<vector4>& f1, const tmp<Field<tensor4>>& tf2)
	{
		sumMagToDiag(f1, tf2());
		tf2.clear();
	} 
	
	void contractScalar(Field<scalar>& f1, const UList<tensor6>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(tensor6, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<tensor6>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractScalar(Field<scalar>& f1, const UList<diagTensor6>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(diagTensor6, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<diagTensor6>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractScalar(Field<scalar>& f1, const UList<sphericalTensor6>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(sphericalTensor6, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<sphericalTensor6>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractScalar(Field<scalar>& f1, const UList<vector6>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(vector6, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<vector6>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractLinear(Field<vector6>& f1, const UList<tensor6>& f2)
	{
		checkFields(f1, f2, "contractLinear" "(f1,f2)");
		List_ACCESS(vector6, f1, f1P);
		List_CONST_ACCESS(tensor6, f2, f2P);
		List_FOR_ALL(f1,i)
			contractLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractLinear(Field<vector6>& f1, const tmp<Field<tensor6>>& tf2)
	{
		contractLinear(f1, tf2());
		tf2.clear();
	}
	
	void contractLinear(Field<vector6>& f1, const UList<diagTensor6>& f2)
	{
		checkFields(f1, f2, "contractLinear" "(f1,f2)");
		List_ACCESS(vector6, f1, f1P);
		List_CONST_ACCESS(diagTensor6, f2, f2P);
		List_FOR_ALL(f1,i)
			contractLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractLinear(Field<vector6>& f1, const tmp<Field<diagTensor6>>& tf2)
	{
		contractLinear(f1, tf2());
		tf2.clear();
	}
	
	void contractLinear(Field<vector6>& f1, const UList<sphericalTensor6>& f2)
	{
		checkFields(f1, f2, "contractLinear" "(f1,f2)");
		List_ACCESS(vector6, f1, f1P);
		List_CONST_ACCESS(sphericalTensor6, f2, f2P);
		List_FOR_ALL(f1,i)
			contractLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractLinear(Field<vector6>& f1, const tmp<Field<sphericalTensor6>>& tf2)
	{
		contractLinear(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<vector6>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(vector6, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<vector6>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<tensor6>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(tensor6, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<tensor6>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<diagTensor6>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(diagTensor6, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<diagTensor6>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<sphericalTensor6>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(sphericalTensor6, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<sphericalTensor6>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandLinear(Field<tensor6>& f1, const UList<vector6>& f2)
	{
		checkFields(f1, f2, "expandLinear" "(f1,f2)");
		List_ACCESS(tensor6, f1, f1P);
		List_CONST_ACCESS(vector6, f2, f2P);
		List_FOR_ALL(f1,i)
			expandLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandLinear(Field<tensor6>& f1, const tmp<Field<vector6>>& tf2)
	{
		expandLinear(f1, tf2());
		tf2.clear();
	}
	
	void expandLinear(Field<diagTensor6>& f1, const UList<vector6>& f2)
	{
		checkFields(f1, f2, "expandLinear" "(f1,f2)");
		List_ACCESS(diagTensor6, f1, f1P);
		List_CONST_ACCESS(vector6, f2, f2P);
		List_FOR_ALL(f1,i)
			expandLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandLinear(Field<diagTensor6>& f1, const tmp<Field<vector6>>& tf2)
	{
		expandLinear(f1, tf2());
		tf2.clear();
	}
	
	void expandLinear(Field<sphericalTensor6>& f1, const UList<vector6>& f2)
	{
		checkFields(f1, f2, "expandLinear" "(f1,f2)");
		List_ACCESS(sphericalTensor6, f1, f1P);
		List_CONST_ACCESS(vector6, f2, f2P);
		List_FOR_ALL(f1,i)
			expandLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandLinear(Field<sphericalTensor6>& f1, const tmp<Field<vector6>>& tf2)
	{
		expandLinear(f1, tf2());
		tf2.clear();
	}
	
	void sumToDiag(Field<vector6>& f1, const UList<tensor6>& f2)
	{
		checkFields(f1, f2, "sumToDiag" "(f1,f2)");
		List_ACCESS(vector6, f1, f1P);
		List_CONST_ACCESS(tensor6, f2, f2P);
		List_FOR_ALL(f1,i)
			sumToDiag(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void sumToDiag(Field<vector6>& f1, const tmp<Field<tensor6>>& tf2)
	{
		sumToDiag(f1, tf2());
		tf2.clear();
	}
	
	void sumMagToDiag(Field<vector6>& f1, const UList<tensor6>& f2)
	{
		checkFields(f1, f2, "sumMagToDiag" "(f1,f2)");
		List_ACCESS(vector6, f1, f1P);
		List_CONST_ACCESS(tensor6, f2, f2P);
		List_FOR_ALL(f1,i)
			sumMagToDiag(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void sumMagToDiag(Field<vector6>& f1, const tmp<Field<tensor6>>& tf2)
	{
		sumMagToDiag(f1, tf2());
		tf2.clear();
	} 
	
	void contractScalar(Field<scalar>& f1, const UList<tensor8>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(tensor8, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<tensor8>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractScalar(Field<scalar>& f1, const UList<diagTensor8>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(diagTensor8, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<diagTensor8>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractScalar(Field<scalar>& f1, const UList<sphericalTensor8>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(sphericalTensor8, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<sphericalTensor8>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractScalar(Field<scalar>& f1, const UList<vector8>& f2)
	{
		checkFields(f1, f2, "contractScalar" "(f1,f2)");
		List_ACCESS(scalar, f1, f1P);
		List_CONST_ACCESS(vector8, f2, f2P);
		List_FOR_ALL(f1,i)
			contractScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractScalar(Field<scalar>& f1, const tmp<Field<vector8>>& tf2)
	{
		contractScalar(f1, tf2());
		tf2.clear();
	}
	
	void contractLinear(Field<vector8>& f1, const UList<tensor8>& f2)
	{
		checkFields(f1, f2, "contractLinear" "(f1,f2)");
		List_ACCESS(vector8, f1, f1P);
		List_CONST_ACCESS(tensor8, f2, f2P);
		List_FOR_ALL(f1,i)
			contractLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractLinear(Field<vector8>& f1, const tmp<Field<tensor8>>& tf2)
	{
		contractLinear(f1, tf2());
		tf2.clear();
	}
	
	void contractLinear(Field<vector8>& f1, const UList<diagTensor8>& f2)
	{
		checkFields(f1, f2, "contractLinear" "(f1,f2)");
		List_ACCESS(vector8, f1, f1P);
		List_CONST_ACCESS(diagTensor8, f2, f2P);
		List_FOR_ALL(f1,i)
			contractLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractLinear(Field<vector8>& f1, const tmp<Field<diagTensor8>>& tf2)
	{
		contractLinear(f1, tf2());
		tf2.clear();
	}
	
	void contractLinear(Field<vector8>& f1, const UList<sphericalTensor8>& f2)
	{
		checkFields(f1, f2, "contractLinear" "(f1,f2)");
		List_ACCESS(vector8, f1, f1P);
		List_CONST_ACCESS(sphericalTensor8, f2, f2P);
		List_FOR_ALL(f1,i)
			contractLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void contractLinear(Field<vector8>& f1, const tmp<Field<sphericalTensor8>>& tf2)
	{
		contractLinear(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<vector8>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(vector8, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<vector8>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<tensor8>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(tensor8, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<tensor8>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<diagTensor8>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(diagTensor8, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<diagTensor8>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandScalar(Field<sphericalTensor8>& f1, const UList<scalar>& f2)
	{
		checkFields(f1, f2, "expandScalar" "(f1,f2)");
		List_ACCESS(sphericalTensor8, f1, f1P);
		List_CONST_ACCESS(scalar, f2, f2P);
		List_FOR_ALL(f1,i)
			expandScalar(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandScalar(Field<sphericalTensor8>& f1, const tmp<Field<scalar>>& tf2)
	{
		expandScalar(f1, tf2());
		tf2.clear();
	}
	
	void expandLinear(Field<tensor8>& f1, const UList<vector8>& f2)
	{
		checkFields(f1, f2, "expandLinear" "(f1,f2)");
		List_ACCESS(tensor8, f1, f1P);
		List_CONST_ACCESS(vector8, f2, f2P);
		List_FOR_ALL(f1,i)
			expandLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandLinear(Field<tensor8>& f1, const tmp<Field<vector8>>& tf2)
	{
		expandLinear(f1, tf2());
		tf2.clear();
	}
	
	void expandLinear(Field<diagTensor8>& f1, const UList<vector8>& f2)
	{
		checkFields(f1, f2, "expandLinear" "(f1,f2)");
		List_ACCESS(diagTensor8, f1, f1P);
		List_CONST_ACCESS(vector8, f2, f2P);
		List_FOR_ALL(f1,i)
			expandLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandLinear(Field<diagTensor8>& f1, const tmp<Field<vector8>>& tf2)
	{
		expandLinear(f1, tf2());
		tf2.clear();
	}
	
	void expandLinear(Field<sphericalTensor8>& f1, const UList<vector8>& f2)
	{
		checkFields(f1, f2, "expandLinear" "(f1,f2)");
		List_ACCESS(sphericalTensor8, f1, f1P);
		List_CONST_ACCESS(vector8, f2, f2P);
		List_FOR_ALL(f1,i)
			expandLinear(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void expandLinear(Field<sphericalTensor8>& f1, const tmp<Field<vector8>>& tf2)
	{
		expandLinear(f1, tf2());
		tf2.clear();
	}
	
	void sumToDiag(Field<vector8>& f1, const UList<tensor8>& f2)
	{
		checkFields(f1, f2, "sumToDiag" "(f1,f2)");
		List_ACCESS(vector8, f1, f1P);
		List_CONST_ACCESS(tensor8, f2, f2P);
		List_FOR_ALL(f1,i)
			sumToDiag(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void sumToDiag(Field<vector8>& f1, const tmp<Field<tensor8>>& tf2)
	{
		sumToDiag(f1, tf2());
		tf2.clear();
	}
	
	void sumMagToDiag(Field<vector8>& f1, const UList<tensor8>& f2)
	{
		checkFields(f1, f2, "sumMagToDiag" "(f1,f2)");
		List_ACCESS(vector8, f1, f1P);
		List_CONST_ACCESS(tensor8, f2, f2P);
		List_FOR_ALL(f1,i)
			sumMagToDiag(List_ELEM(f1, f1P, i), List_ELEM(f2, f2P, i));
		List_END_FOR_ALL
	}

	void sumMagToDiag(Field<vector8>& f1, const tmp<Field<tensor8>>& tf2)
	{
		sumMagToDiag(f1, tf2());
		tf2.clear();
	}
}

#undef UNARY_FUNCTION
#undef ExpandFieldFunctions