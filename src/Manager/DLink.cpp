//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "DLink.h"

//----------------------------------------------------------------------
// Class methods
//----------------------------------------------------------------------
DLink::DLink()
{
	this->Clear();
}

DLink::~DLink()
{
	// Safety
	this->Clear();

}


void DLink::Clear()
{
	this->pNext = nullptr;
	this->pPrev = nullptr;
}

//----------------------------------------------------------------------
// Static methods
//----------------------------------------------------------------------
void DLink::AddToFront(DLink*& pHead, DLink* pNode)
{
	// Will work for Active or Reserve List

	// add to front
	assert(pNode != nullptr);

	// add node
	if (pHead == nullptr)
	{
		// push to the front
		pHead = pNode;
		pNode->pNext = nullptr;
		pNode->pPrev = nullptr;
	}
	else
	{
		// push to front
		pNode->pPrev = nullptr;
		pNode->pNext = pHead;

		pHead->pPrev = pNode;
		pHead = pNode;
	}

	// worst case, pHead was null initially, now we added a node so... this is true
	assert(pHead != nullptr);
}

//void DLink::AddToLast(DLink*& pHead, DLink*& pLast, DLink* pNode)
//{
//	// Will work for Active or Reserve List
//
//	// add to front
//	assert(pNode != nullptr);
//
//	// add node
//	if (pLast == pHead && pHead == nullptr)
//	{
//		// push to the front
//		pHead = pNode;
//		pLast = pNode;
//		pNode->pNext = nullptr;
//		pNode->pPrev = nullptr;
//	}
//	else
//	{
//		assert(pLast != nullptr);
//		// add to end
//		pLast->pNext = pNode;
//		pNode->pPrev = pLast;
//		pNode->pNext = nullptr;
//
//		pLast = pNode;
//		// ---> no change for pHead
//	}
//
//	// worst case, pHead was null initially, now we added a node so... this is true
//	assert(pHead != nullptr);
//	assert(pLast != nullptr);
//}

DLink* DLink::PopFromFront(DLink*& pHead)
{
	// There should always be something on list
	assert(pHead != nullptr);

	// return node
	DLink* pNode = pHead;

	// Update head (OK if it points to NULL)
	pHead = pHead->pNext;
	if (pHead != nullptr)
	{
		pHead->pPrev = nullptr;
	}

	// HUGELY important - otherwise its crossed linked 
	//      Very hard to figure out
	pNode->Clear();

	return pNode;
}


void DLink::RemoveNode(DLink*& pHead, DLink* pNode)
{
	// protection
	assert(pNode != nullptr);

	// 4 different conditions... 
	if (pNode->pPrev != nullptr)
	{	// middle or last node
		pNode->pPrev->pNext = pNode->pNext;
	}
	else
	{  // first
		pHead = pNode->pNext;
	}

	if (pNode->pNext != nullptr)
	{	// middle node
		pNode->pNext->pPrev = pNode->pPrev;
	}
}

//void DLink::RemoveNode(DLink*& pHead, DLink*& pLast, DLink* pNode)
//{
//	// protection
//	assert(pNode != nullptr);
//
//	// Quick HACK... might be a bug... need to diagram
//
//	// 4 different conditions... 
//	if (pNode->pPrev != nullptr)
//	{	// middle or last node
//		pNode->pPrev->pNext = pNode->pNext;
//
//		if (pNode == pLast)
//		{
//			pLast = pNode->pPrev;
//		}
//	}
//	else
//	{  // first
//		pHead = pNode->pNext;
//
//		if (pNode == pLast)
//		{
//			// Only one node
//			pLast = pNode->pNext;
//		}
//		else
//		{
//			// Only first not the last
//			// do nothing more
//		}
//	}
//
//	if (pNode->pNext != nullptr)
//	{	// middle node
//		pNode->pNext->pPrev = pNode->pPrev;
//	}
//
//}

// End of File 
