#pragma once
#include "API/library.h"
#include <Eigen/Dense>
#include "../ComLightLib/comLightServer.h"

namespace MeshRepair
{
	class ResultMesh : public ComLight::ObjectRoot<iResultMesh>
	{
		HRESULT ensureGoodMesh() const;

		HRESULT COMLIGHTCALL getSize( uint32_t& vertices, uint32_t& triangles ) const noexcept override final;
		HRESULT COMLIGHTCALL getIndexBuffer( uint32_t* rdi ) const noexcept override final;
		HRESULT COMLIGHTCALL getVertexBufferFP32( float* rdi ) const noexcept override final;
		HRESULT COMLIGHTCALL getVertexBufferFP64( double* rdi ) const noexcept override final;

	public:

		Eigen::MatrixXd V_sf;
		Eigen::MatrixXi F_sf;
	};

}  // namespace MeshRepair