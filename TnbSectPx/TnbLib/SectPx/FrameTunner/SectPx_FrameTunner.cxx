#include <SectPx_FrameTunner.hxx>

#include <SectPx_Registry.hxx>
#include <SectPx_Frame.hxx>
#include <SectPx_Pnts.hxx>
#include <SectPx_Poles.hxx>
#include <SectPx_Segment.hxx>
#include <SectPx_Makers.hxx>
#include <SectPx_Node.hxx>
#include <SectPx_PntTools.hxx>
#include <SectPx_Interfaces.hxx>
#include <SectPx_ProfileQ.hxx>
#include <SectPx_GeoMap_LinearInterpl.hxx>
#include <SectPx_OneParCPtsMap_Single.hxx>
#include <SectPx_SegmentController.hxx>
#include <SectPx_TightController_Deg2.hxx>

void tnbLib::SectPx_FrameTunner::disJoinSegment
(
	const std::shared_ptr<SectPx_Pole>& thePole,
	const std::shared_ptr<SectPx_Segment>& theSegment
)
{
	Debug_Null_Pointer(thePole);
	Debug_Null_Pointer(theSegment);

	if (thePole->IsBoundary())
	{
		auto pt = std::dynamic_pointer_cast<SectPx_BndPole>(thePole);
		Debug_Null_Pointer(pt);

		Debug_Null_Pointer(pt->Segment().lock());
		if (pt->Segment().lock() NOT_EQUAL theSegment)
		{
			FatalErrorIn(FunctionSIG)
				<< "the pole is not belong to the segment" << endl
				<< abort(FatalError);
		}

		pt->SetSegment(nullptr);
	}
	else if (thePole->IsInterior())
	{
		auto pt = std::dynamic_pointer_cast<SectPx_InterPole>(thePole);
		Debug_Null_Pointer(pt);

		auto seg0 = pt->Backward().lock();
		auto seg1 = pt->Forward().lock();

		if (seg0)
		{
			if (seg0 IS_EQUAL theSegment)
			{
				pt->SetBackward(nullptr);
				return;
			}
		}

		if (seg1)
		{
			if (seg1 IS_EQUAL theSegment)
			{
				pt->SetForward(nullptr);
				return;
			}
		}

		FatalErrorIn(FunctionSIG)
			<< "the pole is not belong to the segment" << endl
			<< abort(FatalError);
	}
	else
	{
		FatalErrorIn(FunctionSIG)
			<< "contradictory data!" << endl
			<< "invalid type of the pole" << endl
			<< abort(FatalError);
	}
}

void tnbLib::SectPx_FrameTunner::disJoinSegment
(
	const std::shared_ptr<SectPx_Segment>& theSegment
)
{
	Debug_Null_Pointer(theSegment);
	const auto& p0 = theSegment->Pole0();
	const auto& p1 = theSegment->Pole1();

	disJoinSegment(p0, theSegment);
	disJoinSegment(p1, theSegment);
}

void tnbLib::SectPx_FrameTunner::JoinSegment
(
	const std::shared_ptr<SectPx_Pole>& thePole,
	const std::shared_ptr<SectPx_Segment>& theSegment,
	const short id
)
{
	if (thePole->IsBoundary())
	{
		auto pt = std::dynamic_pointer_cast<SectPx_BndPole>(thePole);
		Debug_Null_Pointer(pt);

		Debug_If_Condition(pt->Segment().lock());
		pt->SetSegment(theSegment);
	}
	else if (thePole->IsInterior())
	{
		auto pt = std::dynamic_pointer_cast<SectPx_InterPole>(thePole);
		Debug_Null_Pointer(pt);

		if (id IS_EQUAL 0)
		{
			Debug_If_Condition(pt->Forward().lock());
			pt->SetForward(theSegment);
		}
		else
		{
			Debug_If_Condition(pt->Backward().lock());
			pt->SetBackward(theSegment);
		}
	}
	else
	{
		FatalErrorIn(FunctionSIG)
			<< "invalid type of the pole!" << endl
			<< abort(FatalError);
	}
}

