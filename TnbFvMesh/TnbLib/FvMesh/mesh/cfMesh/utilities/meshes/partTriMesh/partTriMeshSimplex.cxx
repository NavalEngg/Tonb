#include <partTriMeshSimplex.hxx>

#include <Map.hxx>

//#define DEBUGSmooth

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	partTriMeshSimplex::partTriMeshSimplex
	(
		const partTriMesh& tm,
		const label pI
	)
		:
		pts_(),
		trias_()
	{
		const pointField& points = tm.points();
		const LongList<labelledTri>& trias = tm.triangles();
		const VRWGraph& pt = tm.pointTriangles();
		const LongList<direction>& pType = tm.pointType();

		//trias_.setSize(pt.sizeOfRow(pI));
		label counter(0);

		Map<label> addr(2 * pt.sizeOfRow(pI));
		forAllRow(pt, pI, tI)
		{
			const labelledTri& tri = trias[pt(pI, tI)];
			for (label i = 0; i < 3; ++i)
			{
				const label tpI = tri[i];

				if (!addr.found(tpI))
				{
					addr.insert(tpI, counter);
					pts_.append(points[tpI]);
					++counter;
				}
			}

# ifdef DEBUGSmooth
			Info << "Tet " << tetI << " is " << tet << endl;
# endif

			label pos(-1);
			for (label i = 0; i < 3; ++i)
				if (tri[i] == pI)
				{
					pos = i;
					break;
				}

			//- avoid using triangle serving as barriers for other points
			if (!(pType[tri[2]] & partTriMesh::FACECENTRE) && (pos != 0))
				continue;

			switch (pos)
			{
			case 0:
			{
				trias_.append
				(
					triFace(addr[tri[0]], addr[tri[1]], addr[tri[2]])
				);
			} break;
			case 1:
			{
				trias_.append
				(
					triFace(addr[tri[1]], addr[tri[2]], addr[tri[0]])
				);
			} break;
			case 2:
			{
				trias_.append
				(
					triFace(addr[tri[2]], addr[tri[0]], addr[tri[1]])
				);
			} break;
			default:
			{
				FatalErrorIn
				(
					"partTriMeshSimplex::partTriMeshSimplex("
					"(const partTriMesh& tm, const label pI)"
				) << "Point " << pI << " is not present in triangle" << tri
					<< abort(FatalError);
			}
			}
		}

# ifdef DEBUGSmooth
		Info << "Simplex at point " << pI << " points " << pts_ << endl;
		Info << "Simplex at point " << pI << " triangles " << trias_ << endl;
# endif

		if (pts_.size() == 0 || trias_.size() == 0)
			FatalError << "Simplex at point " << pI << " is not valid "
			<< pts_ << " triangles " << trias_ << abort(FatalError);
	}

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	partTriMeshSimplex::~partTriMeshSimplex()
	{}

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	vector partTriMeshSimplex::normal() const
	{
		vector normal(vector::zero);
		scalar magN(0.0);

		forAll(trias_, tI)
		{
			const triFace& t = trias_[tI];

			vector n
			(
				0.5 * ((pts_[t[1]] - pts_[t[0]]) ^ (pts_[t[2]] - pts_[t[0]]))
			);
			const scalar magn = mag(n);

			normal += n;
			magN += magn;
		}

		return (normal / (magN + VSMALL));
	}

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// ************************************************************************* //
