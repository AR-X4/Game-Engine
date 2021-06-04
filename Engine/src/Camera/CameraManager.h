#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "Camera.h"
#include "Manager/Manager.h"

namespace Azul
{
	class CameraManager : public Manager
	{
	
	private:  // methods

		CameraManager(int reserveNum = 3, int reserveGrow = 1);
		CameraManager() = delete;
		CameraManager(const CameraManager&) = delete;
		CameraManager& operator = (const CameraManager&) = delete;
		~CameraManager();

		static CameraManager* GetInstance();

		
	public:
		static Camera* Add(Camera::Name NameIn, Camera::Type TypeIn);
		static void Create(int reserveNum = 3, int reserveGrow = 1);
		static void Destroy();
		static void Update();
		static Camera* Find(Camera::Name NameIn, Camera::Type TypeIn);

		static void SetActiveCamera(Camera::Type TypeIn, Camera* pCamera);
		static Camera* GetActiveCamera(Camera::Type TypeIn);

	protected:
		virtual DLink* DerivedCreateNode() override;
		virtual bool DerivedCompare(DLink* pLinkA, DLink* pLinkB) override;
		virtual void DerivedWash(DLink* pLink) override;
		virtual void DerivedDumpNode(DLink* pLink) override;
		virtual void DerivedDestroyNode(DLink* pLink) override;

	private:

		//DATA
		static CameraManager* poInstance;
		Camera* poNodeCompare;
		Camera* pActive3DCamera;
		Camera* pActive2DCamera;

	};

}

#endif

// ---  End of File ---
