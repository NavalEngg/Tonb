#pragma once
#ifndef _Mesh_CurveOptmPoint_Correction_Info_Header
#define _Mesh_CurveOptmPoint_Correction_Info_Header

#include <Standard_TypeDef.hxx>
#include <Global_AccessMethod.hxx>
#include <Global_Serialization.hxx>
#include <Mesh_Module.hxx>

namespace tnbLib
{

	class Mesh_CurveOptmPoint_Correction_Info
	{

		/*Private Data*/

		Standard_Real theTolerance_;
		Standard_Real theUnderRelaxation_;

		Standard_Integer theMaxLevel_;


		//- Private functions and operators

		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int /*file_version*/)
		{
			ar & theTolerance_;
			ar & theUnderRelaxation_;
			ar & theMaxLevel_;
		}

	public:

		static TnbMesh_EXPORT const Standard_Real DEFAULT_TOLERANCE;
		static TnbMesh_EXPORT const Standard_Real DEFAULT_UNDER_RELAXATION;

		static TnbMesh_EXPORT const Standard_Integer DEFAULT_MAX_LEVEL;


		//- default constructor

		Mesh_CurveOptmPoint_Correction_Info()
			: theTolerance_(DEFAULT_TOLERANCE)
			, theUnderRelaxation_(DEFAULT_UNDER_RELAXATION)
			, theMaxLevel_(DEFAULT_MAX_LEVEL)
		{}


		//- constructors


		//- public functions and perators


		//- macros

		GLOBAL_ACCESS_PRIM_SINGLE(Standard_Real, Tolerance)
			GLOBAL_ACCESS_PRIM_SINGLE(Standard_Real, UnderRelaxation)
			GLOBAL_ACCESS_PRIM_SINGLE(Standard_Integer, MaxLevel)
	};
}

#endif // !_Mesh_CurveOptmPoint_Correction_Info_Header
