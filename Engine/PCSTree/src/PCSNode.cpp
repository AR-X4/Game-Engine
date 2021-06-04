

#include "PCSTree.h"
#include "PCSNode.h"

namespace Azul
{

	// constructor
	PCSNode::PCSNode()
		:pParent(nullptr), pChild(nullptr), pNextSibling(nullptr), pPrevSibling(nullptr), pForward(nullptr), pReverse(nullptr)
	{

	}

	// copy constructor
	PCSNode::PCSNode(const PCSNode& in)
	{
		assert(&in != nullptr);

		this->pParent = in.pParent;
		this->pChild = in.pChild;
		this->pNextSibling = in.pNextSibling;
		this->pPrevSibling = in.pPrevSibling;

		this->SetName(in.pName);
	}

	// Specialize Constructor
	PCSNode::PCSNode(PCSNode* const pInParent, PCSNode* const pInChild, PCSNode* const pInNextSibling, PCSNode* const pInPrevSibling, const char* const pInName)
	{
		assert(pInParent != nullptr);
		assert(pInChild != nullptr);
		assert(pInNextSibling != nullptr);
		assert(pInPrevSibling != nullptr);
		assert(pInName != nullptr);

		this->pParent = pInParent;
		this->pChild = pInChild;
		this->pNextSibling = pInNextSibling;
		this->pPrevSibling = pInPrevSibling;

		this->SetName(pInName);
	}

	PCSNode::PCSNode(const char* const pInName)
	{
		assert(pInName != nullptr);

		this->pParent = nullptr;
		this->pChild = nullptr;
		this->pNextSibling = nullptr;
		this->pPrevSibling = nullptr;

		this->SetName(pInName);
	}

	// destructor
	PCSNode::~PCSNode()
	{
		//nothing to release?
	}

	// assignment operator
	PCSNode& PCSNode::operator = (const PCSNode& in)
	{
		assert(&in != nullptr);

		this->pParent = in.pParent;
		this->pChild = in.pChild;
		this->pNextSibling = in.pNextSibling;
		this->pPrevSibling = in.pPrevSibling;

		this->SetName(in.pName);

		return *this;
	}

	void PCSNode::SetParent(PCSNode* const pIn)
	{
		this->pParent = pIn;
	}

	void PCSNode::SetChild(PCSNode* const pIn)
	{
		this->pChild = pIn;
	}

	void PCSNode::SetNextSibling(PCSNode* const pIn)
	{
		this->pNextSibling = pIn;
	}

	void PCSNode::SetPrevSibling(PCSNode* const pIn)
	{
		this->pPrevSibling = pIn;
	}

	void PCSNode::SetForward(PCSNode* const pIn)
	{
		this->pForward = pIn;
	}

	void PCSNode::SetReverse(PCSNode* const pIn)
	{
		this->pReverse = pIn;
	}

	PCSNode* PCSNode::GetParent(void) const
	{
		return this->pParent;
	}

	PCSNode* PCSNode::GetChild(void) const
	{
		return this->pChild;
	}

	PCSNode* PCSNode::GetNextSibling(void) const
	{
		return this->pNextSibling;
	}

	PCSNode* PCSNode::GetForward(void) const
	{
		return this->pForward;
	}

	PCSNode* PCSNode::GetReverse(void) const
	{
		return this->pReverse;
	}

	PCSNode* PCSNode::GetPrevSibling(void) const
	{
		return this->pPrevSibling;
	}

	PCSNode::Code PCSNode::SetName(const char* const pInName)
	{
		if (pInName == nullptr) {
			return Code::FAIL_NULL_PTR;
		}

		memcpy_s(this->pName, PCSNode::NAME_SIZE, pInName, PCSNode::NAME_SIZE);

		return Code::SUCCESS;
	}

	PCSNode::Code PCSNode::GetName(char* const pOutBuffer, unsigned int sizeOutBuffer) const
	{

		if (pOutBuffer == nullptr) {
			return Code::FAIL_NULL_PTR;
		}
		else {
			memcpy_s(pOutBuffer, sizeOutBuffer, this->pName, sizeOutBuffer);
			pOutBuffer[sizeOutBuffer - 1] = 0;
		}

		return Code::SUCCESS;
	}

	void PCSNode::PrintNode() const
	{
		//print name
		Trace::out("Name:			%s\n", this->pName);

		//check if nullptr
		if (this->pParent != nullptr) {
			Trace::out("Parent:			%s\n", this->pParent->pName);
		}
		else {
			Trace::out("Parent:			NULL\n");
		}
		if (this->pChild != nullptr) {
			Trace::out("Child:			%s\n", this->pChild->pName);
		}
		else {
			Trace::out("Child:			NULL\n");
		}
		if (this->pNextSibling != nullptr) {
			Trace::out("Next Sibling:	%s\n", this->pNextSibling->pName);
		}
		else {
			Trace::out("Next Sibling:	NULL\n");
		}
		if (this->pPrevSibling != nullptr) {
			Trace::out("Prev Sibling:	%s\n", this->pPrevSibling->pName);
		}
		else {
			Trace::out("Prev Sibling:	NULL\n");
		}
		if (this->pReverse != nullptr) {
			Trace::out("Reverse:		%s\n", this->pReverse->pName);
		}
		else {
			Trace::out("Reverse:		NULL\n");
		}
		if (this->pForward != nullptr) {
			Trace::out("Forward:		%s\n\n", this->pForward->pName);
		}
		else {
			Trace::out("Forward:		NULL\n\n");
		}

	}

	void PCSNode::PrintChildren() const
	{
		//walk children and PrintNode()
		if (this->pChild != nullptr) {
			PCSNode* Current = this->pChild;
			while (Current != nullptr) {
				Current->PrintNode();
				Current = Current->pNextSibling;
			}
		}
	}

	void PCSNode::PrintSiblings() const
	{
		//walk siblings and PrintNode()
		if (this->pNextSibling != nullptr) {
			PCSNode* Current = this->pNextSibling;
			while (Current != nullptr) {
				Current->PrintNode();
				Current = Current->pNextSibling;
			}
		}
	}

	int PCSNode::GetNumSiblings() const
	{
		//walk siblings and return num siblings
		//this feels gross
		int NumSibs = 0;
		if (this->pParent != nullptr) {
			PCSNode* Current = this->pParent->pChild;

			while (Current != nullptr) {
				NumSibs++;
				Current = Current->pNextSibling;
			}
		}
		else {
			NumSibs++;
		}
		return NumSibs;
	}

	int PCSNode::GetNumChildren() const
	{
		//walk siblings and return num siblings... more efficient way? or is this more robust?
		int NumChildren = 0;
		if (this->pChild != nullptr) {
			PCSNode* Current = this->pChild;
			while (Current != nullptr) {
				NumChildren++;
				Current = Current->pNextSibling;
			}
		}

		return NumChildren;
	}

}

// ---  End of File ---------------
