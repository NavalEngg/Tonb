#pragma once
#ifndef _Cad3d_TModel_Header
#define _Cad3d_TModel_Header

#include <TModel_Entity.hxx>
#include <Cad3d_TModelManager.hxx>
#include <Entity3d_Box.hxx>
#include <OFstream.hxx>

#include <TopoDS_Shape.hxx>

#include <vector>

namespace tnbLib
{

	// Forward Declarations
	class TModel_Shell;
	class Cad_Tools;

	class Cad3d_TModel
		: public TModel_Entity
		, public Cad3d_TModelManager
	{

		typedef std::vector<std::shared_ptr<TModel_Shell>> shellList;

		typedef std::shared_ptr<TModel_Shell> outer;
		typedef std::shared_ptr<shellList> inner;

		friend class Cad_Tools;

		/*Private Data*/

		outer theOuter_;
		inner theInner_;

		TopoDS_Shape theShape_;

	public:

		TnbCad_EXPORT Cad3d_TModel();

		TnbCad_EXPORT Cad3d_TModel
		(
			const Standard_Integer theIndex
		);

		TnbCad_EXPORT Cad3d_TModel
		(
			const Standard_Integer theIndex,
			const word& theName
		);

	protected:

		void SetShape
		(
			const TopoDS_Shape& theShape
		)
		{
			theShape_ = theShape;
		}

	public:

		const auto& Shape() const
		{
			return theShape_;
		}

		const auto& Outer() const
		{
			return theOuter_;
		}

		const auto& Holes() const
		{
			return theInner_;
		}

		TnbCad_EXPORT Standard_Integer NbHoles() const;

		auto HasHole() const
		{
			return (Standard_Boolean)NbHoles();
		}

		//- IO functions and operators

		TnbCad_EXPORT void ExportCornersToPlt(OFstream& File) const;

		TnbCad_EXPORT void ExportSegmentsToPlt(OFstream& File) const;

		TnbCad_EXPORT void ExportFacesToPlt(OFstream& File) const;

		TnbCad_EXPORT void ExportFreeSegmentsToPlt(OFstream& File) const;


	};
}

#endif // !_Cad3d_TModel_Header]
