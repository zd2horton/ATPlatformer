//global variables declared, placed in buffer object types called cbuffer

cbuffer MatrixBuffer
{
	matrix world_matrix;
	matrix view_matrix;
	matrix proojection_matrix;
};

//create types with x, y, z, w position vectors and rgb alpha colours
//position usable by vertex, sv_position usable by pixel

struct VertexInputType
{
	float4 position : POSITION;
	float4 colour: COLOUR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 colour: COLOUR;
};

//input to the vertex shader must match the data format in the buffer and the VIT above
//output of vertex shader sent to pixel shader (PIT type)
//position of vertex taken then multiplied by matrixes for correct positioning
//output variable then takes copy of input colour, output used for pixel shader


PixelInputType ColourVortexShader(VertexInputType input_vertex)
{
	PixelInputType result;

	//change position vector to be 4 units for correct calculations
	input_vertex.position.w = 1.0f;

	//calc. position of vertex against all matrixes
	result.position = mul(input_vertex.position, world_matrix);
	result.position = mul(result.position, view_matrix);
	result.position = mul(result.position, proojection_matrix);

	//store input colour for pixel shader use
	result.colour = input_vertex.colour;
	return result;
}