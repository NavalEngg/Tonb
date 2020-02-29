#include <Marine_CmpSection.hxx>

#include <Geo_ItemSort.hxx>
#include <Entity2d_Box.hxx>
#include <Pln_Curve.hxx>
#include <Pln_Wire.hxx>
#include <Pln_Tools.hxx>
#include <Marine_Section.hxx>
#include <error.hxx>
#include <OSstream.hxx>

#include <map>

tnbLib::Marine_CmpSection::Marine_CmpSection()
{
}

tnbLib::Marine_CmpSection::Marine_CmpSection
(
	const Standard_Integer theIndex
)
	: Marine_Entity(theIndex)
{
}

tnbLib::Marine_CmpSection::Marine_CmpSection
(
	const Standard_Integer theIndex,
	const word & theName
)
	: Marine_Entity(theIndex, theName)
{
}

tnbLib::Entity2d_Box 
tnbLib::Marine_CmpSection::BoundingBox() const
{
	auto iter = Sections().begin();
	auto b = (*iter)->BoundingBox();

	iter++;
	while (iter NOT_EQUAL Sections().end())
	{
		b = Entity2d_Box::Union(b, (*iter)->BoundingBox());
		iter++;
	}
	return std::move(b);
}

Standard_Real 
tnbLib::Marine_CmpSection::X() const
{
	return CoordinateSystem().Location().X();
}

std::shared_ptr<tnbLib::Marine_CmpSection> 
tnbLib::Marine_CmpSection::Copy() const
{
	auto copy = std::make_shared<Marine_CmpSection>(Index(), Name());
	Debug_Null_Pointer(copy);

	for (const auto& x : Sections())
	{
		Debug_Null_Pointer(x);
		copy->Insert(x->Copy());
		copy->CoordinateSystem() = CoordinateSystem();
	}
	return std::move(copy);
}

void tnbLib::Marine_CmpSection::Transform
(
	const gp_Trsf2d & t
)
{
	for (const auto& x : Sections())
	{
		Debug_Null_Pointer(x);
		x->Transform(t);
	}
}

void tnbLib::Marine_CmpSection::ExportToPlt
(
	OFstream & File
) const
{
	for (const auto& x : Sections())
	{
		Debug_Null_Pointer(x);
		x->ExportToPlt(File);
	}
}

namespace tnbLib
{

	namespace createCmpSection
	{

		static Standard_Boolean LessDiameterSize
		(
			const std::shared_ptr<Pln_Wire>& theWire0,
			const std::shared_ptr<Pln_Wire>& theWire1
		)
		{
			Debug_Null_Pointer(theWire0);
			Debug_Null_Pointer(theWire1);

			const auto& b0 = theWire0->BoundingBox();
			const auto& b1 = theWire1->BoundingBox();

			Debug_Null_Pointer(b0);
			Debug_Null_Pointer(b1);

			return -b0->SquareDiameter() < -b1->SquareDiameter();
		}

		static void SortWires(std::vector<std::shared_ptr<Pln_Wire>>& theWires)
		{
			if (theWires.size() IS_EQUAL 1)
			{
				return;
			}

			Geo_ItemSort<std::shared_ptr<Pln_Wire>>
				sort(&LessDiameterSize);
			sort.Perform(theWires);
		}

		static void RetrieveInnerOuterWires
		(
			std::list<std::shared_ptr<Pln_Wire>>& theWires,
			std::shared_ptr<Pln_Wire>& theOuter,
			std::vector<std::shared_ptr<Pln_Wire>>& theInners
		)
		{
			if (theWires.size() IS_EQUAL 1)
			{
				theOuter = theWires.front();
				theWires.pop_front();
				return;
			}

			theOuter = theWires.front();
			theWires.pop_front();

			std::vector<std::list<std::shared_ptr<Pln_Wire>>::iterator> removes;
			auto iter = theWires.begin();
			while (iter NOT_EQUAL theWires.end())
			{
				const auto& x = (*iter);
				if (Entity2d_Box::IsInside(*x->BoundingBox(), *theOuter->BoundingBox()))
				{
					theInners.push_back(x);
					removes.push_back(iter);
				}

				iter++;
			}

			for (const auto& x : removes)
			{
				theWires.erase(x);
			}
		}
	}
}

std::shared_ptr<tnbLib::Marine_CmpSection>
tnbLib::Marine_CmpSection::CreateCmpSection
(
	const std::vector<Handle(Geom2d_Curve)>& theCurves,
	const gp_Ax2 & theSystem,
	const Standard_Real theMinTol,
	const Standard_Real theMaxTol
)
{
	std::vector<std::shared_ptr<Pln_Curve>> curves;
	curves.reserve(theCurves.size());

	Standard_Integer K = 0;
	for (const auto& x : theCurves)
	{
		Debug_Null_Pointer(x);

		if (NOT Pln_Tools::IsBounded(x))
		{
			FatalErrorIn("std::shared_ptr<Marine_CmpSection> Marine_CmpSection::CreateCmpSection(Args....)")
				<< "the curve is not bounded" << endl
				<< abort(FatalError);
		}

		curves.push_back(std::make_shared<Pln_Curve>(++K, x));
	}

	auto section = CreateCmpSection(curves, theSystem, theMinTol, theMaxTol);
	return std::move(section);
}

