#include "../render/common.hlsli"

struct VertexInputType
{
	float4 myPosition	:	POSITION;
	float4 myVxColor	:	COLOR0;
	float4 myVxColor2	:	COLOR1;
	float4 myVxColor3	:	COLOR2;
	float4 myVxColor4	:	COLOR3;
	float2 myUV			:	UV0;
	float2 myUV2		:	UV1;
	float2 myUV3		:	UV2;
	float2 myUV4		:	UV3;
	float3 myNormal		:	NORMAL;
	float3 myTangent	:	TANGENT;
	float3 myBiNormal	:	BINORMAL;
	float4 myBones      :   BONES;
	float4 myWeights    :   WEIGHTS;
	float4x4 myWorld	:	WORLD;
};

cbuffer BoneBuffer : register(b3)
{
	float4x4 myBones[MAX_ANIMATION_BONES];
};

cbuffer ModelObjectBuffer : register(b2) 
{
	float4x4 myObToWorld;
};

InstancedPixelInputType main(VertexInputType input)
{
    InstancedPixelInputType output;
	
	float2 Resolution = myResolution.xy;
	float ratio = Resolution.y / Resolution.x;
		
	float4 vertexWorldPos = mul(input.myWorld, input.myPosition);
	float4 vertexViewPos = mul(myToCamera, vertexWorldPos);

	output.worldPosition = vertexWorldPos;
	output.position = mul(myToProjection, vertexViewPos);
	output.aspectRatio = 1;
	output.tex = input.myUV;
	output.color = 1; 

    return output;  
}