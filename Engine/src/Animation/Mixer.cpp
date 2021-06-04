

#include "Mixer.h"
#include "MathApp.h"
#include "Shaders/ShaderManager.h"

namespace Azul
{
	void Mixer::Blend(Bone *pResult,
		const Bone *pA,
		const Bone *pB,
		const float tS,
		int numBones)
	{

		// Interpolate to tS time, for all bones
		for (int i = 0; i < numBones; i++)
		{
			// interpolate ahoy!
			VectApp::Lerp(pResult->T, pA->T, pB->T, tS);
			QuatApp::Slerp(pResult->Q, pA->Q, pB->Q, tS);
			VectApp::Lerp(pResult->S, pA->S, pB->S, tS);

			// advance the pointer
			pA++;
			pB++;
			pResult++;
		}

	}

	void Mixer::Blend_GPU(
						SSBO* pKey_A,
						SSBO* pKey_B,
						SSBO* pResult_GPU,
						const float tS,
						int numBones)
	{
		assert(pKey_A != nullptr);
		assert(pKey_B != nullptr);
		assert(pResult_GPU != nullptr);

		// Lock and load the shader...
		ShaderObject* pComputeShader = ShaderManager::Find(ShaderObject::Name::COMPUTE_MIXER);

		pComputeShader->SetActive();

		pKey_A->Bind(0);
		pKey_B->Bind(1);
		pResult_GPU->Bind(2);

		glUniform1f(pComputeShader->GetLocation("ts"), tS);
		glUniform1i(pComputeShader->GetLocation("boneCount"), numBones);

		// Dispatch
		pComputeShader->Dispatch((unsigned int)numBones / 20, 1, 1);

		// Block Waiting for stages to complete
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		
	}

}

// --- End of File ---
