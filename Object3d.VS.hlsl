float4 main( float4 pos : POSITION ) : SV_POSITION
{

struct VertexShaderOutput{
float32_t4 position : SV_POSITION;
};

struct VertexShadeInput {
float32_t4 position : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input){
VertexShanderOutput output;
output.position = input.position;
return output;
};



	return pos;
}