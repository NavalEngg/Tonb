#pragma once
#ifndef _Cad_ApproxMetricCriterion_Header
#define _Cad_ApproxMetricCriterion_Header

#include <Cad_Module.hxx>
#include <Standard_Handle.hxx>
#include <Standard_TypeDef.hxx>
#include <Entity2d_BoxFwd.hxx>

#include <memory>

// Forward Declarations [12/26/2021 Amir]
class Geom_Surface;

namespace tnbLib
{

	// Forward Declarations [12/26/2021 Amir]
	class Geo2d_SamplePoints;

	class Cad_ApprxMetricCriterion
	{

		/*Private Data*/

		Standard_Real theTolerance_;

	protected:

		// default constructor [12/26/2021 Amir]

		Cad_ApprxMetricCriterion()
			: theTolerance_(DEFAULT_TOLERANCE)
		{}

		// constructors [12/26/2021 Amir]

		Cad_ApprxMetricCriterion
		(
			const Standard_Real theTol
		)
			: theTolerance_(theTol)
		{}

	public:


		static TnbCad_EXPORT const Standard_Real DEFAULT_TOLERANCE;

		// public functions and operators [12/26/2021 Amir]

		auto Tolerance() const
		{
			return theTolerance_;
		}

		void SetTolerance(const Standard_Real theTol)
		{
			theTolerance_ = theTol;
		}

		virtual Standard_Boolean Subdivide
		(
			const Entity2d_Box& theDomain,
			const std::shared_ptr<Geo2d_SamplePoints>& theSamples,
			const Handle(Geom_Surface)& theGeometry
		) const = 0;
	};
}

#endif // !_Cad_ApproxMetricCriterion_Header
