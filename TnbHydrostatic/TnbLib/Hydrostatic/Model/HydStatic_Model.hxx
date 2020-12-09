#pragma once
#ifndef _HydStatic_Model_Header
#define _HydStatic_Model_Header

#include <HydStatic_Entity.hxx>
#include <Marine_BodiesFwd.hxx>

#include <map>

namespace tnbLib
{

	// Forward Declarations
	class StbGMaker_Model;
	class Marine_Domain;
	class Marine_BaseLine;
	class Marine_Wave;
	class Marine_Water;
	class HydStatic_FloatBody;

	class HydStatic_Model
		: public HydStatic_Entity
	{

		/*Private Data*/

		std::shared_ptr<StbGMaker_Model> theStbModel_;
		std::shared_ptr<Marine_Wave> theWave_;


		std::shared_ptr<Marine_Domain> theDomain_;
		std::shared_ptr<Marine_Water> theWater_;

		std::shared_ptr<HydStatic_FloatBody> theFloatBody_;


		TnbHydStatic_EXPORT void CalcDomain();

		TnbHydStatic_EXPORT void CalcWater();

		TnbHydStatic_EXPORT void CalcFloatBody();


		static TnbHydStatic_EXPORT std::shared_ptr<marineLib::Body_Displacer> RetrieveDisplacer(const std::shared_ptr<StbGMaker_Model>& theModel, const char* name);

	public:

		static TnbHydStatic_EXPORT unsigned short verbose;

		HydStatic_Model()
		{}

		TnbHydStatic_EXPORT HydStatic_Model
		(
			const Standard_Integer theIndex, 
			const word& theName
		);


		//- public functions and operators

		const auto& StbModel() const
		{
			return theStbModel_;
		}

		const auto& Wave() const
		{
			return theWave_;
		}

		const auto& Domain() const
		{
			return theDomain_;
		}

		const auto& Water() const
		{
			return theWater_;
		}

		const auto& FloatBody() const
		{
			return theFloatBody_;
		}

		TnbHydStatic_EXPORT const Marine_BaseLine& BaseLine() const;

		TnbHydStatic_EXPORT void Perform();

		void LoadModel(const std::shared_ptr<StbGMaker_Model>& theModel)
		{
			theStbModel_ = theModel;
		}

		void LoadWave(const std::shared_ptr<Marine_Wave>& theWave)
		{
			theWave_ = theWave;
		}


	};
}

#endif // !_HydStatic_Model_Header
