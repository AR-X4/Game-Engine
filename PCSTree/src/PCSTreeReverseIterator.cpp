//-----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "PCSTreeReverseIterator.h"

namespace Azul
{
	PCSTreeReverseIterator::PCSTreeReverseIterator(PCSNode* rootNode)
	{
		assert(rootNode != nullptr);

		this->pRoot = rootNode;

		this->pCurrent = this->pRoot;
		while (this->pCurrent != nullptr) {
			this->pCurrent = this->pCurrent->GetForward();
		}

	}

	PCSTreeReverseIterator::PCSTreeReverseIterator(const PCSTreeReverseIterator& in)
	{
		this->pRoot = in.pRoot;
		this->pCurrent = in.pCurrent;
	}

	PCSTreeReverseIterator& PCSTreeReverseIterator::operator=(const PCSTreeReverseIterator& in)
	{
		this->pRoot = in.pRoot;
		this->pCurrent = in.pCurrent;

		return *this;
	}

	PCSTreeReverseIterator::~PCSTreeReverseIterator()
	{
	}

	PCSNode* PCSTreeReverseIterator::First()
	{
		return  this->pRoot->GetReverse();
	}

	PCSNode* PCSTreeReverseIterator::Next()
	{
		this->pCurrent = this->pCurrent->GetReverse();

		return  this->pCurrent;
	}

	bool PCSTreeReverseIterator::IsDone()
	{
		return (this->pCurrent == nullptr);
	}

	PCSNode* PCSTreeReverseIterator::CurrentItem()
	{
		return  this->pCurrent;
	}
}

// ---  End of File ---------------
