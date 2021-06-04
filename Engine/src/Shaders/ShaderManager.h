
#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "ShaderObject.h"
#include "Manager/Manager.h"


namespace Azul
{
	class ShaderManager : Manager
	{
	
	private:  // methods

		ShaderManager(int reserveNum = 3, int reserveGrow = 1);
		ShaderManager() = delete;
		ShaderManager(const ShaderManager&) = delete;
		ShaderManager& operator = (const ShaderManager&) = delete;
		~ShaderManager();

		static ShaderManager* GetInstance();
		


	public:
		static void Add(ShaderObject::Name NameIn, const char* const _assetName, ShaderObject::Type shaderType);
		static void Create(int reserveNum = 3, int reserveGrow = 1);
		static void Destroy();
		static ShaderObject* Find(ShaderObject::Name NameIn);

	protected:
		virtual DLink* DerivedCreateNode() override;
		virtual bool DerivedCompare(DLink* pLinkA, DLink* pLinkB) override;
		virtual void DerivedWash(DLink* pLink) override;
		virtual void DerivedDumpNode(DLink* pLink) override;
		virtual void DerivedDestroyNode(DLink* pLink) override;

	private:  

		//DATA
		static ShaderManager* poInstance;
		ShaderObject* poNodeCompare;
	};

}

#endif

// ---  End of File ---
