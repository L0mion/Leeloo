#ifndef LEELOO_INTERSECT_HLSL
#define LEELOO_INTERSECT_HLSL

#include "ConstantBuffers.hlsl"
#include "Structs.hlsl"
#include "Globals.hlsl"

#define FLT_MAX 3.402823466e+38F
#define FLT_MIN 1.175494351e-38F

Intersection intersectSphere(Ray ray, Sphere sphere)
{
	Intersection intersection;
	intersection.m_barCoord = float2(-1.0f, -1.0f);
	intersection.m_range = -1.0f;
	intersection.m_triangleId = -1;
	intersection.m_sphereId = -1;

	bool intersect = false;
	float b = dot(ray.m_direction, ray.m_origin-sphere.m_center);
	float c = dot(ray.m_origin-sphere.m_center, ray.m_origin-sphere.m_center) - (sphere.m_radius * sphere.m_radius);
	
	float t = b*b - c;
	
	if(t >= 0)
	{
		float t1 = -b + sqrt(t);
		float t2 = -b - sqrt(t);
		
		intersection.m_range = min(t1, t2);
		intersection.m_barCoord = float2(0.0f, 0.0f);
	}
		
	return intersection;
}

Intersection intersectTriangle(Ray ray, float3 pos1, float3 pos2, float3 pos3)
{
	Intersection intersection;
	intersection.m_barCoord = float2(-1.0f, -1.0f);
	intersection.m_range = -1.0f;
	intersection.m_triangleId = -1;
	intersection.m_sphereId = -1;

	bool intersect = true;

	float3 e1 = pos2 - pos1;
	float3 e2 = pos3 - pos1;
	float3 q  = cross(ray.m_direction, e2);
	
	float a = dot(e1, q);
	if(a > -0.00001f && a < 0.00001)
		intersect = false;
		
	float f = 1/a;
	float3 s = ray.m_origin - pos1;
	float u =  f*(dot(s, q));
	
	if(u < 0.0f)
		intersect = false;
	
	float3 r = cross(s, e1);
	float v = f*(dot(ray.m_direction, r));
	
	if(v < 0.0f || v+u > 1.0f)
		intersect = false;
	
	if( intersect )
	{
		intersection.m_barCoord = float2(u, v);
		intersection.m_range = dot(e2, r) * f;
	}

	return intersection;
}

Intersection intersectTest(Ray ray, uint index)
{
	float minDistance = ray.m_maxDistance;
	
	bool intersect = false;
	Intersection intersection;
	Intersection bestIntersection;
	bestIntersection.m_barCoord = float2(-1.0f, -1.0f);
	bestIntersection.m_range = ray.m_maxDistance;
	bestIntersection.m_triangleId = -1;
	bestIntersection.m_sphereId = -1;

	float3 pos1, pos2, pos3;
	if(ray.m_triangleId > -2)
	{
		for(int i=0; i<cb_numTriangles; i++)
		{	
			pos1 = g_triangles[i].m_vertices[0].m_position;
			pos2 = g_triangles[i].m_vertices[1].m_position;
			pos3 = g_triangles[i].m_vertices[2].m_position;
			intersection = intersectTriangle(ray, pos1, pos2, pos3);
			
			if(	ray.m_triangleId != i &&
				/*intersection.m_barCoord.x >= 0.0f &&*/
				intersection.m_range < bestIntersection.m_range &&
				intersection.m_range > 0.0f)
			{
				bestIntersection = intersection;
				bestIntersection.m_triangleId = i;
				bestIntersection.m_sphereId = -1;
				intersect = true;
			}
		}
		for(int j=0; j<cb_numSpheres; j++)
		{
			intersection = intersectSphere(ray, g_spheres[j]);
			if(ray.m_sphereId != j &&
				intersection.m_range < bestIntersection.m_range &&
				intersection.m_range > 0.0f)
			{
				bestIntersection = intersection;
				bestIntersection.m_sphereId = j;
				bestIntersection.m_triangleId = -1;
				intersect = true;
			}
		}

		if(!intersect)
		{
			bestIntersection.m_triangleId = -2;
			bestIntersection.m_sphereId = -2;
			g_rays[index].m_triangleId = -2;
			g_rays[index].m_sphereId = -2;
		}
	}

	return bestIntersection;
}

//bool intersectBox(Ray ray, Node node)
//{
//	float tMax = FLT_MAX;
//	float tMin = -FLT_MAX;
//
//	float3 ac = node.m_min + ((node.m_max - node.m_min)/2);
//	float3 a[3];
//	a[0] = float3(1.0f, 0.0f, 0.0f);
//	a[1] = float3(0.0f, 1.0f, 0.0f);
//	a[2] = float3(0.0f, 0.0f, 1.0f);
//	float h[3];
//	h[0] = (node.m_max.x - node.m_min.x)/2;
//	h[1] = (node.m_max.y - node.m_min.y)/2;
//	h[2] = (node.m_max.z - node.m_min.z)/2;
//
//	float3 p = ac - ray.m_origin;
//
//	float e;
//	float f;
//	float t1;
//	float t2;
//	float temp;
//
//	for(int i=0; i<3; i++)
//	{
//		e = dot(a[i], p);
//		f = dot(a[i], ray.m_direction);
//		if(sqrt(f*f) > FLT_MIN)
//		{
//			t1 = (e + h[i])/f;
//			t2 = (e - h[i])/f;
//			if(t1 > t2)
//			{
//				temp = t1;
//				t1 = t2;
//				t2 = temp;
//			}
//			if(t1 > tMin)
//				tMin = t1;
//			if(t2 < tMax)
//				tMax = t2;
//			if(tMin > tMax)
//				return false;
//			if(tMax < 0)
//				return false;
//		}
//		else if((-e-h[i])>0 || -e+h[i]<0)
//			return false;
//	}
//	return true;
//}
//
//float2 BVH_IntersectBox(float3 vfStart,float3 vfInvDir, Node node) 
//{	
//	float2 T;
//
//	float3 vfDiffMax = node.m_max-vfStart;
//	vfDiffMax *= vfInvDir;
//	float3 vfDiffMin = node.m_min-vfStart;
//	vfDiffMin *= vfInvDir;
//
//	T[0] = min(vfDiffMin.x,vfDiffMax.x);
//	T[1] = max(vfDiffMin.x,vfDiffMax.x);
//
//	T[0] = max(T[0],min(vfDiffMin.y,vfDiffMax.y));
//	T[1] = min(T[1],max(vfDiffMin.y,vfDiffMax.y));
//
//	T[0] = max(T[0],min(vfDiffMin.z,vfDiffMax.z));
//	T[1] = min(T[1],max(vfDiffMin.z,vfDiffMax.z));
//
//	//empty interval
//	if (T[0] > T[1])
//	{
//		T[0] = T[1] = -1.0f;
//	}
//
//	return T;
//}

#endif //LEELOO_INTERSECT_HLSL