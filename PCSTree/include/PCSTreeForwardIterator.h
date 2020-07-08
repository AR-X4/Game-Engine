//-----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef PCSTREE_FORWARD_ITERATOR_H
#define PCSTREE_FORWARD_ITERATOR_H

#include "PCSTreeIterator.h"

namespace Azul
{
	class PCSTreeForwardIterator : public PCSTreeIterator
	{
	public:
		PCSTreeForwardIterator(PCSNode* rootNode);
		PCSTreeForwardIterator() = delete;
		PCSTreeForwardIterator(const PCSTreeForwardIterator& in);
		PCSTreeForwardIterator& operator = (const PCSTreeForwardIterator& in);
		~PCSTreeForwardIterator();

		virtual PCSNode* First() override;
		virtual PCSNode* Next() override;
		virtual bool IsDone() override;
		virtual PCSNode* CurrentItem() override;

	private:
		
		PCSNode* pRoot;
		PCSNode* pCurrent;
	};

}

#endif

// ---  End of File ---------------