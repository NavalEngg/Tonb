#pragma once
#ifndef _PtdModel_BladeFormNo1_Chord_Header
#define _PtdModel_BladeFormNo1_Chord_Header

#include <PtdModel_BladeFormNo1.hxx>

namespace tnbLib
{

	namespace ptdModel
	{

		class BladeFormNo1_Chord
			: public PtdModel_BladeFormNo1
		{

		public:

			enum class Parameters
			{
				rootChord,
				tipChord,
				maxChord,
				maxChordLoc,
				rootWeight,
				tipWeight
			};

			static TnbPtdModel_EXPORT word ROOT_CHORD;
			static TnbPtdModel_EXPORT word TIP_CHORD;
			static TnbPtdModel_EXPORT word MAX_CHORD;
			static TnbPtdModel_EXPORT word MAX_CHORD_LOCATION;
			static TnbPtdModel_EXPORT word ROOT_WEIGHT;
			static TnbPtdModel_EXPORT word TIP_WEIGHT;

		private:

			/*Private Data*/

			word theParameters_[6];


			//- private functions and operators

			TnbPtdModel_EXPORT void Init();

		public:

			//- default constructor

			BladeFormNo1_Chord()
			{
				Init();
			}

			//- constructors


			//- public functions and operators

			TnbPtdModel_EXPORT Standard_Integer NbParameters() const override;

			TnbPtdModel_EXPORT word Parameter(const Standard_Integer theIndex) const override;
			inline word Parameter(const Parameters) const;

			TnbPtdModel_EXPORT std::shared_ptr<PtdModel_Form> CreateForm() const override;
			std::shared_ptr<PtdModel_BladeProfile>
				CreateProfile
				(
					const std::shared_ptr<PtdModel_BladeGlobalPars>&,
					const std::shared_ptr<PtdModel_Form>&
				) const override;
		};
	}
}

#include <PtdModel_BladeFormNo1_ChordI.hxx>

#endif // !_PtdModel_BladeFormNo1_Chord_Header
