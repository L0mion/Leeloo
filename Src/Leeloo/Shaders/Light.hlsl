#ifndef LEELOO_LIGHT_HLSL
#define LEELOO_LIGHT_HLSL

#include "ConstantBuffers.hlsl"
#include "Structs.hlsl"

float3 pointLight(float3 normal, float3 position, float4 diffuse, float4 specular, Light light)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	float3 lightVec = position - light.pos;
	float d = length(lightVec);
	lightVec /= d;
	
	litColor += diffuse * light.ambient;
	
	float diffuseFactor = dot(lightVec, normal);
	if(diffuseFactor > 0.0f)
	{
		float specPower = max(specular.a, 1.0f);
		float3 toEye = normalize(cb_cameraPosition-position);
		float3 R = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);
	
		litColor += diffuseFactor * diffuse * light.diffuse;
		litColor += specFactor * specular * light.spec;
	}
	
	litColor /= dot(light.att, float3(1.0f, d, d*d));

	return litColor;
}

float3 shadow(float3 position, float4 diffuse, Light light)
{
	float3 lightVec = position - light.pos;
	float d = length(lightVec);
	lightVec /= d;

	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	litColor += diffuse * light.ambient;
	litColor /= dot(light.att, float3(1.0f, d, d*d));

	return litColor * 0.9;
}

#endif //LEELOO_LIGHT_HLSL