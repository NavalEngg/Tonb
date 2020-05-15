#include <polyMeshGenPoints.hxx>

#include <pointIOField.hxx>
#include <IOobjectList.hxx>
#include <pointSet.hxx>

namespace tnbLib
{

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
	// Constructors
	//- Null constructor
	polyMeshGenPoints::polyMeshGenPoints(const Time& runTime)
		:
		runTime_(runTime),
		points_
		(
			IOobject
			(
				"points",
				runTime.constant(),
				"polyMesh",
				runTime
			),
			0
		),
		pointSubsets_()
	{
	}

	//- Construct from time and points
	polyMeshGenPoints::polyMeshGenPoints
	(
		const Time& runTime,
		const pointField& points
	)
		:
		runTime_(runTime),
		points_
		(
			IOobject
			(
				"points",
				runTime.constant(),
				"polyMesh",
				runTime
			),
			points
		),
		pointSubsets_()
	{
	}

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
	// Destructor
	polyMeshGenPoints::~polyMeshGenPoints()
	{
	}

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	label polyMeshGenPoints::addPointSubset(const word& subsetName)
	{
		label id = pointSubsetIndex(subsetName);
		if (id >= 0)
		{
			Warning << "Point subset " << subsetName << " already exists!" << endl;
			return id;
		}

		id = 0;
		std::map<label, meshSubset>::const_iterator it;
		for (it = pointSubsets_.begin(); it != pointSubsets_.end(); ++it)
			id = tnbLib::max(id, it->first + 1);

		pointSubsets_.insert
		(
			std::make_pair
			(
				id,
				meshSubset(subsetName, meshSubset::POINTSUBSET)
			)
		);

		return id;
	}

	void polyMeshGenPoints::removePointSubset(const label subsetID)
	{
		if (pointSubsets_.find(subsetID) == pointSubsets_.end())
			return;

		pointSubsets_.erase(subsetID);
	}

	word polyMeshGenPoints::pointSubsetName(const label subsetID) const
	{
		std::map<label, meshSubset>::const_iterator it =
			pointSubsets_.find(subsetID);
		if (it == pointSubsets_.end())
		{
			Warning << "Subset " << subsetID << " is not a point subset" << endl;
			return word();
		}

		return it->second.name();
	}

	label polyMeshGenPoints::pointSubsetIndex(const word& subsetName) const
	{
		std::map<label, meshSubset>::const_iterator it;
		for (it = pointSubsets_.begin(); it != pointSubsets_.end(); ++it)
		{
			if (it->second.name() == subsetName)
				return it->first;
		}

		return -1;
	}

	void polyMeshGenPoints::read()
	{
		pointIOField pts
		(
			IOobject
			(
				"points",
				runTime_.constant(),
				"polyMesh",
				runTime_,
				IOobject::MUST_READ
			)
		);
		points_ = pts;

		//- read point subsets
		IOobjectList allSets
		(
			runTime_,
			runTime_.constant(),
			"polyMesh/sets"
		);

		wordList setNames = allSets.names("pointSet");
		forAll(setNames, setI)
		{
			IOobject* obj = allSets.lookup(setNames[setI]);

			pointSet pSet(*obj);

			const labelList content = pSet.toc();
			const label id = addPointSubset(setNames[setI]);

			pointSubsets_[id].updateSubset(content);
		}
	}

	void polyMeshGenPoints::write() const
	{
		points_.write();

		std::map<label, meshSubset>::const_iterator setIt;
		labelLongList containedElements;

		//- write point selections
		for (setIt = pointSubsets_.begin(); setIt != pointSubsets_.end(); ++setIt)
		{
			pointSet set
			(
				IOobject
				(
					setIt->second.name(),
					runTime_.constant(),
					"polyMesh/sets",
					runTime_,
					IOobject::NO_READ,
					IOobject::AUTO_WRITE
				)
			);

			setIt->second.containedElements(containedElements);

			forAll(containedElements, i)
				set.insert(containedElements[i]);
			set.write();
		}
	}

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //