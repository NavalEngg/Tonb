#pragma once
#include <Geo_Tools.hxx>

namespace tnbLib
{

	template<class ElementTraits>
	Standard_Real Mesh_Element<ElementTraits, Mesh_ElementType_Triangle3D>::Oriented
	(
		const Point & theCoord
	) const
	{
		return Geo_Tools::Oriented_cgal(theCoord, Node0()->Coord(), Node1()->Coord(), Node2()->Coord());
	}

	template<class ElementTraits>
	Standard_Real Mesh_Element<ElementTraits, Mesh_ElementType_Triangle3D>::CalcDistance
	(
		const Point & theCoord
	) const
	{
		return Geo_Tools::Distance_cgal(theCoord, Node0()->Coord(), Node1()->Coord(), Node2()->Coord());
	}

	template<class ElementTraits>
	Standard_Real Mesh_Element<ElementTraits, Mesh_ElementType_Triangle3D>::CalcSquareDistance
	(
		const Point & theCoord
	) const
	{
		return Geo_Tools::SquareDistance_cgal(theCoord, Node0()->Coord(), Node1()->Coord(), Node2()->Coord());
	}
}

template<class ElementTraits, tnbLib::Mesh_ElementType ElmType>
inline std::vector<std::shared_ptr<typename tnbLib::Mesh_Element<ElementTraits, ElmType>::nodeType>>
tnbLib::Mesh_Element<ElementTraits, ElmType>::RetrieveNodes
(
	const std::vector<std::shared_ptr<Mesh_Element>>& theElements
)
{
	std::map<Standard_Integer, std::shared_ptr<nodeType>> nodes_map;
	for (const auto& x : theElements)
	{
		Debug_Null_Pointer(x);

		const auto& n0 = x->Node0();
		const auto& n1 = x->Node1();
		const auto& n2 = x->Node2();
		const auto& n3 = x->Node3();

		Debug_Null_Pointer(n0);
		Debug_Null_Pointer(n1);
		Debug_Null_Pointer(n2);
		Debug_Null_Pointer(n3);

		auto iter = nodes_map.find(n0->Index());
		if (iter == nodes_map.end())
		{
			auto paired = std::make_pair(n0->Index(), n0);
			nodes_map.insert(std::move(paired));
		}

		iter = nodes_map.find(n1->Index());
		if (iter == nodes_map.end())
		{
			auto paired = std::make_pair(n1->Index(), n1);
			nodes_map.insert(std::move(paired));
		}

		iter = nodes_map.find(n2->Index());
		if (iter == nodes_map.end())
		{
			auto paired = std::make_pair(n2->Index(), n2);
			nodes_map.insert(std::move(paired));
		}

		iter = nodes_map.find(n3->Index());
		if (iter == nodes_map.end())
		{
			auto paired = std::make_pair(n3->Index(), n3);
			nodes_map.insert(std::move(paired));
		}
	}
	std::vector<std::shared_ptr<nodeType>> nodes;
	nodes.reserve(nodes_map.size());
	for (auto& x : nodes_map)
	{
		nodes.push_back(std::move(x.second));
	}
	return std::move(nodes);
}

template<class ElementTraits>
inline std::vector<std::shared_ptr<typename tnbLib::Mesh_Element<ElementTraits, tnbLib::Mesh_ElementType_Triangle3D>::nodeType>>
tnbLib::Mesh_Element<ElementTraits, tnbLib::Mesh_ElementType_Triangle3D>::RetrieveNodes
(
	const std::vector<std::shared_ptr<Mesh_Element>>& theElements
)
{
	std::map<Standard_Integer, std::shared_ptr<nodeType>> nodes_map;
	for (const auto& x : theElements)
	{
		Debug_Null_Pointer(x);

		const auto& n0 = x->Node0();
		const auto& n1 = x->Node1();
		const auto& n2 = x->Node2();

		Debug_Null_Pointer(n0);
		Debug_Null_Pointer(n1);
		Debug_Null_Pointer(n2);

		auto iter = nodes_map.find(n0->Index());
		if (iter == nodes_map.end())
		{
			auto paired = std::make_pair(n0->Index(), n0);
			nodes_map.insert(std::move(paired));
		}

		iter = nodes_map.find(n1->Index());
		if (iter == nodes_map.end())
		{
			auto paired = std::make_pair(n1->Index(), n1);
			nodes_map.insert(std::move(paired));
		}

		iter = nodes_map.find(n2->Index());
		if (iter == nodes_map.end())
		{
			auto paired = std::make_pair(n2->Index(), n2);
			nodes_map.insert(std::move(paired));
		}	
	}

	std::vector<std::shared_ptr<nodeType>> nodes;
	nodes.reserve(nodes_map.size());
	for (auto& x : nodes_map)
	{
		nodes.push_back(std::move(x.second));
	}
	return std::move(nodes);
}

template<class ElementTraits>
inline std::vector<std::shared_ptr<typename tnbLib::Mesh_Element<ElementTraits, tnbLib::Mesh_ElementType_Triangle2D>::nodeType>>
tnbLib::Mesh_Element<ElementTraits, tnbLib::Mesh_ElementType_Triangle2D>::RetrieveNodes
(
	const std::vector<std::shared_ptr<Mesh_Element>>& theElements
)
{
	std::map<Standard_Integer, std::shared_ptr<nodeType>> nodes_map;
	for (const auto& x : theElements)
	{
		Debug_Null_Pointer(x);

		const auto& n0 = x->Node0();
		const auto& n1 = x->Node1();
		const auto& n2 = x->Node2();

		Debug_Null_Pointer(n0);
		Debug_Null_Pointer(n1);
		Debug_Null_Pointer(n2);

		auto iter = nodes_map.find(n0->Index());
		if (iter == nodes_map.end())
		{
			auto paired = std::make_pair(n0->Index(), n0);
			nodes_map.insert(std::move(paired));
		}

		iter = nodes_map.find(n1->Index());
		if (iter == nodes_map.end())
		{
			auto paired = std::make_pair(n1->Index(), n1);
			nodes_map.insert(std::move(paired));
		}

		iter = nodes_map.find(n2->Index());
		if (iter == nodes_map.end())
		{
			auto paired = std::make_pair(n2->Index(), n2);
			nodes_map.insert(std::move(paired));
		}
	}

	std::vector<std::shared_ptr<nodeType>> nodes;
	nodes.reserve(nodes_map.size());
	for (auto& x : nodes_map)
	{
		nodes.push_back(std::move(x.second));
	}
	return std::move(nodes);
}