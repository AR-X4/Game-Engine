
#ifndef CAMERA_H
#define CAMERA_H

#include "MathEngine.h"
#include "Manager/DLink.h"
#include "BoundingVolume/BoundingSphere.h"


namespace Azul
{
	

	class Camera final : public Align16, public DLink
	{

	public:

		enum class Type
		{
			PERSPECTIVE_3D,
			ORTHOGRAPHIC_2D,
			UNINITIALIZED
		};

		enum class Name {

			Cam1,
			Cam2,
			Cam3,
			UNINITIALIZED

		};

		enum class CullResult
		{
			CULL_INSIDE,
			CULL_OUTSIDE
		};


		//Camera(Camera::Type type);
		Camera();
		Camera(const Camera&) = delete;
		Camera& operator = (const Camera&) = delete;
		~Camera();

		// Setup on single camera
		void setOrthographic(const float xMin, const float xMax, const float yMin, const float yMax, const float zMin, const float zMax);
		void setPerspective(const float FieldOfView_Degs, const float AspectRatio, const float NearDist, const float FarDist);
		void setViewport(const int inX, const int inY, const int width, const int height);
		void setOrientAndPosition(const Vect& Up_vect, const Vect& inLookAt_pt, const Vect& pos_pt);

		// update camera system
		void updateCamera();

		// Get the matrices for rendering
		Matrix& getViewMatrix();
		Matrix& getProjMatrix();

		// accessors
		void getPos(Vect& outPos) const;
		void getDir(Vect& outDir) const;
		void getUp(Vect& outUp) const;
		void getLookAt(Vect& outLookAt) const;
		void getRight(Vect& outRight) const;

		int getScreenWidth() const;
		int getScreenHeight() const;

		// Why no SETS?  Pos,Dir,Up,LookAt, Right
		//   They have to be adjust together in setOrientAndPosition()

		void getFieldOfView(float& Value) const;
		void setFieldOfView(const float Value);

		void getNearDist(float& Value) const;
		void setNearDist(const float Value);

		// helper functions
		Camera::Type getType() const;
		void GetHelper(Vect& up, Vect& tar, Vect& pos, Vect& upNorm, Vect& forwardNorm, Vect& rightNorm);
		void SetHelper(Vect& up, Vect& tar, Vect& pos);

		void getNearTopLeft(Vect& vOut) const;
		void getNearTopRight(Vect& vOut) const;
		void getNearBottomLeft(Vect& vOut) const;
		void getNearBottomRight(Vect& vOut) const;
		void getFarTopLeft(Vect& vOut) const;
		void getFarTopRight(Vect& vOut) const;
		void getFarBottomLeft(Vect& vOut) const;
		void getFarBottomRight(Vect& vOut) const;

		CullResult CullTest(const Sphere& Sphere);

		//Dlink functions
		void Set(Name NameIn, Type TypeIn);
		void Dump();
		void Wash();


	private:  // methods should never be public
		void privSetViewState(void);
		void privCalcPlaneHeightWidth(void);
		void privCalcFrustumVerts(void);
		void privCalcFrustumCollisionNormals(void);
		void privUpdateProjectionMatrix(void);
		void privUpdateViewMatrix(void);


	private:  // data  (Keep it private)

		// -------------------------------------------------------
		// 16-bit aligned data below this point:
		// -------------------------------------------------------

		// Projection Matrix
		Matrix	projMatrix;
		Matrix	viewMatrix;

		// camera unit vectors (up, dir, right)
		Vect	vUp;
		Vect	vDir;
		Vect	vRight;  // derived by up and dir
		Vect	vPos;

		Vect	vLookAt;

		// world space coords for viewing frustum
		Vect	nearTopLeft;
		Vect	nearTopRight;
		Vect	nearBottomLeft;

		Vect	nearBottomRight;
		Vect	farTopLeft;
		Vect	farTopRight;
		Vect	farBottomLeft;

		Vect	farBottomRight;

		// Normals of the frustum
		Vect	frontNorm;
		Vect	backNorm;
		Vect	rightNorm;

		Vect	leftNorm;
		Vect	topNorm;
		Vect	bottomNorm;

		// -------------------------------------------------------
		// unaligned data below this point:
		// -------------------------------------------------------

		Camera::Type camType;

		// Define the frustum inputs
		float	nearDist;
		float	farDist;
		float	fovy;
		float	aspectRatio;

		// Screen size in world space
		float	near_height;
		float	near_width;
		float	far_height;
		float	far_width;

		// Ortho hacks 
		float	xMin;
		float	yMin;
		float	zMin;
		float	xMax;
		float	yMax;
		float	zMax;

		// viewports
		int		viewport_x;
		int		viewport_y;
		int		viewport_width;
		int		viewport_height;

	public:
		

		Name CamName;
		//char pad[12];

	};

}

#endif

// --- End of File ---