std::shared_ptr<tnbLib::Marine_CmpSection> 
tnbLib::Marine_CmpSection::CreateCmpSection
(
	const std::vector<std::shared_ptr<Pln_Curve>>& theCurves, 
	const gp_Ax2 & theSystem, 
	const Standard_Real theMinTol, 
	const Standard_Real theMaxTol
)
{
	auto wires = Pln_Tools::RetrieveWires(theCurves, theMinTol, theMaxTol);

	createCmpSection::SortWires(wires);

	Standard_Integer K = 0;
	for (const auto& x : wires)
	{
		Debug_Null_Pointer(x);
		x->SetIndex(++K);
	}

	std::list<std::shared_ptr<Pln_Wire>> wireList;
	for (const auto& x : wires)
	{
		Debug_Null_Pointer(x);
		wireList.push_back(x);
	}

	K = 0;
	auto cmpSection = std::make_shared<Marine_CmpSection>();
	Debug_Null_Pointer(cmpSection);

	cmpSection->SetCoordinateSystem(theSystem);

	auto& sections = cmpSection->ChangeSections();
	while (NOT wireList.empty())
	{
		std::shared_ptr<Pln_Wire> outer;
		auto inners = std::make_shared<std::vector<std::shared_ptr<Pln_Wire>>>();
		Debug_Null_Pointer(inners);

		createCmpSection::RetrieveInnerOuterWires(wireList, outer, *inners);

		if (NOT outer)
		{
			FatalErrorIn("std::shared_ptr<tnbLib::Marine_CmpSection> Marine_CmpSection::CreateCmpSection(Args...)")
				<< "Invalid section" << endl
				<< abort(FatalError);
		}

		if (outer->Orientation() NOT_EQUAL Pln_Orientation::Pln_Orientation_CCW)
		{
			outer->ApplyOrientation(Pln_Orientation::Pln_Orientation_CCW);
		}

		if (inners->empty())
		{
			auto section = std::make_shared<Marine_Section>(++K, outer);
			Debug_Null_Pointer(section);

			sections.push_back(std::move(section));
		}
		else
		{
			FatalErrorIn("std::shared_ptr<tnbLib::Marine_CmpSection> Marine_CmpSection::CreateCmpSection(Args...)")
				<< "the section shouldn't contain any inner wire" << endl
				<< abort(FatalError);

			/*for (const auto& x : *inners)
			{
				Debug_Null_Pointer(x);
				if (x->Orientation() NOT_EQUAL Pln_Orientation::Pln_Orientation_CW)
				{
					x->ApplyOrientation(Pln_Orientation::Pln_Orientation_CW);
				}
			}

			auto section = std::make_shared<Marine_Section>(++K, outer, inners);
			Debug_Null_Pointer(section);

			sections.push_back(std::move(section));*/
		}
	}

	return std::move(cmpSection);
}

std::shared_ptr<tnbLib::Marine_CmpSection>
tnbLib::Marine_CmpSection::CreateCmpSection
(
	const TopoDS_Shape & theEdges,
	const gp_Ax2& theSystem,
	const Standard_Real theMinTol,
	const Standard_Real theMaxTol
)
{
	auto edges = RetrieveEdges(theEdges);
	auto curves = RetrieveParaCurves(edges, theSystem);

	auto cmpSection = 
		CreateCmpSection(curves, theSystem, theMinTol, theMaxTol);

	return std::move(cmpSection);
}

#include <gp_Pln.hxx>
#include <Geom_Plane.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <GeomProjLib.hxx>

#include <TopLoc_Datum3D.hxx>

std::vector<TopoDS_Edge>
tnbLib::Marine_CmpSection::RetrieveEdges
(
	const TopoDS_Shape & theEdges
)
{
	std::vector<TopoDS_Edge> edges;
	for
		(
			TopExp_Explorer explorer(theEdges, TopAbs_EDGE);
			explorer.More();
			explorer.Next()
			)
	{
		auto edge = TopoDS::Edge(explorer.Current());
		if (NOT edge.IsNull())
		{
			edges.push_back(edge);
		}
	}
	return std::move(edges);
}

std::vector<Handle(Geom2d_Curve)>
tnbLib::Marine_CmpSection::RetrieveParaCurves
(
	const std::vector<TopoDS_Edge>& theEdges,
	const gp_Ax2& theSystem
)
{
	Handle(Geom_Plane) plane = new Geom_Plane(gp_Pln(theSystem));
	Debug_Null_Pointer(plane);

	TopLoc_Location loc;

	std::vector<Handle(Geom2d_Curve)> curves;
	curves.reserve(theEdges.size());

	for (const auto& x : theEdges)
	{
		Standard_Real first, last;
		auto curve = BRep_Tool::CurveOnPlane(x, plane, loc, first, last);

		if (curve.IsNull())
		{
			FatalErrorIn("std::vector<Handle(Geom2d_Curve)> Marine_CmpSection::RetrieveParaCurves(Args...)")
				<< "Failed to Calculate the parametric curve" << endl
				<< abort(FatalError);
		}

		curves.push_back(std::move(curve));
	}
	return std::move(curves);
}