void tnbLib::SectPx_FrameTunner::JoinSegment
(
	const std::shared_ptr<SectPx_Segment>& theSegment
)
{
	Debug_Null_Pointer(theSegment);
	const auto& p0 = theSegment->Pole0();
	const auto& p1 = theSegment->Pole1();

	Debug_Null_Pointer(p0);
	Debug_Null_Pointer(p1);

	JoinSegment(p0, theSegment, 0);
	JoinSegment(p1, theSegment, 1);
}

std::shared_ptr<tnbLib::SectPx_Pole> 
tnbLib::SectPx_FrameTunner::SelectPole
(
	const Standard_Integer theIndex
) const
{
	Debug_Null_Pointer(Registry());

	auto item = Registry()->SelectObj(theIndex);
	Debug_Null_Pointer(item);

	auto pole = std::dynamic_pointer_cast<SectPx_Pole>(item);
	if (NOT pole)
	{
		FatalErrorIn("std::shared_ptr<SectPx_Pole> SelectPole(const Standard_Integer theIndex) const")
			<< "the item is not a pole!" << endl
			<< abort(FatalError) << endl;
	}
	return std::move(pole);
}

std::shared_ptr<tnbLib::SectPx_Segment> 
tnbLib::SectPx_FrameTunner::SelectSegment
(
	const Standard_Integer theIndex
) const
{
	Debug_Null_Pointer(Registry());

	auto item = Registry()->SelectObj(theIndex);
	Debug_Null_Pointer(item);

	auto seg = std::dynamic_pointer_cast<SectPx_Segment>(item);
	if (NOT seg)
	{
		FatalErrorIn("std::shared_ptr<SectPx_Segment> SelectSegment(const Standard_Integer theIndex) const")
			<< "the item is not a pole!" << endl
			<< abort(FatalError) << endl;
	}
	return std::move(seg);
}

std::tuple
<
	std::pair
	<
	typename tnbLib::SectPx_FrameTunner::segmentId, 
	typename tnbLib::SectPx_FrameTunner::segmentId
	>, 
	typename tnbLib::SectPx_FrameTunner::sliderId
>
tnbLib::SectPx_FrameTunner::CreateSlider
(
	const std::shared_ptr<SectPx_Segment>& theSegment,
	const std::shared_ptr<SectPx_Par>& thePar
)
{
	Debug_Null_Pointer(theSegment);
	if (theSegment->HasController())
	{
		FatalErrorIn(FunctionSIG)
			<< "the segment must have no controller!" << endl
			<< abort(FatalError);
	}

	const auto& pntMaker = std::make_shared<maker::Point>(Registry());
	Debug_Null_Pointer(pntMaker);

	const auto& parentReg = Registry();
	if (NOT parentReg)
	{
		FatalErrorIn(FunctionSIG)
			<< "no parent registry has been loaded!" << endl
			<< abort(FatalError);
	}

	disJoinSegment(theSegment);

	const auto& p0 = theSegment->Pole0();
	Debug_Null_Pointer(p0);

	const auto& p1 = theSegment->Pole1();
	Debug_Null_Pointer(p1);

	const auto geoMap =
		std::make_shared<sectPxLib::GeoMap_LinearInterpl>();
	Debug_Null_Pointer(geoMap);

	geoMap->SetQ0(p0->Pnt());
	geoMap->SetQ1(p1->Pnt());

	geoMap->SetPar(thePar);
	parentReg->Import(geoMap);

	const auto pnt_id = pntMaker->CreateField(geoMap);

	auto pnt =
		std::dynamic_pointer_cast<sectPxLib::Pnt_GeoField>
		(
			pntMaker->SelectPnt(pnt_id)
			);
	Debug_Null_Pointer(pnt);

	parentReg->Remove(theSegment);

	auto slider = std::make_shared<sectPxLib::Pole_Slider>(pnt);
	Debug_Null_Pointer(slider);

	auto seg0 = std::make_shared<SectPx_Segment>(p0, slider);
	Debug_Null_Pointer(seg0);

	auto seg1 = std::make_shared<SectPx_Segment>(slider, p1);
	Debug_Null_Pointer(seg1);

	const auto slider_id = parentReg->Import(std::move(slider));

	const auto seg0_id = parentReg->Import(std::move(seg0));
	const auto seg1_id = parentReg->Import(std::move(seg1));

	auto paired = std::make_pair(seg0_id, seg1_id);
	auto t = std::make_tuple(std::move(paired), slider_id);
	return std::move(t);
}

