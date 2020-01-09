#include <MixingPlaneInterpolationTemplate.hxx>

#include <demandDrivenData.hxx>
#include <PrimitivePatchTemplate.hxx>
#include <IOmanip.hxx>
#include <transform.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

	template<class MasterPatch, class SlavePatch>
	tnbLib::direction
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::sweepAxisSwitch() const
	{
		// SweepAxis switch
		switch (sweepAxisType_)
		{
		case SWEEP_X:
		case SWEEP_R:
		{
			return vector::X;
		}
		break;

		case SWEEP_Y:
		case SWEEP_THETA:
		{
			return vector::Y;
		}
		break;

		case SWEEP_Z:
		{
			return vector::Z;
		}
		break;

		default:
		{
			FatalErrorIn
			(
				"direction MixingPlaneInterpolation<MasterPatch, "
				"SlavePatch>::sweepAxisSwitch() const"
			) << "Bad sweepAxis type: "
				<< MixingPlaneInterpolationName::sweepAxisNames_
				[sweepAxisType_]
			<< "Available types: "
				<< MixingPlaneInterpolationName::sweepAxisNames_
				<< abort(FatalError);

			// Dummy return
			return vector::X;
		}
		}
	}


	template<class MasterPatch, class SlavePatch>
	tnbLib::direction
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::stackAxisSwitch() const
	{
		// stackAxis switch
		switch (stackAxisType_)
		{
		case STACK_X:
		case STACK_R:
		{
			return vector::X;
		}
		break;

		case STACK_Y:
		case STACK_THETA:
		{
			return vector::Y;
		}
		break;

		case STACK_Z:
		{
			return vector::Z;
		}
		break;

		default:
		{
			FatalErrorIn
			(
				"direction MixingPlaneInterpolation<MasterPatch, "
				"SlavePatch>::stackAxisSwitch() const"
			) << "Bad stackAxis type: "
				<< MixingPlaneInterpolationName::stackAxisNames_
				[stackAxisType_]
			<< "Available types: "
				<< MixingPlaneInterpolationName::stackAxisNames_
				<< abort(FatalError);

			// Dummy return
			return vector::X;
		}
		}
	}


	template<class MasterPatch, class SlavePatch>
	void MixingPlaneInterpolation<MasterPatch, SlavePatch>::clearOut()
	{
		clearTransfomedPatches();
		clearMixingPlanePatch();

		clearAddressing();
		clearTransforms();
	}


	// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

	template<class MasterPatch, class SlavePatch>
	MixingPlaneInterpolation<MasterPatch, SlavePatch>::
		MixingPlaneInterpolation
		(
			const MasterPatch& masterPatch,
			const SlavePatch& slavePatch,
			const coordinateSystem& cs,
			const MixingPlaneInterpolationName::discretisation& discretisationType,
			const MixingPlaneInterpolationName::sweepAxis& sweepAxisType,
			const MixingPlaneInterpolationName::stackAxis& stackAxisType,
			const pointField& interpolationProfile
		)
		:
		masterPatch_(masterPatch),
		slavePatch_(slavePatch),
		cs_(cs),
		discretisationType_(discretisationType),
		sweepAxisType_(sweepAxisType),
		stackAxisType_(stackAxisType),
		interpolationProfile_(interpolationProfile),

		forwardT_(),
		reverseT_(),
		forwardSep_(),

		transformedMasterPatchPtr_(NULL),
		transformedShadowPatchPtr_(NULL),
		mixingPlanePatchPtr_(NULL),

		masterPatchToProfileTPtr_(NULL),
		masterProfileToPatchTPtr_(NULL),
		slavePatchToProfileTPtr_(NULL),
		slaveProfileToPatchTPtr_(NULL),

		masterPatchToProfileAddrPtr_(NULL),
		masterProfileToPatchAddrPtr_(NULL),
		masterPatchToProfileWeightsPtr_(NULL),
		masterProfileToPatchWeightsPtr_(NULL),

		slavePatchToProfileAddrPtr_(NULL),
		slaveProfileToPatchAddrPtr_(NULL),
		slavePatchToProfileWeightsPtr_(NULL),
		slaveProfileToPatchWeightsPtr_(NULL)

	{
		// Check size of transform.  They should be equal to slave patch size
		// if the transform is not constant
		if (forwardT_.size() > 1 || reverseT_.size() > 1)
		{
			if
				(
					forwardT_.size() != slavePatch_.size()
					|| reverseT_.size() != masterPatch_.size()
					)
			{
				FatalErrorIn
				(
					"MixingPlaneInterpolation<MasterPatch, "
					"SlavePatch>::MixingPlaneInterpolation"
				) << "Incorrectly defined transform: forwardT: "
					<< forwardT_.size() << " patch: " << slavePatch_.size()
					<< " reverseT: " << reverseT_.size()
					<< " patch: " << masterPatch_.size()
					<< abort(FatalError);
			}
		}

		masterPatchToProfileAddr();
	}


	// * * * * * * * * * * * * * * * * Destructor * * * * * * * * * * * * * * * //

	template<class MasterPatch, class SlavePatch>
	MixingPlaneInterpolation<MasterPatch, SlavePatch>::~MixingPlaneInterpolation()
	{
		clearOut();
	}


	// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

	template<class MasterPatch, class SlavePatch>
	const tnbLib::standAlonePatch&
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::mixingPlanePatch() const
	{
		if (!mixingPlanePatchPtr_)
		{
			calcMixingPlanePatch();
		}

		return *mixingPlanePatchPtr_;
	}


	template<class MasterPatch, class SlavePatch>
	const tnbLib::standAlonePatch&
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::
		transformedMasterPatch() const
	{
		if (!transformedMasterPatchPtr_)
		{
			calcTransformedPatches();
		}

		return *transformedMasterPatchPtr_;
	}


	template<class MasterPatch, class SlavePatch>
	const tnbLib::standAlonePatch&
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::
		transformedShadowPatch() const
	{
		if (!transformedShadowPatchPtr_)
		{
			calcTransformedPatches();
		}

		return *transformedShadowPatchPtr_;
	}


	template<class MasterPatch, class SlavePatch>
	const labelListList&
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::
		masterPatchToProfileAddr() const
	{
		if (!masterPatchToProfileAddrPtr_)
		{
			calcAddressing();
		}

		return *masterPatchToProfileAddrPtr_;
	}


	template<class MasterPatch, class SlavePatch>
	const labelListList&
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::
		masterProfileToPatchAddr() const
	{
		if (!masterProfileToPatchAddrPtr_)
		{
			calcAddressing();
		}

		return *masterProfileToPatchAddrPtr_;
	}


	template<class MasterPatch, class SlavePatch>
	const scalarListList&
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::
		masterPatchToProfileWeights() const
	{
		if (!masterPatchToProfileWeightsPtr_)
		{
			calcAddressing();
		}

		return *masterPatchToProfileWeightsPtr_;
	}


	template<class MasterPatch, class SlavePatch>
	const scalarListList&
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::
		masterProfileToPatchWeights() const
	{
		if (!masterProfileToPatchWeightsPtr_)
		{
			calcAddressing();
		}

		return *masterProfileToPatchWeightsPtr_;
	}


	template<class MasterPatch, class SlavePatch>
	const labelListList&
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::
		slavePatchToProfileAddr() const
	{
		if (!slavePatchToProfileAddrPtr_)
		{
			calcAddressing();
		}

		return *slavePatchToProfileAddrPtr_;
	}


	template<class MasterPatch, class SlavePatch>
	const labelListList&
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::
		slaveProfileToPatchAddr() const
	{
		if (!slaveProfileToPatchAddrPtr_)
		{
			calcAddressing();
		}

		return *slaveProfileToPatchAddrPtr_;
	}


	template<class MasterPatch, class SlavePatch>
	const scalarListList&
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::
		slavePatchToProfileWeights() const
	{
		if (!slavePatchToProfileWeightsPtr_)
		{
			calcAddressing();
		}

		return *slavePatchToProfileWeightsPtr_;
	}


	template<class MasterPatch, class SlavePatch>
	const scalarListList&
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::
		slaveProfileToPatchWeights() const
	{
		if (!slaveProfileToPatchWeightsPtr_)
		{
			calcAddressing();
		}

		return *slaveProfileToPatchWeightsPtr_;
	}


	template<class MasterPatch, class SlavePatch>
	const tensorField&
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::
		masterPatchToProfileT() const
	{
		if (!masterPatchToProfileTPtr_)
		{
			calcTransforms();
		}

		return *masterPatchToProfileTPtr_;
	}


	template<class MasterPatch, class SlavePatch>
	const tensorField&
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::
		masterProfileToPatchT() const
	{
		if (!masterProfileToPatchTPtr_)
		{
			calcTransforms();
		}

		return *masterProfileToPatchTPtr_;
	}


	template<class MasterPatch, class SlavePatch>
	const tensorField&
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::slavePatchToProfileT() const
	{
		if (!slavePatchToProfileTPtr_)
		{
			calcTransforms();
		}

		return *slavePatchToProfileTPtr_;
	}


	template<class MasterPatch, class SlavePatch>
	const tensorField&
		MixingPlaneInterpolation<MasterPatch, SlavePatch>::slaveProfileToPatchT() const
	{
		if (!slaveProfileToPatchTPtr_)
		{
			calcTransforms();
		}

		return *slaveProfileToPatchTPtr_;
	}


	template<class MasterPatch, class SlavePatch>
	bool MixingPlaneInterpolation<MasterPatch, SlavePatch>::movePoints()
	{
		clearOut();

		return true;
	}


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// ************************************************************************* //