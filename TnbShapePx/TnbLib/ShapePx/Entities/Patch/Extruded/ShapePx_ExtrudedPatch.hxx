#pragma once
#ifndef _ShapePx_ExtrudedPatch_Header
#define _ShapePx_ExtrudedPatch_Header

#include <ShapePx_Patch.hxx>

#include <map>

namespace tnbLib
{

	// Forward Declarations
	class ShapePx_Profile;

	class ShapePx_ExtrudedPatch
		: public ShapePx_Patch
	{

		/*Private Data*/

		std::map
			<
			Standard_Integer, 
			std::shared_ptr<ShapePx_Profile>
			> theParToProfile_;

	public:


		//- default constructor

		ShapePx_ExtrudedPatch()
		{}


		//- constructors

		ShapePx_ExtrudedPatch
		(
			const Standard_Integer theIndex,
			const word& theName
		);

		ShapePx_ExtrudedPatch
		(
			const std::shared_ptr<ShapePx_Section>& theSect
		);

		ShapePx_ExtrudedPatch
		(
			const Standard_Integer theIndex, 
			const word& theName, 
			const std::shared_ptr<ShapePx_Section>& theSect
		);


		//- public functions and operators

		auto NbSectionParameters() const
		{
			return (Standard_Integer)theParToProfile_.size();
		}

		const auto& SectionParameterToProfileMap() const
		{
			return theParToProfile_;
		}

		std::shared_ptr<ShapePx_Profile> 
			RemoveProfile
			(
				const Standard_Integer theParIndex
			);

		void ImportProfileForParameter
		(
			const Standard_Integer theParIndex, 
			const std::shared_ptr<ShapePx_Profile>& theProfile
		);

		
	};
}

#endif // !_ShapePx_ExtrudedPatch_Header