typename tnbLib::SectPx_FrameTunner::tightnessId 
tnbLib::SectPx_FrameTunner::CreateSymmTightnessDeg2
(
	const std::shared_ptr<SectPx_Pole>& thePole,
	const std::shared_ptr<SectPx_Par>& thePar
)
{
	if (NOT Registry()->IsContains(thePole))
	{
		FatalErrorIn(FunctionSIG)
			<< "the pole is not registered!" << endl
			<< abort(FatalError);
	}

	const auto& parents = Registry();
	Debug_Null_Pointer(parents);

	if (thePole->IsInterior())
	{
		auto corner = std::dynamic_pointer_cast<sectPxLib::Pole_Corner>(thePole);
		if (NOT corner)
		{
			FatalErrorIn(FunctionSIG)
				<< "the pole is not corner!" << endl
				<< abort(FatalError);
		}

		const auto bwd = corner->Backward().lock();
		Debug_Null_Pointer(bwd);

		const auto fwd = corner->Forward().lock();
		Debug_Null_Pointer(fwd);

		if (bwd->HasController())
		{
			FatalErrorIn(FunctionSIG)
				<< "the segment already has a controller!" << endl
				<< abort(FatalError);
		}

		if (fwd->HasController())
		{
			FatalErrorIn(FunctionSIG)
				<< "the segment already has a controller!" << endl
				<< abort(FatalError);
		}

		auto bwd_ctrl = std::make_shared<sectPxLib::OneParCPtsMap_Single>();
		Debug_Null_Pointer(bwd_ctrl);

		parents->Import(bwd_ctrl);

		bwd_ctrl->SetSegment(bwd, Standard_False);
		bwd_ctrl->SetPar(thePar);

		auto fwd_ctrl = std::make_shared<sectPxLib::OneParCPtsMap_Single>();
		Debug_Null_Pointer(fwd_ctrl);

		parents->Import(fwd_ctrl);

		fwd_ctrl->SetSegment(fwd);
		fwd_ctrl->SetPar(thePar);

		auto seg_bwd_ctrl = std::make_shared<SectPx_SegmentController>();
		Debug_Null_Pointer(seg_bwd_ctrl);

		seg_bwd_ctrl->SetCPts(bwd_ctrl);

		auto seg_fwd_ctrl = std::make_shared<SectPx_SegmentController>();
		Debug_Null_Pointer(seg_fwd_ctrl);

		seg_fwd_ctrl->SetCPts(fwd_ctrl);

		Registry()->Import(seg_bwd_ctrl);
		Registry()->Import(seg_fwd_ctrl);

		auto pole_controller =
			std::make_shared<sectPxLib::TightController_Deg2>(std::move(corner));
		Debug_Null_Pointer(pole_controller);

		pole_controller->SetLeft(seg_bwd_ctrl);
		pole_controller->SetRight(seg_fwd_ctrl);

		bwd->SetController(seg_bwd_ctrl);
		fwd->SetController(seg_fwd_ctrl);

		return Registry()->Import(std::move(pole_controller));
	}
	else
	{
		FatalErrorIn(FunctionSIG)
			<< "the pole is not interior!" << endl
			<< abort(FatalError);
	}
	return 0;
}

namespace tnbLib
{

	typedef std::vector<std::shared_ptr<sectPxLib::Pnt_Empty>> emptyPntList;
	typedef std::vector<std::shared_ptr<sectPxLib::Pnt_Compnt>> componentPntList;
	typedef std::vector<std::shared_ptr<SectPx_Pnt>> pntList;
	typedef std::vector<std::shared_ptr<SectPx_Pnt>> cornerPntList;


	struct pntType
	{
		std::shared_ptr<SectPx_Pnt> pnt;
		SectPx_PoleType t;
	};

