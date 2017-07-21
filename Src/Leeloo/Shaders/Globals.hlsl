#ifndef LEELOO_GLOBALS_HLSL
#define LEELOO_GLOBALS_HLSL

#include "Structs.hlsl"

RWTexture2D<float4> g_output : register(u0);

RWStructuredBuffer<float4>		 g_accumulation	 : register(u1);
RWStructuredBuffer<Intersection> g_intersections : register(u2);
RWStructuredBuffer<Ray>			 g_rays			 : register(u3);

StructuredBuffer<Triangle>	g_triangles	: register(t0);
StructuredBuffer<Material>	g_materials	: register(t1);
StructuredBuffer<Light>		g_lights	: register(t2);
StructuredBuffer<Sphere>	g_spheres	: register(t3);

Texture2DArray g_textures : register(t4);

SamplerState g_ss : register(s0);

#endif //LEELOO_GLOBALS_HLSL