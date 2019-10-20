cbuffer MatrixBuffer
{
	matrix world_matrix;
	matrix view_matrix;
	matrix projection_matrix;
};

//3 float normal vectors used for calculating amount of light, using angle between
//direction of normal and direction of light 
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};


PixelInputType LightVertexShader(VertexInputType input_vertex)
{
    PixelInputType result;
    input_vertex.position.w = 1.0f;

    result.position = mul(input_vertex.position, world_matrix);
    result.position = mul(result.position, view_matrix);
    result.position = mul(result.position, projection_matrix);
    
    result.tex = input_vertex.tex;
	
	//normal vector for vertex calculated in world space, normalized after then
	//sent as input into pixel shader
	result.normal = mul(input_vertex.normal, (float3x3)world_matrix);
	result.normal = normalize(result.normal);
    return result;
}