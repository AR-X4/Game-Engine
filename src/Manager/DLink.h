//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef DLINK_H
#define DLINK_H

class DLink
{

protected:

	// Big Four
	DLink();
	DLink(const DLink&) = delete;
	DLink& operator = (const DLink&) = delete;

	// Always have base class have virtual destructor
	virtual ~DLink();

public:

	void Clear();

	//----------------------------------------------------------------------
	// Static methods
	//----------------------------------------------------------------------

	// Methods with Head
	static void AddToFront(DLink*& pHead, DLink* pNode);
	static DLink* PopFromFront(DLink*& pHead);
	static void RemoveNode(DLink*& pHead, DLink* pNode);

	// Methods with Head and Tail
	static void AddToLast(DLink*& pHead, DLink*& pLast, DLink* pNode);
	static void RemoveNode(DLink*& pHead, DLink*& pLast, DLink* pNode);

	//----------------------------------------------------------------------
	// Data
	//----------------------------------------------------------------------
	DLink* pNext;
	DLink* pPrev;
};

#endif

// End of File 
