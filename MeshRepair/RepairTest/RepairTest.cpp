#include "stdafx.h"
#include "Utils/IndexedMesh.h"
#include "../MeshRepair/API/interfaces.h"

static const LPCTSTR stlSource = LR"(C:\Temp\2remove\MeshRepair\model.stl)";
static const LPCTSTR stlResult = LR"(C:\Temp\2remove\MeshRepair\model-saved.stl)";

namespace MeshRepair
{
	extern "C" HRESULT COMLIGHTCALL createMeshRepair( iMeshRepair** rdi );

	HRESULT createMesh( iMeshRepair* mr, const IndexedMesh& rsi, ComLight::CComPtr<iSourceMesh>& rdi )
	{
		if( rsi.triangles.empty() || rsi.vertices.empty() )
			return OLE_E_BLANK;
		if( rsi.triangles.size() > UINT_MAX || rsi.vertices.size() > UINT_MAX )
			return DISP_E_OVERFLOW;
		rdi.release();
		return mr->createIndexedMeshFP32(
		  (uint32_t)rsi.vertices.size(), (const float*)rsi.vertices.data(), (uint32_t)rsi.triangles.size(), (const uint32_t*)rsi.triangles.data(), &rdi );
	}

	HRESULT copyMesh( iResultMesh* rsi, IndexedMesh& rdi )
	{
		uint32_t verts, tris;
		CHECK( rsi->getSize( verts, tris ) );

		try
		{
			rdi.vertices.resize( verts );
			rdi.triangles.resize( tris );
		}
		catch( const std::bad_alloc& )
		{
			return E_OUTOFMEMORY;
		}

		CHECK( rsi->getVertexBufferFP32( (float*)rdi.vertices.data() ) );
		CHECK( rsi->getIndexBuffer( (uint32_t*)rdi.triangles.data() ) );
		return S_OK;
	}
}  // namespace MeshRepair

HRESULT testStlIO()
{
	IndexedMesh mesh;
	CHECK( mesh.loadBinaryStl( stlSource ) );
	CHECK( mesh.saveBinaryStl( stlResult ) );
	return S_OK;
}

HRESULT testRepair()
{
	IndexedMesh mesh;
	CHECK( mesh.loadBinaryStl( stlSource ) );

	using namespace ComLight;
	using namespace MeshRepair;
	CComPtr<iMeshRepair> repair;
	CHECK( createMeshRepair( &repair ) );

	CComPtr<iSourceMesh> source;
	CHECK( createMesh( repair, mesh, source ) );

	Parameters params;
	CComPtr<iResultMesh> result;
	CHECK( repair->repair( source, params, &result ) );

	CHECK( copyMesh( result, mesh ) );
	CHECK( mesh.saveBinaryStl( stlResult ) );
	return S_OK;
}

int main()
{
	// testStlIO();
	testRepair();
	printf( "Hello World!\n" );
}