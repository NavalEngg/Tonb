#pragma once
#ifndef _Mesh_FeatureSizeMapControl_Header
#define _Mesh_FeatureSizeMapControl_Header

#include <Mesh_SizeMapControl.hxx>

namespace tnbLib
{

	template<class GeomType>
	class Mesh_FeatureSizeMapControl
		: public Mesh_SizeMapControl<GeomType>
	{

	public:

		typedef Mesh_SizeMapControl<GeomType> base;
		using base::sizeMapTool;
		using base::meshRefValuesType;

	private:

		/*Private Data*/


		//- private functions and operators

		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int /*file_version*/)
		{
			ar & boost::serialization::base_object<Mesh_SizeMapControl<GeomType>>(*this);
		}

	protected:


		

	public:

		//- default constructor
		Mesh_FeatureSizeMapControl()
		{}


		//- constructors

		Mesh_FeatureSizeMapControl
		(
			const std::shared_ptr<GeomType>& theGeometry,
			const std::shared_ptr<meshRefValuesType>& theRef
		)
			: base(theGeometry, theRef)
		{}


		//- public functions and operators


	};
}

#endif // !_Mesh_FeatureSizeMapControl_Header
