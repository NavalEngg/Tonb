#include <Aft2d_tSegmentNode.hxx>

#include <Aft2d_tPlnCurveSurface.hxx>

typedef tnbLib::Mesh_BndNodeGeomAdaptor<tnbLib::Aft2d_tPlnCurveSurface, void, tnbLib::Mesh_BndNode_Position_Segment>
bndNodeGeomAdaptor;

template<>
TNB_SAVE_IMPLEMENTATION(bndNodeGeomAdaptor)
{
	ar & theCurve_;
}

template<>
TNB_LOAD_IMPLEMENTATION(bndNodeGeomAdaptor)
{
	ar & theCurve_;
}

BOOST_CLASS_EXPORT_IMPLEMENT(tnbLib::Aft2d_tSegmentNode);