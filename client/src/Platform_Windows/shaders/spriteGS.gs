struct VSIN
{
    float3 center 	: ANCHOR;
    float2 dimensions 	: DIMENSIONS;
};
 
struct PSIN
{
    float4 p : SV_POSITION;
    float2 t : TEXCOORD0;
};

[maxvertexcount(4)]
void GS( point VSIN sprite[1], inout TriangleStream<PSIN> triStream )
{
    PSIN v;
    //create sprite quad
    //--------------------------------------------
 
    //bottom left
    v.p = float4(sprite[0].center.x - sprite[0].dimensions.x * 0.5f,
		 sprite[0].center.y - sprite[0].dimensions.y * 0.5f,
		 (sprite[0].center.z + 1.0f)  * 0.5f,
		 1);
    v.t = float2(0,1);
    triStream.Append(v);
 
    //top left
    v.p = float4(sprite[0].center.x - sprite[0].dimensions.x * 0.5f,
		 sprite[0].center.y + sprite[0].dimensions.y * 0.5f,
		 (sprite[0].center.z + 1.0f) * 0.5f,
		 1);
    v.t = float2(0,0);
    triStream.Append(v);
 
    //bottom right
    v.p = float4(sprite[0].center.x + sprite[0].dimensions.x * 0.5f,
		 sprite[0].center.y - sprite[0].dimensions.y * 0.5f,
		 (sprite[0].center.z + 1.0f) * 0.5f,
		 1);
    v.t = float2(1,1);
    triStream.Append(v);
 
    //top right
    v.p = float4(sprite[0].center.x + sprite[0].dimensions.x * 0.5f,
		 sprite[0].center.y + sprite[0].dimensions.y * 0.5f,
		 (sprite[0].center.z + 1.0f) * 0.5f,
		 1);
    v.t = float2(1,0);
    triStream.Append(v);
}
