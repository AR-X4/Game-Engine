#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include "sb7.h"
#include "Manager/DLink.h"

namespace Azul
{
	class ShaderObject : public DLink
	{
	public:
		enum class Name
		{
			UNINITIALIZED,
			NULL_SHADER,
			COLOR_LIGHT,
			COLOR_SINGLE,
			TEXTURE_SIMPLE,
			TEXTURE_POINT_LIGHT,
			CONST_COLOR,
			SPRITE,
			SKIN_TEXTURE,
			SKIN_LIGHT_TEXTURE,
			SKIN_CONST_COLOR,
			COMPUTE_MIXER,
			COMPUTE_WORLD
		};

		enum class Type
		{
			GRAPHICS,
			COMPUTE,
			UNINITIALIZED
		};
		

		ShaderObject();
		ShaderObject(const ShaderObject&) = delete;
		ShaderObject& operator = (ShaderObject&) = delete;
		virtual ~ShaderObject() = default;

		void SetActive() const;
		GLint GetLocation(const char* const pUniformName) const;

		void Dump();
		void Wash();
		void Set(ShaderObject::Name shaderName, const char* const pShaderBaseFileName, ShaderObject::Type type);
		void SetName(ShaderObject::Name shaderName);

		void Dispatch(unsigned int groupsize_x, unsigned int groupsize_y, unsigned int groupsize_z);

	private:
		bool privCreateShader(GLuint& programObject, const char* const pShaderBaseFileName, Type shaderType);
		bool privLoadNCompile(GLuint& shaderObject, const char* const pShaderFileName, GLenum shader_type);

	public:
		Name		ShaderName;
		Type		type;
		GLuint		programObject;


	};
}

#endif

// ---  End of File ---
