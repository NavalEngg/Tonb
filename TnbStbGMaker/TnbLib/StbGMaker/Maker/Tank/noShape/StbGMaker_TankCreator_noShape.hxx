#pragma once
#ifndef _StbGMaker_TankCreator_noShape_Header
#define _StbGMaker_TankCreator_noShape_Header

#include <StbGMaker_TankCreator.hxx>

namespace tnbLib
{

	namespace stbGmakerLib
	{

		class TankCreator_noShape
			: public StbGMaker_TankCreator
		{

			/*Private Data*/

		public:

			TankCreator_noShape()
			{}

			TankCreator_noShape
			(
				const Standard_Integer theIndex, 
				const word& theName
			);


			Standard_Boolean HasShape() const override
			{
				return Standard_False;
			}

			Standard_Integer
				CreateWorkingPlane
				(
					const Standard_Real x
				) override;

			void ExportAs
			(
				std::shared_ptr<Marine_Model>& theModel
			) const override;
		};
	}
}

#endif // !_StbGMaker_TankCreator_noShape_Header
