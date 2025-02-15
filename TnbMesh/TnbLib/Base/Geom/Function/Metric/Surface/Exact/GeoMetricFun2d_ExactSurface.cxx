#include <GeoMetricFun2d_ExactSurface.hxx>

#include <Entity2d_Metric1.hxx>

#include <Geom_Surface.hxx>

tnbLib::Entity2d_Metric1 
tnbLib::GeoMetricFun2d_ExactSurface::Value
(
	const Pnt2d & theCoord
) const
{
	Debug_Null_Pointer(Geometry());

	gp_Vec D1U, D1V;
	gp_Pnt Pt;

	Geometry()->D1(0, 0, Pt, D1U, D1V);

	Standard_Real A = D1U.Dot(D1U);
	Standard_Real B = D1U.Dot(D1V);
	Standard_Real C = D1V.Dot(D1V);

	Entity2d_Metric1 m(A, B, C);
	return std::move(m);
}