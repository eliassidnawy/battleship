//this is where we will store all the info needed for vertex's
//and their appropriate functions
#ifndef __VERTEX_H_
#define __VERTEX_H_

#include <d3dx9.h>

//call these in the constructor and deconstructor of our app
void InitAllVertexDeclarations();
void DestroyAllVertexDeclarations();

struct VertexPos
{
	VertexPos():pos(0.0f, 0.0f, 0.0f){}
	VertexPos(float x, float y, float z):pos(x, y, z){}
	VertexPos(const D3DXVECTOR3 & v):pos(v){}

	D3DXVECTOR3 pos;
	static IDirect3DVertexDeclaration9 * Decl;
};

struct VertexPN
{
	VertexPN():pos(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f){}
	
	VertexPN(float x, float y, float z, 
			  float nx, float ny, float nz):pos(x, y, z), normal(nx, ny, nz){}

	VertexPN(const D3DXVECTOR3 &v, const D3DXVECTOR3 &n):pos(v), normal(n){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;

	static IDirect3DVertexDeclaration9 * Decl;
};

struct VertexPNT
{
	VertexPNT():pos(0.0f, 0.0f, 0.0f),
				normal(0.0f, 0.0f, 0.0f),
				tex0(0.0f, 0.0f){}
	
	VertexPNT(float x, float y, float z, 
			  float nx, float ny, float nz,
			  float u, float v):pos(x, y, z), normal(nx, ny, nz), tex0(u,v){}

	VertexPNT(const D3DXVECTOR3 &v, const D3DXVECTOR3 &n, const D3DXVECTOR2 &uv):pos(v), normal(n), tex0(uv){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex0;

	static IDirect3DVertexDeclaration9 * Decl;
};
#endif