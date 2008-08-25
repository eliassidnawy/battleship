//basic fx file that transforms the geometry from local space to homogenous clip space
//and draws the geometry in wireframe mode

//FX parameter that holds the combined World, View, and Projection Matrices
uniform extern float4x4 gWVP;


//structure
struct OutputVS
{
       float4 posH : POSITION0;
};

//Vertex Shader
OutputVS TransformVS(float3 posL : POSITION0)
{
  //zero out our structure
  OutputVS outVS = (OutputVS)0;
  
  //transform to homogenous clip space
  outVS.posH = mul(float4(posL, 1.0f), gWVP);
  
  //Done - return the output
  return outVS;
}

//Pixel Shader
float4 TransformPS() : COLOR
{
  //r,g,b,a
  return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

//technique with one pass
technique TransformTech
{
  pass P0
  {
    //specify vertex and pixel shader associated with this pass
    vertexShader = compile vs_2_0 TransformVS();
    pixelShader = compile ps_2_0 TransformPS();

    //specify render/device states associated with this pass
    Lighting = true;
    FillMode = Wireframe;
  }
}