	// retrieve slaves, masters, corners, and dangles respectively
	static std::tuple<emptyPntList, componentPntList, cornerPntList, pntList>
		RetrievePntTypes(const std::map<Standard_Integer, std::shared_ptr<maker::Profile>>& theProfiles, const Standard_Integer maxIndex)
	{
		emptyPntList slavePoints;
		componentPntList masterPoints;
		cornerPntList cornerPoints;
		pntList danglePoints;

		std::vector<Standard_Boolean> visited(maxIndex + 1, Standard_False);

		// retrieve dangle, slave and master points
		for (const auto& x : theProfiles)
		{
			const auto& profile = x.second;
			Debug_Null_Pointer(profile);

			const auto& node0 = profile->Node0();
			const auto& node1 = profile->Node1();
			Debug_Null_Pointer(node0);
			Debug_Null_Pointer(node1);

			auto id0 = node0->Index();
			auto id1 = node1->Index();

			if (NOT visited[id0])
			{
				auto interface0 = node0->Interface().lock();
				Debug_Null_Pointer(interface0);

				if (interface0->IsEmpty())
				{
					danglePoints.push_back(node0->Pnt());
				}
				else
				{
					const auto& pnt = node0->Pnt();
					if (pnt->IsMaster())
					{
						auto master = std::dynamic_pointer_cast<sectPxLib::Pnt_Compnt>(pnt);
						Debug_Null_Pointer(master);
						masterPoints.push_back(std::move(master));
					}
					else if (pnt->IsSlave())
					{
						auto slave = std::dynamic_pointer_cast<sectPxLib::Pnt_Empty>(pnt);
						Debug_Null_Pointer(slave);
						slavePoints.push_back(std::move(slave));
					}
					else
					{
						FatalErrorIn(FunctionSIG)
							<< "invalid data!" << endl
							<< " this point is supposed to be master or slave!" << endl
							<< " type = " << pnt->RegObjTypeName() << endl
							<< abort(FatalError);
					}
				}
				visited[id0] = Standard_True;
			}

			if (NOT visited[id1])
			{
				auto interface1 = node1->Interface().lock();
				Debug_Null_Pointer(interface1);

				if (interface1->IsEmpty())
				{
					danglePoints.push_back(node1->Pnt());
				}
				else
				{
					const auto& pnt = node1->Pnt();
					if (pnt->IsMaster())
					{
						auto master = std::dynamic_pointer_cast<sectPxLib::Pnt_Compnt>(pnt);
						Debug_Null_Pointer(master);
						masterPoints.push_back(std::move(master));
					}
					else if (pnt->IsSlave())
					{
						auto slave = std::dynamic_pointer_cast<sectPxLib::Pnt_Empty>(pnt);
						Debug_Null_Pointer(slave);
						slavePoints.push_back(std::move(slave));
					}
					else
					{
						FatalErrorIn(FunctionSIG)
							<< "invalid data!" << endl
							<< " this point is supposed to be master or slave!" << endl
							<< " type = " << pnt->RegObjTypeName() << endl
							<< abort(FatalError);
					}
				}
				visited[id1] = Standard_True;
			}
		}

		for (const auto& x : theProfiles)
		{
			const auto& profile = x.second;
			Debug_Null_Pointer(profile);

			const auto& node0 = profile->Node0();
			const auto& node1 = profile->Node1();
			Debug_Null_Pointer(node0);
			Debug_Null_Pointer(node1);

			auto profilePoints = SectPx_PntTools::TrackPnts(node0, node1);
			for (size_t i = 1; i < profilePoints.size() - 1; i++)
			{
				cornerPoints.push_back(std::move(profilePoints[i]));
			}
		}

		auto t = std::make_tuple
		(
			std::move(slavePoints),
			std::move(masterPoints),
			std::move(cornerPoints),
			std::move(danglePoints)
		);
		return std::move(t);
	}

