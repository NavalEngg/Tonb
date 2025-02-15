#pragma once
#ifndef _TModel_Wire_Header
#define _TModel_Wire_Header

#include <TModel_Entity.hxx>
#include <Entity2d_BoxFwd.hxx>

namespace tnbLib
{

	// Forward Declarations
	class TModel_CmpEdge;

	class TModel_Wire
		: public TModel_Entity
	{

		/*Private Data*/

		std::shared_ptr<TModel_CmpEdge> theEdge_;


		//- private functions and operators

		TNB_SERIALIZATION(TnbCad_EXPORT);

	protected:

		//- default constructors

		TModel_Wire()
		{}

	public:


		//- constructors

		TnbCad_EXPORT explicit TModel_Wire
		(
			const std::shared_ptr<TModel_CmpEdge>& theEdge
		);

		TnbCad_EXPORT TModel_Wire
		(
			const Standard_Integer theIndex,
			const std::shared_ptr<TModel_CmpEdge>& theEdge
		);

		TnbCad_EXPORT TModel_Wire
		(
			const Standard_Integer theIndex,
			const word& theName,
			const std::shared_ptr<TModel_CmpEdge>& theEdge
		);



		//- public functions and operators


		const auto& CmpEdge() const
		{
			return theEdge_;
		}

		TnbCad_EXPORT Entity2d_Box CalcParametricBoundingBox() const;
	};
}


BOOST_CLASS_EXPORT_KEY(tnbLib::TModel_Wire);

#endif // !_TModel_Wire_Header
