#include "stdafx.h"
#include "SourceMesh.h"
using namespace MeshRepair;

HRESULT SourceMesh::createMesh( uint32_t countVertices, const float* vb, uint32_t countTriangles, const uint32_t* ib )
{
	if( countVertices == 0 || countTriangles == 0 )
		return E_INVALIDARG;
	if( nullptr == vb || nullptr == ib )
		return E_POINTER;

	
	CHECK( mesh.assignVertices( countVertices, vb ) );
	CHECK( mesh.assignTriangles( countTriangles, ib ) );

	// See MeshIO::load_mesh function in the original code
	mesh.reorderMorton();

	// Extract the data, store as different types
	CHECK( mesh.copyData( input_vertices, input_faces ) );
	try
	{
		input_tags.clear();
		input_tags.resize( input_faces.size(), 0 );
	}
	catch( const std::bad_alloc& )
	{
		return E_OUTOFMEMORY;
	}

#if 1
	// Compare with my custom mesh loading, and Morton reordering
	TriangleMesh testMesh;
	testMesh.assignVertices( countVertices, vb );
	testMesh.assignTriangles( countTriangles, ib );
	testMesh.reorderMorton();
	const double diagMy = testMesh.boxDiagonal();
	const double diagGg = mesh.boxDiagonal();
	assert( diagMy == diagGg );

	size_t cb = input_vertices.size() * 8 * 3;
	int cmp = memcmp( testMesh.vertexPointer(), input_vertices.data(), cb );
	assert( 0 == cmp );

	cb = input_faces.size() * 4 * 3;
	cmp = memcmp( testMesh.trianglePointer(), input_faces.data(), cb );
	assert( 0 == cmp );
#endif

	return S_OK;
}