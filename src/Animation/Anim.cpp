

#include "Anim.h"
#include "Constants.h"
#include "MathEngine.h"
#include "MathApp.h"
#include "Mixer.h"
#include "PCSTreeForwardIterator.h"
#include "GameObject/GameObjectManager.h"
#include "ClipManager.h"
#include "SkeletonManager.h"

namespace Azul
{
	

	Anim::Anim(SkeletonHierarchy::Name SkelName, Clip::Name ClipName)
		:	pClip(nullptr),
		poBoneResult(nullptr),
		poSkeleton(nullptr),
		
		boneWidth(BONE_WIDTH)
	{
		SkeletonHierarchy* pSkelHier = SkeletonManager::Find(SkelName);
		this->numBones = pSkelHier->numBones;

		this->poBoneResult = new Bone[(unsigned int)numBones]();
		assert(this->poBoneResult);

		this->poSkeleton = new Skeleton(this->poBoneResult, pSkelHier);
		assert(this->poSkeleton);

		this->pClip = ClipManager::Find(ClipName);
		assert(pClip);

	}

	Anim::~Anim()
	{
		delete[] poBoneResult;
		delete poSkeleton;
	}

	Time Anim::FindMaxTime()
	{
		assert(pClip);
		return this->pClip->GetTotalTime();
	}

	Skeleton* Anim::GetSkel()
	{
		return this->poSkeleton;
	}

	void Anim::Animate(Time tCurr)
	{
		assert(pClip);
		this->pClip->AnimateBones(tCurr, this->poBoneResult);
	}

	void Anim::PoseSkeleton(Time tCurr)
	{
		PCSTreeForwardIterator pIter(this->poSkeleton->GetFirstBone());
		PCSNode *pNode = pIter.First();
		GameObjectAnim *pGameObj = nullptr;

		// walks the anim node does the pose for everything that
		//while (pNode != nullptr)
		for(int i = 0; i < numBones; i++)
		{
			assert(pNode);
			// Update the game object
			pGameObj = (GameObjectAnim *)pNode;
			privSetBonePose(pGameObj);

			pNode = pIter.Next();
		}
	}

	void Anim::privSetBonePose(GameObjectAnim *node)
	{
		// Now get the world matrices
		// getting the parent from current node
		GameObjectAnim *childNode = (GameObjectAnim *)node;
		GameObjectAnim *parentNode;

		/*if (node->GetParent()->GetParent() == nullptr) {
			return;
		}*/
		if (node->GetParent()->GetParent()->GetParent() != nullptr)
		{
			parentNode = (GameObjectAnim *)node->GetParent();
		}
		else
		{
			parentNode = nullptr;
		}

		PCSTree *pTree = GameObjectManager::GetPCSTree();
		assert(pTree);
		if (parentNode == pTree->GetRoot())
			return;

		if (parentNode != nullptr && childNode != nullptr)
		{
			// starting point
			Vect start(0.0f, 0.0f, 0.0f);

			//  At this point, Find the two bones initial positions in world space
			//  Now get the length and directions

			Vect ptA = start * *parentNode->GetWorld();
			Vect ptB = start * *childNode->GetWorld();

			// direction between the anchor points of the respective bones
			Vect dir = (ptA - ptB);

			// length of the bone 0
			float mag = dir.mag();

			Matrix S(Matrix::Scale::XYZ, BONE_WIDTH, BONE_WIDTH, mag);
			Quat Q(Quat::Orient::LocalToWorld, dir.getNorm(), Vect(0.0f, 1.0f, 0.0f));
			Matrix T(Matrix::Trans::XYZ, ptB);

			Matrix BoneOrient = S * Q * T;

			childNode->SetBoneOrientation(BoneOrient);
		}
	}


}

//--- End of File ----