	static std::map<Standard_Integer, pntType>
		RetrieveTypeMap(std::tuple<emptyPntList, componentPntList, cornerPntList, pntList>&& thePoints)
	{
		auto[slaves, masters, corners, dangles] = std::move(thePoints);

		std::map<Standard_Integer, pntType> reg;
		for (const auto& x : slaves)
		{
			Debug_Null_Pointer(x);
			auto id = x->Index();
			pntType t{ std::move(x),SectPx_PoleType::slave };

			auto paired = std::make_pair(id, std::move(t));
			auto insert = reg.insert(std::move(paired));
			Debug_If_Condition(NOT insert.second);
		}

		for (const auto& x : masters)
		{
			Debug_Null_Pointer(x);
			auto id = x->Index();
			pntType t{ std::move(x),SectPx_PoleType::master };

			auto paired = std::make_pair(id, std::move(t));
			auto insert = reg.insert(std::move(paired));
			Debug_If_Condition(NOT insert.second);
		}

		for (const auto& x : corners)
		{
			Debug_Null_Pointer(x);
			auto id = x->Index();
			pntType t{ std::move(x),SectPx_PoleType::corner };

			auto paired = std::make_pair(id, std::move(t));
			auto insert = reg.insert(std::move(paired));
			Debug_If_Condition(NOT insert.second);
		}

		for (const auto& x : dangles)
		{
			Debug_Null_Pointer(x);
			auto id = x->Index();
			pntType t{ std::move(x),SectPx_PoleType::dangle };

			auto paired = std::make_pair(id, std::move(t));
			auto insert = reg.insert(std::move(paired));
			Debug_If_Condition(NOT insert.second);
		}
		return std::move(reg);
	}
}

