#version 430 core

// ------------------------------------------
//    Compute World
// ------------------------------------------

// localize breakdown
layout(local_size_x = 20) in;

struct Bone
{
	vec4 t;
	vec4 q;
	vec4 s;
};

layout (std430, binding = 0) buffer LocalBone
{
	Bone LocalBone_array[];
};

layout (std430, binding = 1) buffer HierarchyTable
{
	int HierarchyTable_array[];
};

layout (std430, binding = 2) buffer WorldBone
{
	mat4 WorldBone_array[];
};

// Uniforms
// ...

uniform int tableDepth;
uniform int boneCount;
uniform mat4 rigidWorld;

mat4 Bone2Matrix( Bone bone );
mat4 CreateTransMatrix( vec4 vTrans );
mat4 CreateScaleMatrix( vec4 vScale );
mat4 CreateRotMatrix( vec4 vQuat );

void main()
{

	uint boneIndex = gl_GlobalInvocationID.x;

	if( gl_GlobalInvocationID.x < boneCount )
	{
		mat4 tmp = rigidWorld;
		
		for( int i = 0; i < tableDepth; i++ )
		{
			// skip to the row then do the whole row
			int parentIndex = HierarchyTable_array[(boneIndex * tableDepth)+i];

			if( parentIndex < boneCount )
			{
				Bone b = LocalBone_array[parentIndex];
				tmp = tmp * Bone2Matrix( b ) ;
			}
			else
			{
				// skip (treat as an Identity matrix)
			}
		}

		//tmp = rigidWorld * tmp;
		WorldBone_array[boneIndex] = tmp;
	}
};

mat4 Bone2Matrix( Bone bone )
{
	mat4 tmp;

	mat4 MatTrans = CreateTransMatrix( bone.t );
	mat4 MatRot = CreateRotMatrix( bone.q );
	mat4 MatScale = CreateScaleMatrix( bone.s );

	tmp = MatTrans * MatRot * MatScale;

	return tmp;
};

mat4 CreateTransMatrix( vec4 vTrans )
{
	mat4 tmp = mat4(1.0);

	tmp[3] = vec4(vTrans.x,vTrans.y,vTrans.z,1.0);
	
	return tmp;
};

mat4 CreateScaleMatrix( vec4 vScale )
{
	mat4 tmp = mat4(1.0);

	tmp[0].x = vScale.x;
	tmp[1].y = vScale.y;
	tmp[2].z = vScale.z;

	return tmp;
};

mat4 CreateRotMatrix( vec4 q )
{
	// this function has been transposed
	float x2, y2, z2;
	float xx, xy, xz;
	float yy, yz, zz;
	float wx, wy, wz;

	x2 = q.x + q.x;
	y2 = q.y + q.y;
	z2 = q.z + q.z;

	xx = q.x * x2;
	xy = q.x * y2;
	xz = q.x * z2;

	yy = q.y * y2;
	yz = q.y * z2;
	zz = q.z * z2;

	wx = q.w * x2;
	wy = q.w * y2;
	wz = q.w * z2;

	mat4 result;

	result[0] = vec4(1.0 - (yy + zz), xy + wz,	xz - wy, 0.0);
	result[1] = vec4(xy - wz,1.0f - (xx + zz), yz + wx, 0.0);
	result[2] = vec4(xz + wy,yz - wx,1.0 - (xx + yy), 0.0);
	result[3] = vec4(0.0, 0.0, 0.0, 1.0);

	return result;
};


