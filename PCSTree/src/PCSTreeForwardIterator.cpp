//-----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "PCSTreeForwardIterator.h"

namespace Azul
{
	PCSTreeForwardIterator::PCSTreeForwardIterator(PCSNode* rootNode)
	{
		assert(rootNode != nullptr);

		this->pRoot = rootNode;
		this->pCurrent = this->pRoot;
	}

	PCSTreeForwardIterator::PCSTreeForwardIterator(const PCSTreeForwardIterator& in)
	{
		this->pRoot = in.pRoot;
		this->pCurrent = in.pCurrent;
	}

	PCSTreeForwardIterator& PCSTreeForwardIterator::operator=(const PCSTreeForwardIterator& in)
	{
		this->pRoot = in.pRoot;
		this->pCurrent = in.pCurrent;

		return *this;
	}

	PCSTreeForwardIterator::~PCSTreeForwardIterator()
	{
	}

	PCSNode* PCSTreeForwardIterator::First()
	{
		assert(this->pRoot != nullptr);

		return this->pRoot;
	}

	PCSNode* PCSTreeForwardIterator::Next()
	{
		this->pCurrent = this->pCurrent->GetForward();

		return  this->pCurrent;
	}

	bool PCSTreeForwardIterator::IsDone()
	{
		return  (this->pCurrent == nullptr);
	}

	PCSNode* PCSTreeForwardIterator::CurrentItem()
	{
		return  this->pCurrent;
	}

}

// ---  End of File ---------------
