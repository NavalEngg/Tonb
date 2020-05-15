#include <meshSurfaceEngine.hxx>

#include <demandDrivenData.hxx>

# ifdef USE_OMP
#include <omp.h>
# endif

// #define DEBUGSearch

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

	meshSurfaceEngine::meshSurfaceEngine(polyMeshGen& mesh)
		:
		mesh_(mesh),
		activePatch_(-1),
		boundaryPointsPtr_(NULL),
		boundaryFacesPtr_(NULL),
		boundaryFacePatchPtr_(NULL),
		boundaryFaceOwnersPtr_(NULL),
		pointFacesPtr_(NULL),
		pointInFacePtr_(NULL),
		pointPatchesPtr_(NULL),
		bppPtr_(NULL),
		pointPointsPtr_(NULL),
		edgesPtr_(NULL),
		bpEdgesPtr_(NULL),
		edgeFacesPtr_(NULL),
		faceEdgesPtr_(NULL),
		edgePatchesPtr_(NULL),
		faceFacesPtr_(NULL),
		pointNormalsPtr_(NULL),
		faceNormalsPtr_(NULL),
		faceCentresPtr_(NULL),

		globalBoundaryPointLabelPtr_(NULL),
		globalBoundaryPointToLocalPtr_(NULL),
		bpProcsPtr_(NULL),
		bpNeiProcsPtr_(NULL),
		globalBoundaryEdgeLabelPtr_(NULL),
		globalBoundaryEdgeToLocalPtr_(NULL),
		beProcsPtr_(NULL),
		beNeiProcsPtr_(NULL),
		otherEdgeFaceAtProcPtr_(NULL),
		otherEdgeFacePatchPtr_(NULL),
		globalBoundaryFaceLabelPtr_(NULL)
	{
		calculateBoundaryFaces();
		calculateBoundaryNodes();
	}

	meshSurfaceEngine::meshSurfaceEngine(polyMeshGen &mesh, const label patchI)
		:
		mesh_(mesh),
		activePatch_(patchI),
		boundaryPointsPtr_(NULL),
		boundaryFacesPtr_(NULL),
		boundaryFacePatchPtr_(NULL),
		boundaryFaceOwnersPtr_(NULL),
		pointFacesPtr_(NULL),
		pointInFacePtr_(NULL),
		pointPatchesPtr_(NULL),
		bppPtr_(NULL),
		pointPointsPtr_(NULL),
		edgesPtr_(NULL),
		bpEdgesPtr_(NULL),
		edgeFacesPtr_(NULL),
		faceEdgesPtr_(NULL),
		edgePatchesPtr_(NULL),
		faceFacesPtr_(NULL),
		pointNormalsPtr_(NULL),
		faceNormalsPtr_(NULL),
		faceCentresPtr_(NULL),

		globalBoundaryPointLabelPtr_(NULL),
		globalBoundaryPointToLocalPtr_(NULL),
		bpProcsPtr_(NULL),
		bpNeiProcsPtr_(NULL),
		globalBoundaryEdgeLabelPtr_(NULL),
		globalBoundaryEdgeToLocalPtr_(NULL),
		beProcsPtr_(NULL),
		beNeiProcsPtr_(NULL),
		otherEdgeFaceAtProcPtr_(NULL),
		otherEdgeFacePatchPtr_(NULL),
		globalBoundaryFaceLabelPtr_(NULL)
	{
		calculateBoundaryFaces();
		calculateBoundaryNodes();
	}

	// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

	meshSurfaceEngine::~meshSurfaceEngine()
	{
		clearOut();
	}

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	void meshSurfaceEngine::clearOut()
	{
		deleteDemandDrivenData(boundaryPointsPtr_);
		deleteDemandDrivenData(boundaryFacesPtr_);
		deleteDemandDrivenData(boundaryFacePatchPtr_);
		deleteDemandDrivenData(boundaryFaceOwnersPtr_);
		deleteDemandDrivenData(pointFacesPtr_);
		deleteDemandDrivenData(pointInFacePtr_);
		deleteDemandDrivenData(pointPatchesPtr_);
		deleteDemandDrivenData(bppPtr_);
		deleteDemandDrivenData(pointPointsPtr_);
		deleteDemandDrivenData(pointNormalsPtr_);
		deleteDemandDrivenData(faceNormalsPtr_);
		deleteDemandDrivenData(faceCentresPtr_);
		deleteDemandDrivenData(edgesPtr_);
		deleteDemandDrivenData(bpEdgesPtr_);
		deleteDemandDrivenData(edgeFacesPtr_);
		deleteDemandDrivenData(faceEdgesPtr_);
		deleteDemandDrivenData(edgePatchesPtr_);
		deleteDemandDrivenData(faceFacesPtr_);

		deleteDemandDrivenData(globalBoundaryPointLabelPtr_);
		deleteDemandDrivenData(globalBoundaryPointToLocalPtr_);
		deleteDemandDrivenData(bpProcsPtr_);
		deleteDemandDrivenData(bpNeiProcsPtr_);
		deleteDemandDrivenData(globalBoundaryEdgeLabelPtr_);
		deleteDemandDrivenData(globalBoundaryEdgeToLocalPtr_);
		deleteDemandDrivenData(beProcsPtr_);
		deleteDemandDrivenData(beNeiProcsPtr_);
		deleteDemandDrivenData(otherEdgeFaceAtProcPtr_);
		deleteDemandDrivenData(otherEdgeFacePatchPtr_);
		deleteDemandDrivenData(globalBoundaryFaceLabelPtr_);
	}

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// ************************************************************************* //