#include <Tnb_Examples.hxx>

#include <Cad2d_Modeler.hxx>
#include <Cad2d_Modeler_Tools.hxx>
#include <error.hxx>
#include <OSstream.hxx>

void tnbLib::example_cad2d_modeler()
{
	auto seg0 = cad2dLib::Modeler_Tools::MakeSegment(Pnt2d(0, 0), Pnt2d(0.3, 1.67));
	Debug_Null_Pointer(seg0);

	auto seg1 = cad2dLib::Modeler_Tools::MakeSegment(Pnt2d(0.2, 0), Pnt2d(0.14, 0.23));
	Debug_Null_Pointer(seg1);

	auto seg2 = cad2dLib::Modeler_Tools::MakeSegment(Pnt2d(0.25, 0.336), Pnt2d(0.05, 0.98));
	Debug_Null_Pointer(seg2);

	Cad2d_Modeler modeler;
	modeler.SetDomain(Entity2d_Box(Pnt2d(-1, -1), Pnt2d(2, 2)));
	modeler.Import(seg0);
	modeler.Import(seg1);
	modeler.Import(seg2);

	cout << "nb segments = " << modeler.NbEdges() << std::endl;

	const auto& edges = modeler.Edges();
	for (const auto& x : edges)
	{
		cout << " edge nb. " << x.second->Index() << std::endl;
	}

	const auto& selected = modeler.SelectEdge(2);

	modeler.RemoveEdge(selected);

	cout << std::endl;
	cout << std::endl;
	cout << "nb segments = " << modeler.NbEdges() << std::endl;
	for (const auto& x : edges)
	{
		cout << " edge nb. " << x.second->Index() << std::endl;
	}

	auto seg3 = cad2dLib::Modeler_Tools::MakeSegment(Pnt2d(0.18, 0.32), Pnt2d(0.45, 0.1));
	Debug_Null_Pointer(seg3);

	modeler.Import(seg3);

	cout << std::endl;
	cout << std::endl;
	cout << "nb segments = " << modeler.NbEdges() << std::endl;
	for (const auto& x : edges)
	{
		cout << " edge nb. " << x.second->Index() << std::endl;
	}
}