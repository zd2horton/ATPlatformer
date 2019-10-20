cbuffer MatrixBuffer
{
	matrix world_matrix;
	matrix view_matrix;
	matrix projection_matrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


PixelInputType TextureVertexShader(VertexInputType input_vertex)
{
    PixelInputType result;
    input_vertex.position.w = 1.0f;

    result.position = mul(input_vertex.position, world_matrix);
    result.position = mul(result.position, view_matrix);
    result.position = mul(result.position, projection_matrix);
    
    result.tex = input_vertex.tex;
    return result;
}