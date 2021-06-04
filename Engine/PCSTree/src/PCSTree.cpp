

#include "PCSTree.h"
#include "PCSNode.h"
#include "PCSTreeForwardIterator.h"

namespace Azul
{
	// constructor
	PCSTree::PCSTree()
		:mInfo({}), pRoot(nullptr)
	{

	};

	// destructor
	PCSTree::~PCSTree()
	{

	};


	// get Root
	PCSNode* PCSTree::GetRoot() const
	{
		return this->pRoot;
	}


	// insert
	void PCSTree::Insert(PCSNode* const pInNode, PCSNode* const pParent)
	{
		assert(pInNode != nullptr);

		//-------link new node-------
		//check if root exists
		if (this->pRoot == nullptr) {
			assert(pParent == nullptr);

			this->pRoot = pInNode;

			this->mInfo.currNumNodes++;
			this->mInfo.currNumLevels++;
			this->mInfo.maxNumLevels++;
			this->mInfo.maxNumNodes++;

		}
		else {
			assert(pParent != nullptr);

			//try to get child siblings
			if (pParent->GetChild() != nullptr) {
				pInNode->SetNextSibling(pParent->GetChild());
				pParent->GetChild()->SetPrevSibling(pInNode);

				//Set Iterator ptrs
				pInNode->SetForward(pInNode->GetNextSibling());
				pInNode->GetNextSibling()->SetReverse(pInNode);


			}
			//else try to get sibling
			else if (pParent->GetNextSibling() != nullptr) {
				//Set Iterator ptrs
				pInNode->SetForward(pParent->GetNextSibling());
				pParent->GetNextSibling()->SetReverse(pInNode);


			}
			//else go up a level and try to get parent sibling
			else {

				PCSNode* pCurrent = pParent;
				while (pCurrent->GetParent() != nullptr) {

					if (pCurrent->GetParent()->GetNextSibling() != nullptr) {

						//Set Iterator ptrs
						pCurrent = pCurrent->GetParent()->GetNextSibling();
						pInNode->SetForward(pCurrent);
						pCurrent->SetReverse(pInNode);
						break;
					}
					else {
						pCurrent = pCurrent->GetParent();
					}
				}
			}

			//Set Parent/Child Relationship
			pParent->SetForward(pInNode);
			pInNode->SetReverse(pParent);

			pParent->SetChild(pInNode);
			pInNode->SetParent(pParent);

			//-------update stats-------
			//update num nodes
			this->mInfo.currNumNodes++;
			if (this->mInfo.currNumNodes > this->mInfo.maxNumNodes) {
				this->mInfo.maxNumNodes = this->mInfo.currNumNodes;
			}

			//walk to root and update num levels
			PCSNode* current = pInNode;
			int count = 0;
			while (current != nullptr) {
				count++;
				current = current->GetParent();
			}
			if (this->mInfo.currNumLevels < count) {
				this->mInfo.currNumLevels = count;
			}
			if (this->mInfo.currNumLevels > this->mInfo.maxNumLevels) {
				this->mInfo.maxNumLevels = this->mInfo.currNumLevels;
			}
		}
	}

	// Remove
	void PCSTree::Remove(PCSNode* const pInNode)
	{
		assert(pInNode != nullptr);

		//------recursively remove children from bottom up to root-------
		while (pInNode->GetChild() != nullptr) {
			PCSNode* CurrentChild = pInNode->GetChild();
			this->Remove(CurrentChild);
		}

		//check not removing root
		if (pInNode != this->pRoot) {

			//------update links------

			//check if next sibling
			if (pInNode->GetNextSibling() != nullptr) {
				pInNode->GetNextSibling()->SetPrevSibling(pInNode->GetPrevSibling());
			}
			//check if prev sibling
			if (pInNode->GetPrevSibling() != nullptr) {
				pInNode->GetPrevSibling()->SetNextSibling(pInNode->GetNextSibling());
			}
			//else only child
			else {
				pInNode->GetParent()->SetChild(pInNode->GetNextSibling());
			}
			//update Iterator ptrs
			if (pInNode->GetForward() != nullptr) {
				pInNode->GetForward()->SetReverse(pInNode->GetReverse());
			}
			//Reverse never nullptr
			pInNode->GetReverse()->SetForward(pInNode->GetForward());
		}

		//------update current stats-------
		this->mInfo.currNumNodes--;

		//walk tree and count levels
		PCSNode* Current = this->pRoot;
		int CurrentLevel = 1;
		int DeepestLevel = CurrentLevel;
		while (Current != nullptr) {

			//try to get a child
			if (Current->GetChild() != nullptr) {
				CurrentLevel++;
				Current = Current->GetChild();
			}
			//else try to get a sibling
			else if (Current->GetNextSibling() != nullptr) {

				Current = Current->GetNextSibling();
			}
			//else go back up a level and check for sibling
			else {
				if (CurrentLevel > DeepestLevel) {
					DeepestLevel = CurrentLevel;
				}

				while (Current->GetParent() != nullptr) {
					CurrentLevel--;

					if (Current->GetParent()->GetNextSibling() != nullptr) {
						Current = Current->GetParent()->GetNextSibling();
						break;
					}
					else {
						Current = Current->GetParent();
					}
				}
				if (Current == this->pRoot) {
					break;
				}
			}
		}

		//check update current num levels
		if (this->mInfo.currNumLevels > DeepestLevel) {
			this->mInfo.currNumLevels = DeepestLevel;
		}

		//--------clear node--------
		pInNode->SetParent(nullptr);
		pInNode->SetPrevSibling(nullptr);
		pInNode->SetNextSibling(nullptr);
		pInNode->SetChild(nullptr);

		if (pInNode == this->pRoot) {
			this->pRoot = nullptr;
			this->mInfo.currNumLevels = 0;
		}
	}

	// get tree info
	void PCSTree::GetInfo(Info& info)
	{
		info.currNumNodes = this->mInfo.currNumNodes;
		info.currNumLevels = this->mInfo.currNumLevels;
		info.maxNumLevels = this->mInfo.maxNumLevels;
		info.maxNumNodes = this->mInfo.maxNumNodes;
	}

	void PCSTree::Print() const
	{
		//depth walk tree and print nodes
		PCSTreeForwardIterator pIt = PCSTreeForwardIterator(this->pRoot);

		while (!pIt.IsDone()) {
			pIt.CurrentItem()->PrintNode();
			pIt.Next();
		}

	}
}

// ---  End of File ---------------
