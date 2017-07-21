#include "ConstantBuffers.hlsl"
#include "Structs.hlsl"
#include "IntersectTest.hlsl"
#include "Globals.hlsl"

[numthreads(BLOCK_SIZE_X, BLOCK_SIZE_Y, 1)]
void main(uint3 threadId : SV_DispatchThreadID, uint groupId : SV_GroupID)
{
	uint index = threadId.y * cb_screenWidth + threadId.x;

	Ray ray = g_rays[index];


	g_intersections[index] = intersectTest(ray, index);
}