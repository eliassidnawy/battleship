#include "d3dUtil.h"
#include "Vertex.h"

//function: LoadXFile
/*  This function takes 4 paramaters:
    filename: name of the .X file to load
    meshOut: the address of an ID3DXMesh object
    mtrls: list of materials, datatype is a vector of type Mtrl
    texs: list of textures, datatype is a vector of type IDirect3DTexture9*

    This function will perform 6 steps:
    1. Load info from .X file
    2. Determine if mesh already has normal info
    3. Change the vertex format to one capitalble with ours
    4. Generate normals if mesh doesn't have them
    5. Optimize
    6. Extract materials and load textures
*/


void LoadXFile(const std::string & filename, ID3DXMesh** meshOut, std::vector<Mtrl> & mtrls, std::vector<IDirect3DTexture9*> & texs)
{
	ID3DXMesh* meshSys				= 0;
	ID3DXBuffer * adjBuffer			= 0;
	ID3DXBuffer * mtrlBuffer		= 0;
	DWORD numMtrls					= 0;

    //step 1. Load the x file into system memory
    HR(D3DXLoadMeshFromX(filename.c_str(), D3DXMESH_SYSTEMMEM, gd3dDevice, &adjBuffer, &mtrlBuffer, 0, &numMtrls, &meshSys));

    //step 2. look into MAX_FVF_DECL_SIZE
    D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
    HR(meshSys->GetDeclaration(elems));

    bool hasNormals = false;
	D3DVERTEXELEMENT9 term = D3DDECL_END();
    for(int i = 0; i < MAX_FVF_DECL_SIZE; ++i)
    {
        //did we reach end?
        if(elems[i].Stream == 0xff)
            break;

        if(elems[i].Type == D3DDECLTYPE_FLOAT3 && elems[i].Usage == D3DDECLUSAGE_NORMAL && elems[i].UsageIndex == 0)
        {
            hasNormals = true;
            break;
        }
    }

    //step 3.
    D3DVERTEXELEMENT9 elements[64];
    UINT numElements = 0;
    VertexPNT::Decl->GetDeclaration(elements, &numElements);
   
    ID3DXMesh * temp = 0;
    //HR(meshSys->CloneMesh(D3DXMESH_SYSTEMMEM, elements, gd3dDevice, &temp));
	 HR(meshSys->CloneMesh(D3DXMESH_32BIT, elements, gd3dDevice, &temp));
	ReleaseCOM(meshSys);
	meshSys = temp;

    //step 4
    if( hasNormals == false )
        HR(D3DXComputeNormals(meshSys, 0));

    //step 5
    HR(meshSys->Optimize(D3DXMESH_MANAGED | D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT
        | D3DXMESHOPT_VERTEXCACHE, (DWORD*)adjBuffer->GetBufferPointer(), 0, 0, 0, meshOut));

    ReleaseCOM(meshSys); //Done w/ system mesh
    ReleaseCOM(adjBuffer); //done with buffer

    //step 6: get the materials and load the textures
    if(mtrlBuffer != 0 && numMtrls != 0)
    {
		D3DXMATERIAL *d3dxMtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
		
		for(DWORD i = 0; i < numMtrls; ++i)
		{
			//save the ith material.  MatD3D ambient
			//doesnt have a default value, so I'm setting
			//it to be the same as the diffuse
			Mtrl m;
			m.ambient = d3dxMtrls[i].MatD3D.Diffuse;
			m.diffuse = d3dxMtrls[i].MatD3D.Diffuse;
			m.spec = d3dxMtrls[i].MatD3D.Specular;
			m.specPower = d3dxMtrls[i].MatD3D.Power;
			mtrls.push_back(m);

			//check if the ith material has an associative texture
			if(d3dxMtrls[i].pTextureFilename != 0)
			{
				//yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;
				char *texFN = d3dxMtrls[i].pTextureFilename;
				HR(D3DXCreateTextureFromFile(gd3dDevice, texFN, &tex));

				//save the loaded texure
				texs.push_back(tex);
			}
			else
			{
				//no texture
				texs.push_back( 0 );
			}
		}
	}

	ReleaseCOM(mtrlBuffer); // done with the buffer
}