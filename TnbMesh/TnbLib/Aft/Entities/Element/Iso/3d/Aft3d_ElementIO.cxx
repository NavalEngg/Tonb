#include <Aft3d_Element.hxx>

#include <Aft3d_Edge.hxx>
#include <Aft3d_Facet.hxx>

namespace tnbLib
{

	template<>
	template<>
	void Mesh_Element<Aft3d_ElementTraits, Mesh_ElementType_Tetrahedron>::serialize<TNB_iARCH_TYPE>
		(
			TNB_iARCH_TYPE& ar,
			const unsigned int file_vertion
			)
	{
		ar & boost::serialization::base_object<Mesh_Entity>(*this);
		ar & boost::serialization::base_object<adaptor>(*this);
		ar & theNodes_;
		ar & theEdges_;
		ar & theFacets_;
	}

	template<>
	template<>
	void Mesh_Element<Aft3d_ElementTraits, Mesh_ElementType_Tetrahedron>::serialize<TNB_oARCH_TYPE>
		(
			TNB_oARCH_TYPE& ar,
			const unsigned int file_vertion
			)
	{
		ar & boost::serialization::base_object<Mesh_Entity>(*this);
		ar & boost::serialization::base_object<adaptor>(*this);
		ar & theNodes_;
		ar & theEdges_;
		ar & theFacets_;
	}
}

BOOST_CLASS_EXPORT_IMPLEMENT(tnbLib::Aft3d_Element);