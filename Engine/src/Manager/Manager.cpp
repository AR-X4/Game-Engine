

#include "Manager.h"

Manager::Manager()
{
	this->mDeltaGrow = 0;
	this->mNumReserved = 0;
	this->mInitialNumReserved = 0;

	this->mNumActive = 0;
	this->mTotalNumNodes = 0;
	this->poActive = nullptr;
	this->poReserve = nullptr;
}

Manager::~Manager()
{
	// This function does our work!
	//      Derived class should have called this... but just in case
	this->BaseDestroy();

	assert(this->poActive == nullptr);
	assert(this->poReserve == nullptr);

	//Trace::out("---> ~Manager()\n");
}


void Manager::BaseInitialize(int InitialNumReserved, int DeltaGrow)
{
	// Check now or pay later
	assert(InitialNumReserved >= 0);
	assert(DeltaGrow > 0);

	// Squirrel away these
	this->mDeltaGrow = DeltaGrow;
	this->mInitialNumReserved = InitialNumReserved;

	// Preload the reserve
	this->PrivFillReservedPool(InitialNumReserved);
}

//----------------------------------------------------------------------
// Base methods - called in Derived class but lives in Base
//----------------------------------------------------------------------
void Manager::BaseSetReserve(int reserveNum, int reserveGrow)
{
	this->mDeltaGrow = reserveGrow;

	if (reserveNum > this->mNumReserved)
	{
		// Preload the reserve
		this->PrivFillReservedPool(reserveNum - this->mNumReserved);
	}
}

DLink* Manager::BaseGetActiveHead()
{
	//assert(this->poActive != nullptr);
	return this->poActive;
}

DLink* Manager::BaseAdd()
{
	// Are there any nodes on the Reserve list?
	if (this->poReserve == nullptr)
	{
		// refill the reserve list by the DeltaGrow
		this->PrivFillReservedPool(this->mDeltaGrow);
	}

	// Always take from the reserve list
	DLink* pLink = DLink::PopFromFront(this->poReserve);

	assert(pLink != nullptr);

	// Wash it
	this->DerivedWash(pLink);

	// Update stats
	this->mNumActive++;
	this->mNumReserved--;

	// copy to active
	DLink::AddToFront(this->poActive, pLink);

	// YES - here's your new one (its recycled from reserved)
	return pLink;
}


DLink* Manager::BaseFind(DLink* pNodeTarget)
{
	assert(pNodeTarget != nullptr);

	// search the active list
	DLink* pLink = this->poActive;

	// Walk through the nodes
	while (pLink != nullptr)
	{
		if (this->DerivedCompare(pLink, pNodeTarget))
		{
			// found it
			break;
		}
		pLink = pLink->pNext;
	}

	return pLink;
}


void Manager::BaseRemove(DLink* pNode)
{
	assert(pNode != nullptr);

	// Don't do the work here... delegate it
	DLink::RemoveNode(this->poActive, pNode);

	// wash it before returning to reserve list
	this->DerivedWash(pNode);

	// add it to the return list
	DLink::AddToFront(this->poReserve, pNode);

	// stats update
	this->mNumActive--;
	this->mNumReserved++;
}


DLink* Manager::BaseGetActive()
{
	return this->poActive;
}

void Manager::BaseDestroy()
{
	// search for the name
	DLink* pNode;
	DLink* pTmpNode;

	// Active List
	pNode = this->poActive;

	while (pNode != nullptr)
	{
		// walking through the list
		pTmpNode = pNode;
		pNode = pNode->pNext;

		// node to cleanup
		assert(pTmpNode != nullptr);

		DLink::RemoveNode(this->poActive, pTmpNode);

		this->DerivedDestroyNode(pTmpNode);
		pTmpNode = nullptr;

		this->mNumActive--;
		this->mTotalNumNodes--;
	}

	// Reserve List
	pNode = this->poReserve;

	while (pNode != nullptr)
	{
		// walking through the list
		pTmpNode = pNode;
		pNode = pNode->pNext;

		// node to cleanup
		assert(pTmpNode != nullptr);

		DLink::RemoveNode(this->poReserve, pTmpNode);

		this->DerivedDestroyNode(pTmpNode);
		pTmpNode = nullptr;

		this->mNumReserved--;
		this->mTotalNumNodes--;
	}
}

void Manager::BaseDump()
{
	this->BaseDumpStats();
	this->BaseDumpNodes();
}

void Manager::BaseDumpNodes()
{
	Trace::out("\n");
	Trace::out("------ Active List: ---------------------------\n");

	DLink* pNode = this->poActive;
	int i;

	if (pNode == nullptr)
	{
		Trace::out("  <list empty>\n");
	}
	else
	{
		i = 0;
		while (pNode != nullptr)
		{
			Trace::out("%d: -------------\n", i);
			this->DerivedDumpNode(pNode);
			i++;
			pNode = pNode->pNext;
		}
	}
	Trace::out("\n");
	Trace::out("------ Reserve List: ---------------------------\n");

	pNode = this->poReserve;

	if (pNode == nullptr)
	{
		Trace::out("  <list empty>\n");
	}
	else
	{
		i = 0;
		while (pNode != nullptr)
		{
			Trace::out("%d: -------------\n", i);
			this->DerivedDumpNode(pNode);
			i++;
			pNode = pNode->pNext;
		}
	}
}


void Manager::BaseDumpStats()
{
	Trace::out("\n");
	Trace::out("\n");
	Trace::out("-------- Stats: -------------\n");
	Trace::out("  Total Num Nodes: %d\n", this->mTotalNumNodes);
	Trace::out("       Num Active: %d\n", this->mNumActive);
	Trace::out("     Num Reserved: %d\n", this->mNumReserved);
	Trace::out("       Delta Grow: %d\n", this->mDeltaGrow);
}

//----------------------------------------------------------------------
// Private methods - helpers
//----------------------------------------------------------------------
void Manager::PrivFillReservedPool(int count)
{
	// doesn't make sense if its not at least 1
	assert(count > 0);

	this->mTotalNumNodes += count;
	this->mNumReserved += count;

	// Preload the reserve
	for (int i = 0; i < count; i++)
	{
		DLink* pNode = this->DerivedCreateNode();
		assert(pNode != nullptr);

		DLink::AddToFront(this->poReserve, pNode);
	}
}

// End of File 
