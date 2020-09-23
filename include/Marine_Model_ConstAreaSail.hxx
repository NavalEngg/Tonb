#pragma once
#ifndef _Marine_Model_ConstAreaSail_Header
#define _Marine_Model_ConstAreaSail_Header

#include <Marine_AreaSailModel.hxx>

namespace tnbLib
{

	namespace marineLib
	{

		class Model_ConstAreaSail
			: public Marine_AreaSailModel
		{

			/*Private Data*/

			Standard_Real theArea_;
			Standard_Real theZbar_;

		public:

			Model_ConstAreaSail
			(
				const Standard_Real theArea, 
				const Standard_Real theZbar
			);

			Model_ConstAreaSail
			(
				const Standard_Integer theIndex,
				const Standard_Real theArea, 
				const Standard_Real theZbar
			);

			Model_ConstAreaSail
			(
				const Standard_Integer theIndex,
				const word& theName, 
				const Standard_Real theArea,
				const Standard_Real theZbar
			);


			Marine_SailModelType SailType() const override
			{
				return Marine_SailModelType::constant_area;
			}

			auto Area() const
			{
				return theArea_;
			}

			auto zBar() const
			{
				return theZbar_;
			}
		};
	}
}

#endif // !_Marine_Model_ConstAreaSail_Header
