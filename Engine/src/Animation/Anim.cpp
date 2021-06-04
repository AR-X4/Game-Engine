#include "Anim.h"
#include "Constants.h"
#include "MathEngine.h"
#include "MathApp.h"
#include "Mixer.h"
#include "PCSTreeForwardIterator.h"
#include "GameObject/GameObjectManager.h"
#include "ClipManager.h"
#include "Shaders/ShaderManager.h"

namespace Azul
{
	Anim::Anim(FBXSkinnedModel* pModelSkin, SSBO* pBoneWorld, Clip::Name ClipNameA)
		: pClip(nullptr),
		poSkeleton(nullptr),
		pHierarchyTable(pModelSkin->poHierarchy_SBO),
		pBoneWorld_SBO(pBoneWorld),
		hierarchyDepth(pModelSkin->HierarchyDepth)
	{
		this->numBones = (int)pModelSkin->NumBones;

		this->poSkeleton = new Skeleton();
		assert(this->poSkeleton);

		this->pClip = ClipManager::Find(ClipNameA);
		assert(pClip);

		pWorldComputeShader = ShaderManager::Find(ShaderObject::Name::COMPUTE_WORLD);
		assert(pWorldComputeShader);
	}
	

	Anim::~Anim()
	{
		delete poSkeleton;
	}

	AnimTime Anim::FindMaxTime()
	{
		assert(pClip);
		return this->pClip->GetTotalTime();
	}

	int Anim::GetNumBones() const
	{
		return numBones;
	}

	Skeleton* Anim::GetSkel() const
	{
		return this->poSkeleton;
	}

	void Anim::Animate(AnimTime tCurr)
	{
		assert(pClip);
		this->pClip->AnimateBones(tCurr);
	}


	void Anim::GpuComputeWorld(SSBO* Result)
	{
		pWorldComputeShader->SetActive();

		// need a clean way to store and hold this data...
		SSBO* pBoneLocal = Result;

		pBoneLocal->Bind(0);
		pHierarchyTable->Bind(1);
		pBoneWorld_SBO->Bind(2); 

		glUniform1i(pWorldComputeShader->GetLocation("tableDepth"), (int)hierarchyDepth);
		glUniform1i(pWorldComputeShader->GetLocation("boneCount"), (int)numBones);

		glUniformMatrix4fv(pWorldComputeShader->GetLocation("rigidWorld"), 1, GL_FALSE, (float*)poSkeleton->GetRigidBody()->GetWorld());

		// Dispatch
		pWorldComputeShader->Dispatch((unsigned int)numBones/20, 1, 1);

		// Block Waiting for stages to complete
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

}

//--- End of File ----
