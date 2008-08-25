#include "Vertex.h"
#include "d3dUtil.h"

//initialize static variables
IDirect3DVertexDeclaration9 * VertexPos::Decl = 0;
IDirect3DVertexDeclaration9 * VertexPN::Decl  = 0;
IDirect3DVertexDeclaration9 * VertexPNT::Decl = 0;

void InitAllVertexDeclarations()
{
	//this vertex structure only has a position element to it
	//VertexPos
	D3DVERTEXELEMENT9 VertexPosElements[] = 
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,0},
		D3DDECL_END()
	};

	//create the vertex declaration
	HR(gd3dDevice->CreateVertexDeclaration(VertexPosElements, &VertexPos::Decl));

	//this vertex structure has a position and a normal
	//VertexPN
	D3DVERTEXELEMENT9 VertexPNElements[] = 
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},

		D3DDECL_END()
	};

	//create the vertex declaration
	HR(gd3dDevice->CreateVertexDeclaration(VertexPNElements, &VertexPN::Decl));

	//this vertex has a position, normal, and texture coords
	//VertexPNT
	D3DVERTEXELEMENT9 VertexPNTElements[] = 
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},

		D3DDECL_END()
	};

	//create the vertex declaration
	HR(gd3dDevice->CreateVertexDeclaration(VertexPNTElements, &VertexPNT::Decl));
}

void DestroyAllVertexDeclarations()
{
	ReleaseCOM(VertexPos::Decl);
	ReleaseCOM(VertexPN::Decl);
	ReleaseCOM(VertexPNT::Decl);
}