#include <ggiAMGInterface.hxx>

#include <demandDrivenData.hxx>
#include <PstreamReduceOps.hxx>
#include <addToRunTimeSelectionTable.hxx>

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace tnbLib
{
	defineTypeNameAndDebug(ggiAMGInterface, 0);
	addToRunTimeSelectionTable
	(
		AMGInterface,
		ggiAMGInterface,
		lduInterface
	);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void tnbLib::ggiAMGInterface::initFastReduce() const
{
	if (mapPtr_)
	{
		FatalErrorIn("void ggiAMGInterface::initFastReduce() const")
			<< "map already calculated"
			<< abort(FatalError);
	}

	if (!Pstream::parRun())
	{
		FatalErrorIn("void ggiAMGInterface::initFastReduce() const")
			<< "Requested calculation of send-receive addressing for a "
			<< "serial run.  This is not allowed"
			<< abort(FatalError);
	}

	// Note: this is different from ggiPolyPatch comms because zone
	// is the same on master the slave side.
	// HJ, 31/May/2016

	// Establish parallel comms pattern

	// Get addressing
	const labelList& za = zoneAddressing();
	const labelList& shadowZa = shadowInterface().zoneAddressing();

	// Make a zone-sized field and fill it in with proc markings for processor
	// that holds and requires the data
	labelField zoneProcID(zoneSize(), -1);

	forAll(za, zaI)
	{
		zoneProcID[za[zaI]] = Pstream::myProcNo();
	}

	reduce(zoneProcID, maxOp<labelField>());

	// Find out where my zone data is coming from
	labelList nRecv(Pstream::nProcs(), 0);

	forAll(shadowZa, shadowZaI)
	{
		nRecv[zoneProcID[shadowZa[shadowZaI]]]++;
	}

	// Make a receiving sub-map
	// It tells me which data I will receive from which processor and
	// where I need to put it into the remoteZone data before the mapping
	labelListList constructMap(Pstream::nProcs());

	// Size the receiving list
	forAll(nRecv, procI)
	{
		constructMap[procI].setSize(nRecv[procI]);
	}

	// Reset counters for processors
	nRecv = 0;

	forAll(shadowZa, shadowZaI)
	{
		label recvProc = zoneProcID[shadowZa[shadowZaI]];

		constructMap[recvProc][nRecv[recvProc]] = shadowZa[shadowZaI];

		nRecv[recvProc]++;
	}

	// Make the sending sub-map
	// It tells me which data is required from me to be sent to which
	// processor

	// Algorithm
	// - expand the local zone faces with indices into a size of local zone
	// - go through remote zone addressing on all processors
	// - find out who hits my faces
	labelList localZoneIndices(zoneSize(), -1);

	forAll(za, zaI)
	{
		localZoneIndices[za[zaI]] = zaI;
	}

	labelListList shadowToReceiveAddr(Pstream::nProcs());

	// Get the list of what my shadow needs to receive from my zone
	// on all other processors
	shadowToReceiveAddr[Pstream::myProcNo()] = shadowZa;
	Pstream::gatherList(shadowToReceiveAddr);
	Pstream::scatterList(shadowToReceiveAddr);

	// Now local zone indices contain the index of a local face that will
	// provide the data.  For faces that are not local, the index will be -1

	// Find out where my zone data is going to

	// Make a sending sub-map
	// It tells me which data I will send to which processor
	labelListList sendMap(Pstream::nProcs());

	// Collect local labels to be sent to each processor
	forAll(shadowToReceiveAddr, procI)
	{
		const labelList& curProcSend = shadowToReceiveAddr[procI];

		// Find out how much of my data is going to this processor
		label nProcSend = 0;

		forAll(curProcSend, sendI)
		{
			if (localZoneIndices[curProcSend[sendI]] > -1)
			{
				nProcSend++;
			}
		}

		if (nProcSend > 0)
		{
			// Collect the indices
			labelList& curSendMap = sendMap[procI];

			curSendMap.setSize(nProcSend);

			// Reset counter
			nProcSend = 0;

			forAll(curProcSend, sendI)
			{
				if (localZoneIndices[curProcSend[sendI]] > -1)
				{
					curSendMap[nProcSend] =
						localZoneIndices[curProcSend[sendI]];
					nProcSend++;
				}
			}
		}
	}

	// Map will return the object of the size of remote zone
	// HJ, 9/May/2016
	mapPtr_ = new mapDistribute(zoneSize(), sendMap, constructMap);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

tnbLib::ggiAMGInterface::ggiAMGInterface
(
	const lduPrimitiveMesh& lduMesh,
	const lduInterface& fineInterface,
	const labelField& localRestrictAddressing,
	const labelField& neighbourRestrictAddressing
)
	:
	AMGInterface(lduMesh),
	fineGgiInterface_(refCast<const ggiLduInterface>(fineInterface)),
	zoneSize_(0),
	zoneAddressing_(),
	mapPtr_(NULL)
{
	// Note.
	// All processors will do the same coarsening and then filter
	// the addressing to the local processor
	// HJ, 1/Apr/2009

	// Note.
	// Signalling in global clustering requires me to recognise clustering
	// from separate processors as separate.  In the first phase, this will be
	// used to recognise cluster from each processor as separate and in the
	// second phase it will be used to filter local processor faces from
	// the global patch.
	// Currently, I am calculating unique cluster index as:
	//
	// id = cluster + procOffset*myProcID
	// With procOffset = 1 million, this should be sufficient for 2000 CPUs
	// with 2 million coarse cells each.  For larger numbers, I need a
	// larger max int, which can be changed on request
	// HJ, 1/Apr/2009

	// Expand the local and neighbour addressing to full zone size
	labelField localExpandAddressing(fineGgiInterface_.zoneSize(), 0);

	// Memory management, local
	{
		const labelList& addr = fineGgiInterface_.zoneAddressing();

		forAll(addr, i)
		{
			localExpandAddressing[addr[i]] =
				localRestrictAddressing[i] + procOffset * Pstream::myProcNo();
		}

		if (!localParallel())
		{
			// Optimisation of this comms call is needed
			// HJ, 9/Jun/2016
			reduce(localExpandAddressing, sumOp<labelField>());
		}
	}

	labelField neighbourExpandAddressing
	(
		fineGgiInterface_.shadowInterface().zoneSize(),
		0
	);

	// Memory management, neighbour
	{
		const labelList& addr =
			fineGgiInterface_.shadowInterface().zoneAddressing();

		forAll(addr, i)
		{
			neighbourExpandAddressing[addr[i]] =
				neighbourRestrictAddressing[i]
				+ procOffset * Pstream::myProcNo();
		}

		if (!localParallel())
		{
			// Optimisation of this comms call is needed
			// HJ, 9/Jun/2016
			reduce(neighbourExpandAddressing, sumOp<labelField>());
		}
	}

	// Make a lookup table of entries for owner/neighbour.
	// All sizes are guessed at the size of fine interface
	// HJ, 19/Feb/2009

	HashTable<SLList<label>, label, Hash<label> > neighboursTable
	(
		localExpandAddressing.size()
	);

	// Table of face-sets to be agglomerated
	HashTable<SLList<SLList<label> >, label, Hash<label> > faceFaceTable
	(
		localExpandAddressing.size()
	);

	// Table of face-sets weights to be agglomerated
	HashTable<SLList<SLList<scalar> >, label, Hash<label> >
		faceFaceWeightsTable
		(
			localExpandAddressing.size()
		);

	// Count the number of coarse faces
	label nCoarseFaces = 0;

	// Count the number of agglomeration pairs
	label nAgglomPairs = 0;

	// On the fine level, addressing is made in a labelListList
	if (fineGgiInterface_.fineLevel())
	{
		const labelListList& fineAddr = fineGgiInterface_.addressing();
		const scalarListList& fineWeights = fineGgiInterface_.weights();

		forAll(fineAddr, ffI)
		{
			const labelList& curFineNbrs = fineAddr[ffI];
			const scalarList& curFineWeigts = fineWeights[ffI];

			forAll(curFineNbrs, nbrI)
			{
				label curMaster = -1;
				label curSlave = -1;

				// My label = ffI
				// Nbr label = nnI
				const label nnI = curFineNbrs[nbrI];
				const scalar curNW = curFineWeigts[nbrI];

				if (fineGgiInterface_.master())
				{
					// Master side
					curMaster = localExpandAddressing[ffI];
					curSlave = neighbourExpandAddressing[nnI];
				}
				else
				{
					// Slave side
					curMaster = neighbourExpandAddressing[nnI];
					curSlave = localExpandAddressing[ffI];
				}

				// Look for the master cell.  If it has already got a face,
				// add the coefficient to the face.  If not, create a new
				// face.
				if (neighboursTable.found(curMaster))
				{
					// Check all current neighbours to see if the current
					// slave already exists.  If so, add the coefficient.

					SLList<label>& curNbrs = neighboursTable.find(curMaster)();

					SLList<SLList<label> >& curFaceFaces =
						faceFaceTable.find(curMaster)();

					SLList<SLList<scalar> >& curFaceWeights =
						faceFaceWeightsTable.find(curMaster)();

					bool nbrFound = false;

					SLList<label>::iterator nbrsIter = curNbrs.begin();

					SLList<SLList<label> >::iterator faceFacesIter =
						curFaceFaces.begin();

					SLList<SLList<scalar> >::iterator faceFaceWeightsIter =
						curFaceWeights.begin();

					for
						(
							;
							nbrsIter != curNbrs.end()
							&& faceFacesIter != curFaceFaces.end()
							&& faceFaceWeightsIter != curFaceWeights.end();
							++nbrsIter, ++faceFacesIter, ++faceFaceWeightsIter
							)
					{
						// Check neighbour slave
						if (nbrsIter() == curSlave)
						{
							nbrFound = true;
							faceFacesIter().append(ffI);
							faceFaceWeightsIter().append(curNW);
							nAgglomPairs++;

							break;
						}
					}

					if (!nbrFound)
					{
						curNbrs.append(curSlave);
						curFaceFaces.append(SLList<label>(ffI));
						curFaceWeights.append(SLList<scalar>(curNW));

						// New coarse face created
						nCoarseFaces++;
						nAgglomPairs++;
					}
				}
				else
				{
					// This master has got no neighbours yet.  Add a neighbour
					// and a coefficient, thus creating a new face
					neighboursTable.insert(curMaster, SLList<label>(curSlave));

					faceFaceTable.insert
					(
						curMaster,
						SLList<SLList<label> >(SLList<label>(ffI))
					);

					faceFaceWeightsTable.insert
					(
						curMaster,
						SLList<SLList<scalar> >(SLList<scalar>(curNW))
					);

					// New coarse face created
					nCoarseFaces++;
					nAgglomPairs++;
				}
			} // end for all current neighbours
		} // end for all fine faces
	}
	else
	{
		// Coarse level, addressing is stored in faceCells
		forAll(localExpandAddressing, ffI)
		{
			label curMaster = -1;
			label curSlave = -1;

			// Do switching on master/slave indices based on the
			// owner/neighbour of the processor index such that
			// both sides get the same answer.
			if (master())
			{
				// Master side
				curMaster = localExpandAddressing[ffI];
				curSlave = neighbourExpandAddressing[ffI];
			}
			else
			{
				// Slave side
				curMaster = neighbourExpandAddressing[ffI];
				curSlave = localExpandAddressing[ffI];
			}

			// Look for the master cell.  If it has already got a face,
			// add the coefficient to the face.  If not, create a new face.
			if (neighboursTable.found(curMaster))
			{
				// Check all current neighbours to see if the current slave
				// already exists and if so, add the fine face
				// to the agglomeration.

				SLList<label>& curNbrs = neighboursTable.find(curMaster)();

				SLList<SLList<label> >& curFaceFaces =
					faceFaceTable.find(curMaster)();

				SLList<SLList<scalar> >& curFaceWeights =
					faceFaceWeightsTable.find(curMaster)();

				bool nbrFound = false;

				SLList<label>::iterator nbrsIter = curNbrs.begin();

				SLList<SLList<label> >::iterator faceFacesIter =
					curFaceFaces.begin();

				SLList<SLList<scalar> >::iterator faceFaceWeightsIter =
					curFaceWeights.begin();

				for
					(
						;
						nbrsIter != curNbrs.end()
						&& faceFacesIter != curFaceFaces.end()
						&& faceFaceWeightsIter != curFaceWeights.end();
						++nbrsIter, ++faceFacesIter, ++faceFaceWeightsIter
						)
				{
					// Check neighbour slave
					if (nbrsIter() == curSlave)
					{
						nbrFound = true;
						faceFacesIter().append(ffI);
						// Add dummy weight
						faceFaceWeightsIter().append(1.0);
						nAgglomPairs++;
						break;
					}
				}

				if (!nbrFound)
				{
					curNbrs.append(curSlave);
					curFaceFaces.append(SLList<label>(ffI));
					// Add dummy weight
					curFaceWeights.append(SLList<scalar>(1.0));

					// New coarse face created
					nCoarseFaces++;
					nAgglomPairs++;
				}
			}
			else
			{
				// This master has got no neighbours yet.  Add a neighbour
				// and a coefficient, thus creating a new face
				neighboursTable.insert(curMaster, SLList<label>(curSlave));

				faceFaceTable.insert
				(
					curMaster,
					SLList<SLList<label> >(SLList<label>(ffI))
				);

				// Add dummy weight
				faceFaceWeightsTable.insert
				(
					curMaster,
					SLList<SLList<scalar> >(SLList<scalar>(1.0))
				);

				// New coarse face created
				nCoarseFaces++;
				nAgglomPairs++;
			}
		} // end for all fine faces
	}

	faceCells_.setSize(nCoarseFaces, -1);
	fineAddressing_.setSize(nAgglomPairs, -1);
	restrictAddressing_.setSize(nAgglomPairs, -1);
	restrictWeights_.setSize(nAgglomPairs);

	labelList contents = neighboursTable.toc();

	// Sort makes sure the order is identical on both sides.
	// Since the global zone is defined by this sort, the neighboursTable
	// must be complete on all processors
	// HJ, 20/Feb/2009 and 6/Jun/2016
	sort(contents);

	// Grab zone size and create zone addressing
	zoneSize_ = nCoarseFaces;

	zoneAddressing_.setSize(nCoarseFaces);
	label nProcFaces = 0;

	// Reset face counter for re-use
	nCoarseFaces = 0;
	nAgglomPairs = 0;

	if (master())
	{
		// On master side, the owner addressing is stored in table of contents
		forAll(contents, masterI)
		{
			SLList<label>& curNbrs =
				neighboursTable.find(contents[masterI])();

			// Note: neighbour processor index is irrelevant.  HJ, 1/Apr/2009

			SLList<SLList<label> >& curFaceFaces =
				faceFaceTable.find(contents[masterI])();

			SLList<SLList<scalar> >& curFaceWeights =
				faceFaceWeightsTable.find(contents[masterI])();

			SLList<label>::iterator nbrsIter = curNbrs.begin();
			SLList<SLList<label> >::iterator faceFacesIter =
				curFaceFaces.begin();

			SLList<SLList<scalar> >::iterator faceFaceWeightsIter =
				curFaceWeights.begin();

			for
				(
					;
					nbrsIter != curNbrs.end()
					&& faceFacesIter != curFaceFaces.end()
					&& faceFaceWeightsIter != curFaceWeights.end();
					++nbrsIter, ++faceFacesIter, ++faceFaceWeightsIter
					)
			{
				// Check if master is on local processor
				if
					(
						contents[masterI] >= procOffset * Pstream::myProcNo()
						&& contents[masterI] < procOffset*(Pstream::myProcNo() + 1)
						)
				{
					// Record that this face belongs locally
					zoneAddressing_[nProcFaces] = nCoarseFaces;
					faceCells_[nProcFaces] =
						contents[masterI] - procOffset * Pstream::myProcNo();
					nProcFaces++;

					SLList<label>::iterator facesIter =
						faceFacesIter().begin();
					SLList<scalar>::iterator weightsIter =
						faceFaceWeightsIter().begin();

					for
						(
							;
							facesIter != faceFacesIter().end()
							&& weightsIter != faceFaceWeightsIter().end();
							++facesIter, ++weightsIter
							)
					{
						fineAddressing_[nAgglomPairs] = facesIter();
						restrictAddressing_[nAgglomPairs] = nCoarseFaces;
						restrictWeights_[nAgglomPairs] = weightsIter();
						nAgglomPairs++;
					}
				}

				// Not a local face, but still created in global zone
				nCoarseFaces++;
			}
		}

		// Resize arrays: not all of ggi is used locally
		faceCells_.setSize(nProcFaces);
		zoneAddressing_.setSize(nProcFaces);

		fineAddressing_.setSize(nAgglomPairs);
		restrictAddressing_.setSize(nAgglomPairs);
		restrictWeights_.setSize(nAgglomPairs);
	}
	else
	{
		// On slave side, the owner addressing is stored in linked lists
		forAll(contents, masterI)
		{
			// Note: master processor index is irrelevant.  HJ, 1/Apr/2009

			SLList<label>& curNbrs = neighboursTable.find(contents[masterI])();

			SLList<SLList<label> >& curFaceFaces =
				faceFaceTable.find(contents[masterI])();

			SLList<SLList<scalar> >& curFaceWeights =
				faceFaceWeightsTable.find(contents[masterI])();

			SLList<label>::iterator nbrsIter = curNbrs.begin();

			SLList<SLList<label> >::iterator faceFacesIter =
				curFaceFaces.begin();

			SLList<SLList<scalar> >::iterator faceFaceWeightsIter =
				curFaceWeights.begin();
			for
				(
					;
					nbrsIter != curNbrs.end()
					&& faceFacesIter != curFaceFaces.end()
					&& faceFaceWeightsIter != curFaceWeights.end();
					++nbrsIter, ++faceFacesIter, ++faceFaceWeightsIter
					)
			{
				// Check if the face is on local processor
				if
					(
						nbrsIter() >= procOffset * Pstream::myProcNo()
						&& nbrsIter() < procOffset*(Pstream::myProcNo() + 1)
						)
				{
					// Record that this face belongs locally.
					zoneAddressing_[nProcFaces] = nCoarseFaces;
					faceCells_[nProcFaces] =
						nbrsIter() - procOffset * Pstream::myProcNo();
					nProcFaces++;

					SLList<label>::iterator facesIter =
						faceFacesIter().begin();

					SLList<scalar>::iterator weightsIter =
						faceFaceWeightsIter().begin();

					for
						(
							;
							facesIter != faceFacesIter().end()
							&& weightsIter != faceFaceWeightsIter().end();
							++facesIter, ++weightsIter
							)
					{
						fineAddressing_[nAgglomPairs] = facesIter();
						restrictAddressing_[nAgglomPairs] = nCoarseFaces;
						restrictWeights_[nAgglomPairs] = weightsIter();
						nAgglomPairs++;
					}
				}

				nCoarseFaces++;
			}
		}

		// Resize arrays: not all of ggi is used locally
		faceCells_.setSize(nProcFaces);
		zoneAddressing_.setSize(nProcFaces);
		fineAddressing_.setSize(nAgglomPairs);
		restrictAddressing_.setSize(nAgglomPairs);
		restrictWeights_.setSize(nAgglomPairs);
	}
}


// * * * * * * * * * * * * * * * * Destructor * * * * * * * * * * * * * * * //

tnbLib::ggiAMGInterface::~ggiAMGInterface()
{
	deleteDemandDrivenData(mapPtr_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

tnbLib::tmp<tnbLib::scalarField> tnbLib::ggiAMGInterface::agglomerateCoeffs
(
	const scalarField& fineCoeffs
) const
{
	// Note: reconsider better parallel communication here.
	// Currently expanding to full zone size
	// HJ, 16/Mar/2016

	// Reassemble fine coefficients to full fine zone size
	// No need to initialise to zero, as only local coefficients
	// are used.  HJ, 9/Jun/2016
	scalarField zoneFineCoeffs(fineGgiInterface_.zoneSize());

	const labelList& fineZa = fineGgiInterface_.zoneAddressing();

	forAll(fineZa, i)
	{
		zoneFineCoeffs[fineZa[i]] = fineCoeffs[i];
	}

	// Reduce zone data is not required: all coefficients are local
	// HJ, 9/Jun/2016

	scalarField zoneCoarseCoeffs(zoneSize(), 0);

	// Restrict coefficient
	forAll(restrictAddressing_, ffi)
	{
		zoneCoarseCoeffs[restrictAddressing_[ffi]] +=
			restrictWeights_[ffi] * zoneFineCoeffs[fineAddressing_[ffi]];
	}

	tmp<scalarField> tcoarseCoeffs(new scalarField(size(), 0.0));
	scalarField& coarseCoeffs = tcoarseCoeffs();

	// Filter zone coefficients to local field
	const labelList& za = zoneAddressing();

	forAll(za, i)
	{
		coarseCoeffs[i] = zoneCoarseCoeffs[za[i]];
	}

	return tcoarseCoeffs;
}


bool tnbLib::ggiAMGInterface::master() const
{
	return fineGgiInterface_.master();
}


bool tnbLib::ggiAMGInterface::fineLevel() const
{
	return false;
}


tnbLib::label tnbLib::ggiAMGInterface::shadowIndex() const
{
	return fineGgiInterface_.shadowIndex();
}


const tnbLib::ggiLduInterface& tnbLib::ggiAMGInterface::shadowInterface() const
{
	return refCast<const ggiLduInterface>(ldu().interfaces()[shadowIndex()]);
}


tnbLib::label tnbLib::ggiAMGInterface::interfaceSize() const
{
	return faceCells_.size();
}

tnbLib::label tnbLib::ggiAMGInterface::zoneSize() const
{
	return zoneSize_;
}


const tnbLib::labelList& tnbLib::ggiAMGInterface::zoneAddressing() const
{
	return zoneAddressing_;
}


const tnbLib::labelListList& tnbLib::ggiAMGInterface::addressing() const
{
	FatalErrorIn("const labelListList& ggiAMGInterface::addressing() const")
		<< "Requested fine addressing at coarse level"
		<< abort(FatalError);

	return labelListList::null();
}


bool tnbLib::ggiAMGInterface::localParallel() const
{
	return fineGgiInterface_.localParallel();
}


const tnbLib::mapDistribute& tnbLib::ggiAMGInterface::map() const
{
	if (!mapPtr_)
	{
		initFastReduce();
	}

	return *mapPtr_;
}


const tnbLib::scalarListList& tnbLib::ggiAMGInterface::weights() const
{
	FatalErrorIn("const labelListList& ggiAMGInterface::weights() const")
		<< "Requested fine addressing at coarse level"
		<< abort(FatalError);

	return scalarListList::null();
}


const tnbLib::tensorField& tnbLib::ggiAMGInterface::forwardT() const
{
	return fineGgiInterface_.forwardT();
}


const tnbLib::tensorField& tnbLib::ggiAMGInterface::reverseT() const
{
	return fineGgiInterface_.reverseT();
}


void tnbLib::ggiAMGInterface::initTransfer
(
	const Pstream::commsTypes commsType,
	const unallocLabelList& interfaceData
) const
{
	// Label transfer is local
	labelTransferBuffer_ = interfaceData;
}


tnbLib::tmp<tnbLib::labelField> tnbLib::ggiAMGInterface::transfer
(
	const Pstream::commsTypes,
	const unallocLabelList& interfaceData
) const
{
	// Label transfer is local without global reduction
	return this->shadowInterface().labelTransferBuffer();
}


void tnbLib::ggiAMGInterface::initInternalFieldTransfer
(
	const Pstream::commsTypes commsType,
	const unallocLabelList& iF
) const
{
	// Label transfer is local without global reduction
	labelTransferBuffer_ = interfaceInternalField(iF);
}


tnbLib::tmp<tnbLib::labelField> tnbLib::ggiAMGInterface::internalFieldTransfer
(
	const Pstream::commsTypes,
	const unallocLabelList&
) const
{
	return shadowInterface().labelTransferBuffer();
}


void tnbLib::ggiAMGInterface::initInternalFieldTransfer
(
	const Pstream::commsTypes commsType,
	const scalarField& iF
) const
{
	scalarField pif = interfaceInternalField(iF);

	// New treatment.  HJ, 26/Jun/2011
	fieldTransferBuffer_ = fastReduce(pif);
}


tnbLib::tmp<tnbLib::scalarField> tnbLib::ggiAMGInterface::internalFieldTransfer
(
	const Pstream::commsTypes,
	const scalarField&
) const
{
	return shadowInterface().fieldTransferBuffer();
}


// ************************************************************************* //