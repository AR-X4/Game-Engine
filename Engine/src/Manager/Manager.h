

#ifndef MANAGER_H
#define MANAGER_H

#include "DLink.h"

class Manager
{
public:
	DLink* BaseGetActive();

	//----------------------------------------------------------------------
	// Protected
	//----------------------------------------------------------------------
protected:

	//----------------------------------------------------------------------
	// Big Four
	//----------------------------------------------------------------------
	Manager();
	Manager(const Manager&) = delete;
	Manager& operator = (const Manager&) = delete;
	virtual ~Manager();

	void BaseInitialize(int InitialNumReserved = 3, int DeltaGrow = 1);

	//----------------------------------------------------------------------
	// Base methods - called in Derived class but lives in Base
	//----------------------------------------------------------------------
	void BaseSetReserve(int reserveNum, int reserveGrow);
	DLink* BaseGetActiveHead();
	DLink* BaseAdd();
	DLink* BaseFind(DLink* pNodeTarget);
	void BaseRemove(DLink* pNode);
	void BaseDestroy();
	void BaseDump();
	void BaseDumpNodes();
	void BaseDumpStats();

	//----------------------------------------------------------------------
	// Abstract methods - the "contract" Derived class must implement
	//----------------------------------------------------------------------
	virtual DLink* DerivedCreateNode() = 0;
	virtual bool DerivedCompare(DLink* pLinkA, DLink* pLinkB) = 0;
	virtual void DerivedWash(DLink* pLink) = 0;
	virtual void DerivedDumpNode(DLink* pLink) = 0;
	virtual void DerivedDestroyNode(DLink* pLink) = 0;

	//----------------------------------------------------------------------
	// Private methods - helpers
	//----------------------------------------------------------------------
private:

	void PrivFillReservedPool(int count);

	//----------------------------------------------------------------------
	// Data - must be agnostic to Derived data
	//----------------------------------------------------------------------
private:
	DLink* poActive;
	DLink* poReserve;

	int mDeltaGrow;
	int mTotalNumNodes;
	int mNumReserved;
	int mInitialNumReserved;
	int mNumActive;
};

#endif

// End of File 
