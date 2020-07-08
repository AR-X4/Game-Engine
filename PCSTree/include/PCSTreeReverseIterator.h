//-----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef PCSTREE_REVERSE_ITERATOR_H
#define PCSTREE_REVERSE_ITERATOR_H

#include "PCSTreeIterator.h"

namespace Azul
{
	class PCSTreeReverseIterator : public PCSTreeIterator
	{
	public:

		PCSTreeReverseIterator(PCSNode* rootNode);
		PCSTreeReverseIterator() = delete;
		PCSTreeReverseIterator(const PCSTreeReverseIterator& in);
		PCSTreeReverseIterator& operator = (const PCSTreeReverseIterator& in);
		~PCSTreeReverseIterator();

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