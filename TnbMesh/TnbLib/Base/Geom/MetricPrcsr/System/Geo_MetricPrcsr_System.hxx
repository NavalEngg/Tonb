#pragma once
#ifndef _Geo_MetricPrcsr_System_Header
#define _Geo_MetricPrcsr_System_Header

#include <memory>
#include <Mesh_Module.hxx>

namespace tnbLib
{

	// Forward Declarations
	class Geo_MetricPrcsrAnIso_Info;
	class NumAlg_AdaptiveInteg_Info;

	namespace sysLib
	{

		extern TnbMesh_EXPORT std::shared_ptr<NumAlg_AdaptiveInteg_Info> gl_geo_metric_processor_integration_info;
		extern TnbMesh_EXPORT std::shared_ptr<Geo_MetricPrcsrAnIso_Info> gl_geo_metric_processor_aniso_info;

		TnbMesh_EXPORT void init_gl_geo_metric_processor_run_time_info();
	}
}

#endif // !_Geo_MetricPrcsr_System_Header