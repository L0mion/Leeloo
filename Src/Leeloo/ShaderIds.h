#ifndef LEELOO_SHADER_IDS_H
#define LEELOO_SHADER_IDS_H

struct ShaderIds
{
	enum Id
	{
		ShaderIds_PRIMARY_RAY_STAGE,
		ShaderIds_INTERSECTION_STAGE,
		ShaderIds_COLOR_STAGE,

		ShaderIds_COUNT
	};
};

#endif //LEELOO_SHADER_IDS_H