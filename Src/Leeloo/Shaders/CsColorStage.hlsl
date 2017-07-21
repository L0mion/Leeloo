#include "ConstantBuffers.hlsl"
#include "Structs.hlsl"
#include "Globals.hlsl"
#include "IntersectTest.hlsl"
#include "Light.hlsl"


void reflectRay(uint index, float3 position, float3 normal, int triangleId, int sphereId )
{
	Ray ray					= g_rays[index];
	ray.m_origin			= position;
	ray.m_direction			= reflect(ray.m_direction, normal);
	ray.m_direction			= normalize(ray.m_direction);
	ray.m_triangleId		= triangleId;
	//ray.m_sphereId			= sphereId;
	if(sphereId >= 0)
		ray.m_reflectiveFactor *= g_materials[g_spheres[sphereId].m_mtlIndex].m_specular;
	else
		ray.m_reflectiveFactor *= g_materials[g_triangles[triangleId].m_mtlIndex].m_specular;
	g_rays[index] = ray;
}


[numthreads(BLOCK_SIZE_X, BLOCK_SIZE_Y, 1)]
void main(uint3 threadId : SV_DispatchThreadID, uint groupId : SV_GroupID)
{
	uint index =  threadId.y * cb_screenWidth + threadId.x;

	Intersection intersection = g_intersections[index];

	Vertex vertexA;
	Vertex vertexB;
	Vertex vertexC;

	float4 color				= float4(0.0f, 0.0f, 0.0f, 1.0f);
	float3 litColor				= float3(0.0f, 0.0f, 0.0f);
	float2 barCoord				= float2(0.0f, 0.0f);
	float3 intersectPosition	= float3(0.0f, 0.0f, 0.0f);
	float3 intersectNormal		= float3(0.0f, 0.0f, 0.0f);
	float2 intersectTexture		= float2(0.0f, 0.0f);
	float4 diffuse				= float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular				= float4(0.0f, 0.0f, 0.0f, 0.0f);

	if(intersection.m_triangleId >= 0)
	{
		vertexA = g_triangles[intersection.m_triangleId].m_vertices[0];
		vertexB = g_triangles[intersection.m_triangleId].m_vertices[1];
		vertexC = g_triangles[intersection.m_triangleId].m_vertices[2];

		barCoord = intersection.m_barCoord;

		intersectPosition =	(1 - barCoord.x - barCoord.y) * vertexA.m_position +
							barCoord.x * vertexB.m_position +
							barCoord.y * vertexC.m_position;
		intersectNormal =	(1 - barCoord.x - barCoord.y) * vertexA.m_normal +
							barCoord.x * vertexB.m_normal +
							barCoord.y * vertexC.m_normal;
		
		intersectTexture =	(1 - barCoord.x - barCoord.y) * vertexA.m_texCoord +
										barCoord.x * vertexB.m_texCoord +
									barCoord.y * vertexC.m_texCoord;
		intersectNormal = normalize(intersectNormal);

		color	 = g_textures.SampleLevel(g_ss, float3(intersectTexture, g_triangles[intersection.m_triangleId].m_mtlIndex), 0);
		diffuse	 = g_materials[g_triangles[intersection.m_triangleId].m_mtlIndex].m_diffuse; 
		specular = g_materials[g_triangles[intersection.m_triangleId].m_mtlIndex].m_specular; 
	
		specular.a *= 256.0f;
		bool inShadow = false;
		for(uint lightIndex = 0; lightIndex < cb_numLights; lightIndex++)
		{	
			inShadow = false;
			Light light = g_lights[lightIndex];
					
			Ray lightRay;
			lightRay.m_origin			= intersectPosition;
			lightRay.m_direction		= normalize(light.pos - intersectPosition);
			lightRay.m_maxDistance		= length(light.pos - intersectPosition);
			lightRay.m_triangleId		= intersection.m_triangleId;
			lightRay.m_reflectiveFactor	= float3(1.0f, 1.0f, 1.0f);
			lightRay.m_sphereId			= intersection.m_sphereId;
	
			Intersection shadowIntersection = intersectTest(lightRay, index);
			if( lightRay.m_triangleId != shadowIntersection.m_triangleId &&
				shadowIntersection.m_barCoord.x >= 0.0f &&
				shadowIntersection.m_range > 0.0001f)
			{
					inShadow = true;
			}
	
			if(inShadow)
			{
				litColor += shadow(intersectPosition, diffuse, light);//color * g_lights[lightIndex].ambient;
			}
			else
				litColor += pointLight(intersectNormal, intersectPosition, diffuse, specular, light);
	
			inShadow = false;
		}
	}
	else if(intersection.m_sphereId >= 0)
	{
		color		= g_materials[g_spheres[intersection.m_sphereId].m_mtlIndex].m_ambient;
		diffuse		= g_materials[g_spheres[intersection.m_sphereId].m_mtlIndex].m_diffuse;//float4(0.0f, 0.5f, 0.0f, 1.0f);
		specular	= g_materials[g_spheres[intersection.m_sphereId].m_mtlIndex].m_specular;//float4(0.99f, 0.99f, 0.99f, 1.0f); 
	
		intersectPosition = g_rays[index].m_origin +intersection.m_range*g_rays[index].m_direction;
		intersectNormal = g_spheres[intersection.m_sphereId].m_center - intersectPosition;
		intersectNormal = normalize(intersectNormal);

		specular.a *= 256.0f;
		bool inShadow = false;
		for(uint lightIndex = 0; lightIndex < cb_numLights; lightIndex++)
		{	
			inShadow = false;
			Light light = g_lights[lightIndex];
					
			Ray lightRay;
			lightRay.m_origin			= intersectPosition;
			lightRay.m_direction		= normalize(light.pos - intersectPosition);
			lightRay.m_maxDistance		= length(light.pos - intersectPosition);
			lightRay.m_triangleId		= intersection.m_triangleId;
			lightRay.m_reflectiveFactor	= float3(1.0f, 1.0f, 1.0f);
			lightRay.m_sphereId			= intersection.m_sphereId;
	
			Intersection shadowIntersection = intersectTest(lightRay, index);
			if( lightRay.m_sphereId != shadowIntersection.m_sphereId &&
				shadowIntersection.m_barCoord.x >= 0.0f &&
				shadowIntersection.m_range > 0.0001f)
			{
					inShadow = true;
			}
	
			if(inShadow)
			{
				litColor += shadow(intersectPosition, diffuse, light);//color * g_lights[lightIndex].ambient;
			}
			else
				litColor += pointLight(intersectNormal, intersectPosition, diffuse, specular, light);
	
			inShadow = false;
		}
	}

	g_accumulation[index] += color * float4(litColor, 1.0f) * float4(g_rays[index].m_reflectiveFactor, 1.0f);
	reflectRay(index, intersectPosition, intersectNormal, intersection.m_triangleId, intersection.m_sphereId);
	g_output[threadId.xy] = g_accumulation[index];
}