void tnbLib::SectPx_FrameTunner::ImportFrame
(
	const std::shared_ptr<SectPx_Frame>& theFrame
)
{
	theRegistry_ = theFrame->Registry();
	const auto& reg = Registry();
	const auto& pntsReg = reg->ScatterMap(SectPx_RegObjType::pnt);

	const auto& cmptProfile = theFrame->CmptProfileMaker();
	const auto& profiles = cmptProfile->Profiles();

	auto pointTypes = RetrievePntTypes(profiles, reg->MaxIndex());
	auto typeRegistry = RetrieveTypeMap(std::move(pointTypes));

	std::vector<std::shared_ptr<sectPxLib::Pole_Slave>> slaves;
	std::map<Standard_Integer, Standard_Integer> pntToPole;
	for (const auto& px : profiles)
	{
		Debug_Null_Pointer(px.second);
		const auto& profile = px.second;

		auto profileQ = profile->RetrieveProfileQ();
		for (const auto& x : profileQ->Q())
		{
			Debug_Null_Pointer(x);
			auto id = x->Index();
			auto iter = typeRegistry.find(id);
			if (iter IS_EQUAL typeRegistry.end())
			{
				FatalErrorIn(FunctionSIG)
					<< "the item is not in the tree!" << endl
					<< abort(FatalError);
			}

			Debug_If_Condition(iter->second.pnt NOT_EQUAL x);
			const auto& t = iter->second.t;

			switch (t)
			{
			case SectPx_PoleType::dangle:
			{
				auto pnt = std::dynamic_pointer_cast<sectPxLib::Pnt_Compnt>(x);
				Debug_Null_Pointer(pnt);

				auto pole = std::make_shared<sectPxLib::Pole_Dangle>(pnt);
				Debug_Null_Pointer(pole);

				auto paired = std::make_pair(pnt->Index(), Registry()->Import(std::move(pole)));
				auto insert = pntToPole.insert(std::move(paired));
				if (NOT insert.second)
				{
					FatalErrorIn(FunctionSIG)
						<< "duplicate data" << endl
						<< abort(FatalError);
				}
				break;
			}
			case SectPx_PoleType::corner:
			{
				auto pole = std::make_shared<sectPxLib::Pole_Corner>(x);
				Debug_Null_Pointer(pole);

				auto paired = std::make_pair(x->Index(), Registry()->Import(std::move(pole)));
				auto insert = pntToPole.insert(std::move(paired));
				if (NOT insert.second)
				{
					FatalErrorIn(FunctionSIG)
						<< "duplicate data" << endl
						<< abort(FatalError);
				}
				break;
			}
			case SectPx_PoleType::master:
			{
				auto pnt = std::dynamic_pointer_cast<sectPxLib::Pnt_Compnt>(x);
				Debug_Null_Pointer(pnt);

				auto pole = std::make_shared<sectPxLib::Pole_Master>(pnt);
				Debug_Null_Pointer(pole);

				auto paired = std::make_pair(x->Index(), Registry()->Import(std::move(pole)));
				auto insert = pntToPole.insert(std::move(paired));
				if (NOT insert.second)
				{
					FatalErrorIn(FunctionSIG)
						<< "duplicate data" << endl
						<< abort(FatalError);
				}
				break;
			}
			case SectPx_PoleType::slave:
			{
				auto pnt = std::dynamic_pointer_cast<sectPxLib::Pnt_Empty>(x);
				Debug_Null_Pointer(pnt);

				auto pole = std::make_shared<sectPxLib::Pole_Slave>();
				Debug_Null_Pointer(pole);

				auto paired = std::make_pair(x->Index(), Registry()->Import(pole));
				auto insert = pntToPole.insert(std::move(paired));
				if (NOT insert.second)
				{
					FatalErrorIn(FunctionSIG)
						<< "duplicate data" << endl
						<< abort(FatalError);
				}

				slaves.push_back(pole);
				break;
			}
			default:
				FatalErrorIn(FunctionSIG)
					<< "Invalid type of the Pnt" << endl
					<< abort(FatalError);
				break;
			}
		}
	}

	for (const auto& x : slaves)
	{
		Debug_Null_Pointer(x);
		auto id = x->Index();

		auto iter = pntToPole.find(id);
		if (iter IS_EQUAL pntToPole.end())
		{
			FatalErrorIn(FunctionSIG)
				<< "the item is not in the tree!" << endl
				<< abort(FatalError);
		}

		auto pole_id = iter->second;

		auto pole = Registry()->SelectObj(pole_id);
		Debug_Null_Pointer(pole);
		Debug_If_Condition(NOT pole->IsPole());

		auto master = std::dynamic_pointer_cast<sectPxLib::Pole_Master>(pole);
		Debug_Null_Pointer(master);

		x->SetMaster(master);
	}

	for (const auto& px : profiles)
	{
		Debug_Null_Pointer(px.second);
		const auto& profile = px.second;

		auto profileQ = profile->RetrieveProfileQ();

		const auto& Q = profileQ->Q();
		forThose(Index, 0, Q.size() - 2)
		{
			const auto& p0 = Q[Index];
			const auto& p1 = Q[Index + 1];

			auto id0 = p0->Index();
			auto iter0 = pntToPole.find(id0);
			if (iter0 IS_EQUAL pntToPole.end())
			{
				FatalErrorIn(FunctionSIG)
					<< "the item is not in the tree!" << endl
					<< abort(FatalError);
			}

			auto pole_id0 = iter0->second;

			auto poleObj0 = Registry()->SelectObj(pole_id0);
			Debug_Null_Pointer(poleObj0);
			Debug_If_Condition(NOT poleObj0->IsPole());

			auto id1 = p1->Index();
			auto iter1 = pntToPole.find(id1);
			if (iter1 IS_EQUAL pntToPole.end())
			{
				FatalErrorIn(FunctionSIG)
					<< "the item is not in the tree!" << endl
					<< abort(FatalError);
			}

			auto pole_id1 = iter1->second;

			auto poleObj1 = Registry()->SelectObj(pole_id1);
			Debug_Null_Pointer(poleObj1);
			Debug_If_Condition(NOT poleObj1->IsPole());

			Debug_If_Condition(poleObj0 IS_EQUAL poleObj1);

			auto pole0 = std::dynamic_pointer_cast<SectPx_Pole>(poleObj0);
			auto pole1 = std::dynamic_pointer_cast<SectPx_Pole>(poleObj1);

			auto seg = std::make_shared<SectPx_Segment>(std::move(pole0), std::move(pole1));
			Debug_Null_Pointer(seg);

			Registry()->Import(seg);

			JoinSegment(seg);
		}
	}
}