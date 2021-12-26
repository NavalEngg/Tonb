#pragma once
#ifndef _Aft2d_gModelSurface_Header
#define _Aft2d_gModelSurface_Header

#include <Aft_Model.hxx>
#include <Geo2d_SizeFunction.hxx>
#include <Geo2d_MetricFunction.hxx>
#include <Aft2d_MetricPrcsrSurface.hxx>
#include <Aft2d_OptNodeSurface_Calculator.hxx>
#include <Aft2d_FrontSurfaceGlobalData.hxx>
#include <Aft2d_FrontSurfaceInfo.hxx>
#include <Aft2d_gModelSurfaceFwd.hxx>
#include <Aft2d_CoreSurface.hxx>

#include <Aft2d_gModelSurfaceI.hxx>

namespace tnbLib
{

	template<>
	template<>
	TnbMesh_EXPORT void Aft2d_gModelSurface::serialize<TNB_iARCH_TYPE>
		(
			TNB_iARCH_TYPE& ar,
			const unsigned int file_version
			);

	template<>
	template<>
	TnbMesh_EXPORT void Aft2d_gModelSurface::serialize<TNB_oARCH_TYPE>
		(
			TNB_oARCH_TYPE& ar,
			const unsigned int file_version
			);
}

#endif // !_Aft2d_gModelSurface_Header
