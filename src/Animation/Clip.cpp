


#include "Clip.h"
#include "Time/Time.h"
#include "Bone.h"
#include "Mixer.h"

namespace Azul
{
	Clip::Clip( int _numBones, FrameBucket* poHeadIn, Name _ClipName)
	: numBones(_numBones),
	numFrames(0),
	TotalTime(Time::Duration::ZERO),
	poHead(poHeadIn),
	pNext(nullptr),
	pPrev(nullptr),
	ClipName(_ClipName)
	{
		//this->privSetAnimationData();
		this->TotalTime = this->privFindMaxTime();
		this->numFrames = this->privFindNumFrames();



		/*FrameBucket* bucket = poHeadIn;

		for (int i = 0; i < 31; i++) {
			Trace::out2("\n-----------FRAME: %d-----------\n\n", i);

			for (int j = 0; j < _numBones; j++)
			{
				Trace::out2("INDEX: %d\n", j);

				Trace::out2("Vect(%f, %f, %f)\nQuat(%f, %f, %f)\nVect(%f, %f, %f)\n\n",
					bucket->poBone[j].T.x(), bucket->poBone[j].T.y(), bucket->poBone[j].T.z(),
					bucket->poBone[j].Q.qx(), bucket->poBone[j].Q.qy(), bucket->poBone[j].Q.qz(),
					bucket->poBone[j].S.x(), bucket->poBone[j].S.y(), bucket->poBone[j].S.z());

			}
			bucket = bucket->nextBucket;

		}*/
	}

	Clip::~Clip()
	{
		FrameBucket *pTmp = poHead;

		while (pTmp != nullptr)
		{
			FrameBucket *pDeleteMe = pTmp;
			pTmp = pTmp->nextBucket;
			delete pDeleteMe;
		}
	}

	void Clip::AnimateBones(Time tCurr, Bone *pResult)
	{
		// First one 
		FrameBucket *pTmp = this->poHead;

		// Find which key frames
		while (tCurr >= pTmp->KeyTime && pTmp->nextBucket != nullptr)
		{
			pTmp = pTmp->nextBucket;
		}

		// pTmp is the "B" key frame
		// pTmp->prev is the "A" key frame
		FrameBucket *pA = pTmp->prevBucket;
		FrameBucket *pB = pTmp;

		// find the "S" of the time
		float tS = (tCurr - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);

		Mixer::Blend(pResult, pA->poBone, pB->poBone, tS, this->numBones);

	}

	
	int Clip::privFindNumFrames()
	{
		int count = 0;
		FrameBucket *pTmp = this->poHead;

		while (pTmp->nextBucket != nullptr)
		{
			count++;
			pTmp = pTmp->nextBucket;
		}
		return count;
	}

	Time Clip::privFindMaxTime()
	{
		Time tMax;
		FrameBucket *pTmp = this->poHead;

		while (pTmp->nextBucket != nullptr)
		{
			pTmp = pTmp->nextBucket;
		}

		tMax = pTmp->KeyTime;

		return tMax;
	}

	Time Clip::GetTotalTime()
	{
		return this->TotalTime;
	}

	//void Clip::privSetAnimationData()
	//{

	//	// --------  Frame 0  ----------------------------

	//	FrameBucket *pTmp = new FrameBucket();
	//	pTmp->prevBucket = nullptr;
	//	pTmp->nextBucket = nullptr;
	//	pTmp->KeyTime = 0 * Time(Time::Duration::NTSC_30_FRAME);
	//	pTmp->poBone = new Bone[(unsigned int)this->numBones];
	//	poHead = pTmp;

	//	//------- Key Time: 0  ms: 0 ------------------ 

	//	////     Node Name: RootNode
	//	//pTmp->poBone[-3].T = Vect(0.000000f, 0.000000f, 0.000000f);
	//	//pTmp->poBone[-3].Q = Quat(Quat::Rot3::XYZ, 0.000000f, -0.000000f, 0.000000f);
	//	//pTmp->poBone[-3].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	////     Node Name: Humanoid:MaxToFilmbox_Root
	//	//pTmp->poBone[-2].T = Vect(0.000000f, 0.000000f, 0.000000f);
	//	//pTmp->poBone[-2].Q = Quat(Quat::Rot3::XYZ, -1.570796f, -0.000000f, 0.000000f);
	//	//pTmp->poBone[-2].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	////     Node Name: Humanoid:Reference
	//	//pTmp->poBone[-1].T = Vect(0.000000f, 0.000000f, 0.000000f);
	//	//pTmp->poBone[-1].Q = Quat(Quat::Rot3::XYZ, 0.000000f, -0.000000f, 0.000000f);
	//	//pTmp->poBone[-1].S = Vect(0.165000f, 0.165000f, 0.165000f);

	//	//     Node Name: Humanoid:Hips
	//	pTmp->poBone[0].T = Vect(-280.172638f, 1288.917847f, 526.212402f);
	//	pTmp->poBone[0].Q = Quat(Quat::Rot3::XYZ, 0.401148f, 0.069213f, -0.030462f);
	//	pTmp->poBone[0].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Spine_Dummy
	//	pTmp->poBone[1].T = Vect(0.000014f, -13.665373f, 18.144430f);
	//	pTmp->poBone[1].Q = Quat(Quat::Rot3::XYZ, -3.141593f, -1.570796f, 0.000000f);
	//	pTmp->poBone[1].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Spine
	//	pTmp->poBone[2].T = Vect(73.400002f, 0.000001f, 0.000000f);
	//	pTmp->poBone[2].Q = Quat(Quat::Rot3::XYZ, -0.031461f, 0.001422f, 0.030284f);
	//	pTmp->poBone[2].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Spine1
	//	pTmp->poBone[3].T = Vect(70.001335f, -0.000002f, -0.000000f);
	//	pTmp->poBone[3].Q = Quat(Quat::Rot3::XYZ, -0.060012f, 0.005307f, 0.060495f);
	//	pTmp->poBone[3].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Spine2
	//	pTmp->poBone[4].T = Vect(71.400032f, -0.000000f, 0.000000f);
	//	pTmp->poBone[4].Q = Quat(Quat::Rot3::XYZ, -0.060005f, 0.005307f, 0.060495f);
	//	pTmp->poBone[4].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Neck
	//	pTmp->poBone[5].T = Vect(121.576225f, -0.000001f, -0.000000f);
	//	pTmp->poBone[5].Q = Quat(Quat::Rot3::XYZ, -0.083518f, 0.064311f, 0.228354f);
	//	pTmp->poBone[5].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Head
	//	pTmp->poBone[6].T = Vect(67.855774f, 0.000039f, -0.000000f);
	//	pTmp->poBone[6].Q = Quat(Quat::Rot3::XYZ, -0.119745f, -0.014206f, -0.072717f);
	//	pTmp->poBone[6].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Head_End
	//	pTmp->poBone[7].T = Vect(92.789192f, 0.000013f, -0.000000f);
	//	pTmp->poBone[7].Q = Quat(Quat::Rot3::XYZ, -0.000000f, 0.000003f, 0.000000f);
	//	pTmp->poBone[7].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftShoulder_Dummy
	//	pTmp->poBone[8].T = Vect(65.589684f, 2.530523f, 21.279638f);
	//	pTmp->poBone[8].Q = Quat(Quat::Rot3::XYZ, 1.436787f, 0.214620f, 3.102453f);
	//	pTmp->poBone[8].S = Vect(0.999999f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftShoulder
	//	pTmp->poBone[9].T = Vect(-36.845097f, -0.000012f, -0.000005f);
	//	pTmp->poBone[9].Q = Quat(Quat::Rot3::XYZ, -0.086833f, -0.017928f, -0.028073f);
	//	pTmp->poBone[9].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftArm_Dummy
	//	pTmp->poBone[10].T = Vect(-1.681311f, 62.969807f, -0.310574f);
	//	pTmp->poBone[10].Q = Quat(Quat::Rot3::XYZ, 1.507469f, 0.072192f, -2.069909f);
	//	pTmp->poBone[10].S = Vect(1.000002f, 0.999999f, 1.000002f);

	//	//     Node Name: Humanoid:LeftArm
	//	pTmp->poBone[11].T = Vect(-20.843035f, -1.709348f, 11.313786f);
	//	pTmp->poBone[11].Q = Quat(Quat::Rot3::XYZ, -0.141143f, 0.317536f, 2.825809f);
	//	pTmp->poBone[11].S = Vect(0.999999f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftArmRoll
	//	pTmp->poBone[12].T = Vect(78.950714f, 0.000019f, 0.000006f);
	//	pTmp->poBone[12].Q = Quat(Quat::Rot3::XYZ, 0.113023f, 0.000000f, 0.000000f);
	//	pTmp->poBone[12].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftForeArm
	//	pTmp->poBone[13].T = Vect(65.979294f, 0.000019f, -0.000010f);
	//	pTmp->poBone[13].Q = Quat(Quat::Rot3::XYZ, 0.065537f, 0.030556f, -2.037498f);
	//	pTmp->poBone[13].S = Vect(1.000002f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:LeftForeArmRoll
	//	pTmp->poBone[14].T = Vect(101.404221f, 0.000009f, 0.000010f);
	//	pTmp->poBone[14].Q = Quat(Quat::Rot3::XYZ, 0.100070f, 0.000000f, 0.000000f);
	//	pTmp->poBone[14].S = Vect(0.999999f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftHand
	//	pTmp->poBone[15].T = Vect(71.692039f, 0.000010f, 0.000004f);
	//	pTmp->poBone[15].Q = Quat(Quat::Rot3::XYZ, -0.030181f, 0.374359f, -0.159030f);
	//	pTmp->poBone[15].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftThumb1
	//	pTmp->poBone[16].T = Vect(29.257690f, -22.640423f, -5.069802f);
	//	pTmp->poBone[16].Q = Quat(Quat::Rot3::XYZ, 0.709381f, 0.537656f, -0.914867f);
	//	pTmp->poBone[16].S = Vect(0.999999f, 1.000001f, 1.000003f);

	//	//     Node Name: Humanoid:LeftThumb2
	//	pTmp->poBone[17].T = Vect(22.746157f, -0.353276f, -0.113011f);
	//	pTmp->poBone[17].Q = Quat(Quat::Rot3::XYZ, 0.081784f, 0.342417f, 0.683623f);
	//	pTmp->poBone[17].S = Vect(0.999997f, 1.000002f, 1.000001f);

	//	//     Node Name: Humanoid:LeftThumb3
	//	pTmp->poBone[18].T = Vect(17.496328f, 0.116578f, 0.089712f);
	//	pTmp->poBone[18].Q = Quat(Quat::Rot3::XYZ, 0.027024f, 0.198537f, 0.096617f);
	//	pTmp->poBone[18].S = Vect(0.999999f, 1.000001f, 1.000000f);

	//	//     Node Name: Humanoid:LeftThumb_End
	//	pTmp->poBone[19].T = Vect(18.145250f, 0.286301f, 0.550287f);
	//	pTmp->poBone[19].Q = Quat(Quat::Rot3::XYZ, -0.000000f, 0.253071f, -0.000000f);
	//	pTmp->poBone[19].S = Vect(0.999999f, 1.000000f, 1.000001f);

	//	//     Node Name: Humanoid:LeftIndex1
	//	pTmp->poBone[20].T = Vect(58.921104f, -18.573286f, 1.447069f);
	//	pTmp->poBone[20].Q = Quat(Quat::Rot3::XYZ, 1.586099f, 0.426615f, 0.025352f);
	//	pTmp->poBone[20].S = Vect(1.000001f, 0.999997f, 1.000005f);

	//	//     Node Name: Humanoid:LeftIndex2
	//	pTmp->poBone[21].T = Vect(27.947330f, -0.000028f, 0.000011f);
	//	pTmp->poBone[21].Q = Quat(Quat::Rot3::XYZ, -0.015023f, 0.000552f, -0.279687f);
	//	pTmp->poBone[21].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftIndex3
	//	pTmp->poBone[22].T = Vect(17.556934f, -0.000062f, -0.000005f);
	//	pTmp->poBone[22].Q = Quat(Quat::Rot3::XYZ, -0.015553f, -0.011463f, -0.281321f);
	//	pTmp->poBone[22].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftIndex_End
	//	pTmp->poBone[23].T = Vect(12.971698f, -0.000033f, -0.000009f);
	//	pTmp->poBone[23].Q = Quat(Quat::Rot3::XYZ, 0.000005f, -0.020750f, -0.003191f);
	//	pTmp->poBone[23].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftMiddle1
	//	pTmp->poBone[24].T = Vect(57.844334f, -4.515547f, 3.569310f);
	//	pTmp->poBone[24].Q = Quat(Quat::Rot3::XYZ, 1.525321f, 0.667963f, 0.018870f);
	//	pTmp->poBone[24].S = Vect(1.000001f, 0.999997f, 1.000005f);

	//	//     Node Name: Humanoid:LeftMiddle2
	//	pTmp->poBone[25].T = Vect(32.362274f, -0.000006f, -0.000017f);
	//	pTmp->poBone[25].Q = Quat(Quat::Rot3::XYZ, -0.024592f, 0.021554f, -0.488195f);
	//	pTmp->poBone[25].S = Vect(1.000001f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:LeftMiddle3
	//	pTmp->poBone[26].T = Vect(18.401133f, 0.000069f, 0.000003f);
	//	pTmp->poBone[26].Q = Quat(Quat::Rot3::XYZ, -0.024587f, 0.021555f, -0.488208f);
	//	pTmp->poBone[26].S = Vect(1.000001f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:LeftMiddle_End
	//	pTmp->poBone[27].T = Vect(13.346432f, 0.000025f, -0.000002f);
	//	pTmp->poBone[27].Q = Quat(Quat::Rot3::XYZ, 0.000003f, -0.000005f, -0.000006f);
	//	pTmp->poBone[27].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftRing1
	//	pTmp->poBone[28].T = Vect(57.439991f, 9.168867f, 2.940117f);
	//	pTmp->poBone[28].Q = Quat(Quat::Rot3::XYZ, 1.483405f, 0.852180f, 0.008604f);
	//	pTmp->poBone[28].S = Vect(1.000001f, 0.999997f, 1.000005f);

	//	//     Node Name: Humanoid:LeftRing2
	//	pTmp->poBone[29].T = Vect(30.068848f, 0.000063f, 0.000002f);
	//	pTmp->poBone[29].Q = Quat(Quat::Rot3::XYZ, -0.041307f, 0.045509f, -0.696692f);
	//	pTmp->poBone[29].S = Vect(1.000001f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftRing3
	//	pTmp->poBone[30].T = Vect(15.270657f, 0.000024f, 0.000004f);
	//	pTmp->poBone[30].Q = Quat(Quat::Rot3::XYZ, -0.041310f, 0.045508f, -0.696687f);
	//	pTmp->poBone[30].S = Vect(1.000001f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftRing_End
	//	pTmp->poBone[31].T = Vect(12.744839f, 0.000002f, -0.000001f);
	//	pTmp->poBone[31].Q = Quat(Quat::Rot3::XYZ, 0.000002f, -0.000000f, -0.000031f);
	//	pTmp->poBone[31].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftPinky1
	//	pTmp->poBone[32].T = Vect(56.621067f, 20.249598f, -1.450183f);
	//	pTmp->poBone[32].Q = Quat(Quat::Rot3::XYZ, 1.406783f, 0.858990f, 0.038222f);
	//	pTmp->poBone[32].S = Vect(1.000001f, 0.999997f, 1.000005f);

	//	//     Node Name: Humanoid:LeftPinky2
	//	pTmp->poBone[33].T = Vect(19.894529f, 0.000152f, -0.000017f);
	//	pTmp->poBone[33].Q = Quat(Quat::Rot3::XYZ, -0.070957f, 0.120430f, -0.831169f);
	//	pTmp->poBone[33].S = Vect(1.000001f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftPinky3
	//	pTmp->poBone[34].T = Vect(12.904247f, 0.000005f, 0.000009f);
	//	pTmp->poBone[34].Q = Quat(Quat::Rot3::XYZ, -0.070955f, 0.120425f, -0.831149f);
	//	pTmp->poBone[34].S = Vect(1.000002f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftPinky_End
	//	pTmp->poBone[35].T = Vect(10.892069f, -0.000096f, -0.000019f);
	//	pTmp->poBone[35].Q = Quat(Quat::Rot3::XYZ, 0.000003f, 0.000015f, 0.000012f);
	//	pTmp->poBone[35].S = Vect(0.999999f, 1.000000f, 1.000001f);

	//	//     Node Name: Humanoid:RightShoulder_Dummy
	//	pTmp->poBone[36].T = Vect(65.589691f, 2.532739f, -21.279242f);
	//	pTmp->poBone[36].Q = Quat(Quat::Rot3::XYZ, 1.436894f, 0.214624f, -0.039117f);
	//	pTmp->poBone[36].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightShoulder
	//	pTmp->poBone[37].T = Vect(36.845016f, 0.000015f, 0.000005f);
	//	pTmp->poBone[37].Q = Quat(Quat::Rot3::XYZ, -0.062317f, -0.007251f, -0.047899f);
	//	pTmp->poBone[37].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightArm_Dummy
	//	pTmp->poBone[38].T = Vect(1.681307f, -62.969696f, 0.310573f);
	//	pTmp->poBone[38].Q = Quat(Quat::Rot3::XYZ, 1.507469f, 0.072192f, -2.069909f);
	//	pTmp->poBone[38].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightArm
	//	pTmp->poBone[39].T = Vect(20.843040f, 1.709348f, -11.313787f);
	//	pTmp->poBone[39].Q = Quat(Quat::Rot3::XYZ, 0.317549f, 0.075090f, -2.764645f);
	//	pTmp->poBone[39].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightArmRoll
	//	pTmp->poBone[40].T = Vect(-78.950615f, 0.000005f, 0.000027f);
	//	pTmp->poBone[40].Q = Quat(Quat::Rot3::XYZ, 0.886762f, 0.000001f, 0.000000f);
	//	pTmp->poBone[40].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightForeArm
	//	pTmp->poBone[41].T = Vect(-65.979179f, -0.000020f, 0.000040f);
	//	pTmp->poBone[41].Q = Quat(Quat::Rot3::XYZ, 0.108827f, 0.019707f, -1.300434f);
	//	pTmp->poBone[41].S = Vect(1.000001f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:RightForeArmRoll
	//	pTmp->poBone[42].T = Vect(-101.404114f, -0.000007f, 0.000018f);
	//	pTmp->poBone[42].Q = Quat(Quat::Rot3::XYZ, 0.152709f, 0.000000f, -0.000000f);
	//	pTmp->poBone[42].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightHand
	//	pTmp->poBone[43].T = Vect(-71.691925f, -0.000011f, 0.000047f);
	//	pTmp->poBone[43].Q = Quat(Quat::Rot3::XYZ, -0.020400f, -0.458479f, 0.087373f);
	//	pTmp->poBone[43].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightThumb1
	//	pTmp->poBone[44].T = Vect(-29.257633f, 22.640385f, 5.069794f);
	//	pTmp->poBone[44].Q = Quat(Quat::Rot3::XYZ, 0.709378f, 0.537650f, -0.914868f);
	//	pTmp->poBone[44].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightThumb2
	//	pTmp->poBone[45].T = Vect(-22.746113f, 0.353250f, 0.112989f);
	//	pTmp->poBone[45].Q = Quat(Quat::Rot3::XYZ, 0.081785f, 0.342420f, 0.683621f);
	//	pTmp->poBone[45].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightThumb3
	//	pTmp->poBone[46].T = Vect(-17.496260f, -0.116582f, -0.089711f);
	//	pTmp->poBone[46].Q = Quat(Quat::Rot3::XYZ, 0.027026f, 0.198541f, 0.096615f);
	//	pTmp->poBone[46].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightThumb_End
	//	pTmp->poBone[47].T = Vect(-18.145161f, -0.286345f, -0.550298f);
	//	pTmp->poBone[47].Q = Quat(Quat::Rot3::XYZ, -0.000000f, 0.253073f, -0.000000f);
	//	pTmp->poBone[47].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightIndex1
	//	pTmp->poBone[48].T = Vect(-58.920994f, 18.573256f, -1.447088f);
	//	pTmp->poBone[48].Q = Quat(Quat::Rot3::XYZ, 1.586071f, 0.426614f, 0.025344f);
	//	pTmp->poBone[48].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightIndex2
	//	pTmp->poBone[49].T = Vect(-27.947296f, -0.000004f, -0.000010f);
	//	pTmp->poBone[49].Q = Quat(Quat::Rot3::XYZ, -0.015047f, 0.000544f, -0.279685f);
	//	pTmp->poBone[49].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightIndex3
	//	pTmp->poBone[50].T = Vect(-17.556936f, 0.000007f, 0.000005f);
	//	pTmp->poBone[50].Q = Quat(Quat::Rot3::XYZ, -0.015576f, -0.011470f, -0.281319f);
	//	pTmp->poBone[50].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightIndex_End
	//	pTmp->poBone[51].T = Vect(-12.971677f, 0.000051f, 0.000009f);
	//	pTmp->poBone[51].Q = Quat(Quat::Rot3::XYZ, 0.000005f, -0.020750f, -0.003191f);
	//	pTmp->poBone[51].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightMiddle1
	//	pTmp->poBone[52].T = Vect(-57.844231f, 4.515540f, -3.569327f);
	//	pTmp->poBone[52].Q = Quat(Quat::Rot3::XYZ, 1.525267f, 0.667962f, 0.018847f);
	//	pTmp->poBone[52].S = Vect(1.000000f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:RightMiddle2
	//	pTmp->poBone[53].T = Vect(-32.362286f, -0.000032f, 0.000014f);
	//	pTmp->poBone[53].Q = Quat(Quat::Rot3::XYZ, -0.024632f, 0.021536f, -0.488193f);
	//	pTmp->poBone[53].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightMiddle3
	//	pTmp->poBone[54].T = Vect(-18.401140f, -0.000102f, -0.000003f);
	//	pTmp->poBone[54].Q = Quat(Quat::Rot3::XYZ, -0.024626f, 0.021536f, -0.488206f);
	//	pTmp->poBone[54].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightMiddle_End
	//	pTmp->poBone[55].T = Vect(-13.346445f, -0.000007f, -0.000000f);
	//	pTmp->poBone[55].Q = Quat(Quat::Rot3::XYZ, 0.000003f, -0.000005f, -0.000006f);
	//	pTmp->poBone[55].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightRing1
	//	pTmp->poBone[56].T = Vect(-57.439896f, -9.168852f, -2.940132f);
	//	pTmp->poBone[56].Q = Quat(Quat::Rot3::XYZ, 1.483317f, 0.852179f, 0.008559f);
	//	pTmp->poBone[56].S = Vect(1.000000f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:RightRing2
	//	pTmp->poBone[57].T = Vect(-30.068869f, -0.000103f, -0.000006f);
	//	pTmp->poBone[57].Q = Quat(Quat::Rot3::XYZ, -0.041361f, 0.045479f, -0.696690f);
	//	pTmp->poBone[57].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightRing3
	//	pTmp->poBone[58].T = Vect(-15.270673f, 0.000013f, -0.000004f);
	//	pTmp->poBone[58].Q = Quat(Quat::Rot3::XYZ, -0.041363f, 0.045478f, -0.696685f);
	//	pTmp->poBone[58].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightRing_End
	//	pTmp->poBone[59].T = Vect(-12.744834f, -0.000026f, -0.000002f);
	//	pTmp->poBone[59].Q = Quat(Quat::Rot3::XYZ, 0.000002f, -0.000000f, -0.000031f);
	//	pTmp->poBone[59].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightPinky1
	//	pTmp->poBone[60].T = Vect(-56.620979f, -20.249563f, 1.450169f);
	//	pTmp->poBone[60].Q = Quat(Quat::Rot3::XYZ, 1.406680f, 0.858990f, 0.038173f);
	//	pTmp->poBone[60].S = Vect(1.000000f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:RightPinky2
	//	pTmp->poBone[61].T = Vect(-19.894527f, -0.000120f, 0.000016f);
	//	pTmp->poBone[61].Q = Quat(Quat::Rot3::XYZ, -0.071022f, 0.120399f, -0.831170f);
	//	pTmp->poBone[61].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightPinky3
	//	pTmp->poBone[62].T = Vect(-12.904246f, -0.000024f, -0.000012f);
	//	pTmp->poBone[62].Q = Quat(Quat::Rot3::XYZ, -0.071021f, 0.120393f, -0.831149f);
	//	pTmp->poBone[62].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightPinky_End
	//	pTmp->poBone[63].T = Vect(-10.892068f, 0.000086f, 0.000016f);
	//	pTmp->poBone[63].Q = Quat(Quat::Rot3::XYZ, 0.000003f, 0.000015f, 0.000012f);
	//	pTmp->poBone[63].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftHips_Dummy
	//	pTmp->poBone[64].T = Vect(26.547031f, -13.007975f, -6.758529f);
	//	pTmp->poBone[64].Q = Quat(Quat::Rot3::XYZ, 1.413518f, 1.560784f, 1.413525f);
	//	pTmp->poBone[64].S = Vect(0.999997f, 0.999997f, 0.999997f);

	//	//     Node Name: Humanoid:LeftUpLeg
	//	pTmp->poBone[65].T = Vect(0.000000f, 0.000000f, 29.999998f);
	//	pTmp->poBone[65].Q = Quat(Quat::Rot3::XYZ, -0.050134f, -0.055586f, -0.099378f);
	//	pTmp->poBone[65].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftUpLegRoll
	//	pTmp->poBone[66].T = Vect(112.510529f, 0.000000f, 0.000020f);
	//	pTmp->poBone[66].Q = Quat(Quat::Rot3::XYZ, -0.030479f, -0.000000f, -0.000000f);
	//	pTmp->poBone[66].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftLeg
	//	pTmp->poBone[67].T = Vect(129.020859f, -0.000000f, 0.000070f);
	//	pTmp->poBone[67].Q = Quat(Quat::Rot3::XYZ, 0.061168f, -0.006313f, 0.545807f);
	//	pTmp->poBone[67].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftLegRoll
	//	pTmp->poBone[68].T = Vect(183.400421f, -0.000001f, -0.000005f);
	//	pTmp->poBone[68].Q = Quat(Quat::Rot3::XYZ, 0.090728f, 0.000000f, 0.000000f);
	//	pTmp->poBone[68].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftFoot
	//	pTmp->poBone[69].T = Vect(71.066254f, 0.000009f, 0.000012f);
	//	pTmp->poBone[69].Q = Quat(Quat::Rot3::XYZ, -0.050824f, 0.007916f, -1.238458f);
	//	pTmp->poBone[69].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftToes
	//	pTmp->poBone[70].T = Vect(64.739685f, 0.000002f, -0.000000f);
	//	pTmp->poBone[70].Q = Quat(Quat::Rot3::XYZ, 0.000000f, 0.000000f, -0.619500f);
	//	pTmp->poBone[70].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftToes_End
	//	pTmp->poBone[71].T = Vect(56.363625f, 0.000004f, 0.000005f);
	//	pTmp->poBone[71].Q = Quat(Quat::Rot3::XYZ, 0.000000f, 0.000000f, 0.000000f);
	//	pTmp->poBone[71].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightHips_Dummy
	//	pTmp->poBone[72].T = Vect(-26.547031f, -13.007975f, -6.758529f);
	//	pTmp->poBone[72].Q = Quat(Quat::Rot3::XYZ, -1.728055f, -1.560784f, -1.413538f);
	//	pTmp->poBone[72].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightUpLeg
	//	pTmp->poBone[73].T = Vect(-0.000000f, -0.000000f, -29.999926f);
	//	pTmp->poBone[73].Q = Quat(Quat::Rot3::XYZ, -0.047229f, 0.278622f, -1.509598f);
	//	pTmp->poBone[73].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightUpLegRoll
	//	pTmp->poBone[74].T = Vect(-112.510269f, -0.000004f, -0.000017f);
	//	pTmp->poBone[74].Q = Quat(Quat::Rot3::XYZ, 0.028273f, -0.000000f, 0.000000f);
	//	pTmp->poBone[74].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightLeg
	//	pTmp->poBone[75].T = Vect(-129.020554f, 0.000002f, -0.000071f);
	//	pTmp->poBone[75].Q = Quat(Quat::Rot3::XYZ, -0.025544f, 0.002375f, 2.002476f);
	//	pTmp->poBone[75].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightLegRoll
	//	pTmp->poBone[76].T = Vect(-183.399948f, -0.000002f, 0.000005f);
	//	pTmp->poBone[76].Q = Quat(Quat::Rot3::XYZ, -0.040242f, -0.000000f, 0.000000f);
	//	pTmp->poBone[76].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightFoot
	//	pTmp->poBone[77].T = Vect(-71.066093f, -0.000015f, -0.000011f);
	//	pTmp->poBone[77].Q = Quat(Quat::Rot3::XYZ, -0.040273f, 0.003243f, -1.226986f);
	//	pTmp->poBone[77].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightToes
	//	pTmp->poBone[78].T = Vect(-64.739525f, 0.000005f, 0.000000f);
	//	pTmp->poBone[78].Q = Quat(Quat::Rot3::XYZ, 0.000000f, 0.000000f, -0.619500f);
	//	pTmp->poBone[78].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightToes_End
	//	pTmp->poBone[79].T = Vect(-56.363506f, -0.000007f, -0.000007f);
	//	pTmp->poBone[79].Q = Quat(Quat::Rot3::XYZ, 0.000000f, -0.000000f, 0.000000f);
	//	pTmp->poBone[79].S = Vect(1.000000f, 1.000000f, 1.000000f);


	//	// --------  Frame 5 ----------------------------

	//	FrameBucket *pTmp2 = new FrameBucket();
	//	pTmp2->prevBucket = pTmp;
	//	pTmp2->nextBucket = nullptr;
	//	pTmp2->KeyTime = 5 * Time(Time::Duration::NTSC_30_FRAME);
	//	pTmp2->poBone = new Bone[(unsigned int)this->numBones];
	//	pTmp->nextBucket = pTmp2;

	//	// ------- Key Time: 5 ms: 166 ------------------ 

	//	pTmp = pTmp2;

	//	////     Node Name: RootNode
	//	//pTmp->poBone[-3].T = Vect(0.000000f, 0.000000f, 0.000000f);
	//	//pTmp->poBone[-3].Q = Quat(Quat::Rot3::XYZ, 0.000000f, -0.000000f, 0.000000f);
	//	//pTmp->poBone[-3].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	////     Node Name: Humanoid:MaxToFilmbox_Root
	//	//pTmp->poBone[-2].T = Vect(0.000000f, 0.000000f, 0.000000f);
	//	//pTmp->poBone[-2].Q = Quat(Quat::Rot3::XYZ, -1.570796f, -0.000000f, 0.000000f);
	//	//pTmp->poBone[-2].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	////     Node Name: Humanoid:Reference
	//	//pTmp->poBone[-1].T = Vect(0.000000f, 0.000000f, 0.000000f);
	//	//pTmp->poBone[-1].Q = Quat(Quat::Rot3::XYZ, 0.000000f, -0.000000f, 0.000000f);
	//	//pTmp->poBone[-1].S = Vect(0.165000f, 0.165000f, 0.165000f);

	//	//     Node Name: Humanoid:Hips
	//	pTmp->poBone[0].T = Vect(-258.587677f, 866.939819f, 525.831482f);
	//	pTmp->poBone[0].Q = Quat(Quat::Rot3::XYZ, 0.340421f, 0.004302f, -0.131523f);
	//	pTmp->poBone[0].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Spine_Dummy
	//	pTmp->poBone[1].T = Vect(0.000014f, -13.665373f, 18.144430f);
	//	pTmp->poBone[1].Q = Quat(Quat::Rot3::XYZ, -3.141593f, -1.570796f, 0.000000f);
	//	pTmp->poBone[1].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Spine
	//	pTmp->poBone[2].T = Vect(73.400002f, 0.000001f, 0.000000f);
	//	pTmp->poBone[2].Q = Quat(Quat::Rot3::XYZ, -0.064586f, 0.020186f, 0.029162f);
	//	pTmp->poBone[2].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Spine1
	//	pTmp->poBone[3].T = Vect(70.001335f, -0.000002f, -0.000000f);
	//	pTmp->poBone[3].Q = Quat(Quat::Rot3::XYZ, -0.106095f, 0.024672f, 0.058030f);
	//	pTmp->poBone[3].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Spine2
	//	pTmp->poBone[4].T = Vect(71.400032f, -0.000000f, 0.000000f);
	//	pTmp->poBone[4].Q = Quat(Quat::Rot3::XYZ, -0.106087f, 0.024672f, 0.058030f);
	//	pTmp->poBone[4].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Neck
	//	pTmp->poBone[5].T = Vect(121.576225f, -0.000001f, -0.000000f);
	//	pTmp->poBone[5].Q = Quat(Quat::Rot3::XYZ, 0.038794f, -0.007953f, 0.166151f);
	//	pTmp->poBone[5].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Head
	//	pTmp->poBone[6].T = Vect(67.855774f, 0.000039f, -0.000000f);
	//	pTmp->poBone[6].Q = Quat(Quat::Rot3::XYZ, 0.063734f, -0.048411f, -0.075629f);
	//	pTmp->poBone[6].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Head_End
	//	pTmp->poBone[7].T = Vect(92.789192f, 0.000013f, -0.000000f);
	//	pTmp->poBone[7].Q = Quat(Quat::Rot3::XYZ, -0.000000f, 0.000003f, 0.000000f);
	//	pTmp->poBone[7].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftShoulder_Dummy
	//	pTmp->poBone[8].T = Vect(65.589684f, 2.530523f, 21.279638f);
	//	pTmp->poBone[8].Q = Quat(Quat::Rot3::XYZ, 1.436787f, 0.214620f, 3.102453f);
	//	pTmp->poBone[8].S = Vect(0.999999f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftShoulder
	//	pTmp->poBone[9].T = Vect(-36.845097f, -0.000012f, -0.000005f);
	//	pTmp->poBone[9].Q = Quat(Quat::Rot3::XYZ, -0.040098f, -0.002447f, -0.030272f);
	//	pTmp->poBone[9].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftArm_Dummy
	//	pTmp->poBone[10].T = Vect(-1.681311f, 62.969807f, -0.310574f);
	//	pTmp->poBone[10].Q = Quat(Quat::Rot3::XYZ, 1.507469f, 0.072192f, -2.069909f);
	//	pTmp->poBone[10].S = Vect(1.000002f, 0.999999f, 1.000002f);

	//	//     Node Name: Humanoid:LeftArm
	//	pTmp->poBone[11].T = Vect(-20.843035f, -1.709348f, 11.313786f);
	//	pTmp->poBone[11].Q = Quat(Quat::Rot3::XYZ, 0.200827f, 0.357298f, 3.020103f);
	//	pTmp->poBone[11].S = Vect(0.999999f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftArmRoll
	//	pTmp->poBone[12].T = Vect(78.950714f, 0.000008f, -0.000000f);
	//	pTmp->poBone[12].Q = Quat(Quat::Rot3::XYZ, 0.586154f, 0.000000f, 0.000000f);
	//	pTmp->poBone[12].S = Vect(1.000000f, 0.999999f, 1.000001f);

	//	//     Node Name: Humanoid:LeftForeArm
	//	pTmp->poBone[13].T = Vect(65.979294f, 0.000019f, -0.000010f);
	//	pTmp->poBone[13].Q = Quat(Quat::Rot3::XYZ, 0.164049f, 0.027976f, -1.926609f);
	//	pTmp->poBone[13].S = Vect(1.000002f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftForeArmRoll
	//	pTmp->poBone[14].T = Vect(101.404221f, 0.000009f, 0.000010f);
	//	pTmp->poBone[14].Q = Quat(Quat::Rot3::XYZ, 0.245294f, 0.000000f, 0.000000f);
	//	pTmp->poBone[14].S = Vect(0.999999f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftHand
	//	pTmp->poBone[15].T = Vect(71.692039f, 0.000010f, 0.000004f);
	//	pTmp->poBone[15].Q = Quat(Quat::Rot3::XYZ, -0.058125f, -0.418784f, 0.271912f);
	//	pTmp->poBone[15].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftThumb1
	//	pTmp->poBone[16].T = Vect(29.257690f, -22.640423f, -5.069802f);
	//	pTmp->poBone[16].Q = Quat(Quat::Rot3::XYZ, 0.709381f, 0.537656f, -0.914867f);
	//	pTmp->poBone[16].S = Vect(0.999999f, 1.000001f, 1.000003f);

	//	//     Node Name: Humanoid:LeftThumb2
	//	pTmp->poBone[17].T = Vect(22.746157f, -0.353276f, -0.113011f);
	//	pTmp->poBone[17].Q = Quat(Quat::Rot3::XYZ, 0.081784f, 0.342417f, 0.683623f);
	//	pTmp->poBone[17].S = Vect(0.999997f, 1.000002f, 1.000001f);

	//	//     Node Name: Humanoid:LeftThumb3
	//	pTmp->poBone[18].T = Vect(17.496328f, 0.116578f, 0.089712f);
	//	pTmp->poBone[18].Q = Quat(Quat::Rot3::XYZ, 0.027024f, 0.198537f, 0.096617f);
	//	pTmp->poBone[18].S = Vect(0.999999f, 1.000001f, 1.000000f);

	//	//     Node Name: Humanoid:LeftThumb_End
	//	pTmp->poBone[19].T = Vect(18.145250f, 0.286301f, 0.550287f);
	//	pTmp->poBone[19].Q = Quat(Quat::Rot3::XYZ, -0.000000f, 0.253071f, -0.000000f);
	//	pTmp->poBone[19].S = Vect(0.999999f, 1.000000f, 1.000001f);

	//	//     Node Name: Humanoid:LeftIndex1
	//	pTmp->poBone[20].T = Vect(58.921104f, -18.573286f, 1.447069f);
	//	pTmp->poBone[20].Q = Quat(Quat::Rot3::XYZ, 1.586099f, 0.426615f, 0.025352f);
	//	pTmp->poBone[20].S = Vect(1.000001f, 0.999997f, 1.000005f);

	//	//     Node Name: Humanoid:LeftIndex2
	//	pTmp->poBone[21].T = Vect(27.947330f, -0.000028f, 0.000011f);
	//	pTmp->poBone[21].Q = Quat(Quat::Rot3::XYZ, -0.015023f, 0.000552f, -0.279687f);
	//	pTmp->poBone[21].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftIndex3
	//	pTmp->poBone[22].T = Vect(17.556934f, -0.000062f, -0.000005f);
	//	pTmp->poBone[22].Q = Quat(Quat::Rot3::XYZ, -0.015553f, -0.011463f, -0.281321f);
	//	pTmp->poBone[22].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftIndex_End
	//	pTmp->poBone[23].T = Vect(12.971698f, -0.000033f, -0.000009f);
	//	pTmp->poBone[23].Q = Quat(Quat::Rot3::XYZ, 0.000005f, -0.020750f, -0.003191f);
	//	pTmp->poBone[23].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftMiddle1
	//	pTmp->poBone[24].T = Vect(57.844334f, -4.515547f, 3.569310f);
	//	pTmp->poBone[24].Q = Quat(Quat::Rot3::XYZ, 1.525321f, 0.667963f, 0.018870f);
	//	pTmp->poBone[24].S = Vect(1.000001f, 0.999997f, 1.000005f);

	//	//     Node Name: Humanoid:LeftMiddle2
	//	pTmp->poBone[25].T = Vect(32.362274f, -0.000006f, -0.000017f);
	//	pTmp->poBone[25].Q = Quat(Quat::Rot3::XYZ, -0.024592f, 0.021554f, -0.488195f);
	//	pTmp->poBone[25].S = Vect(1.000001f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:LeftMiddle3
	//	pTmp->poBone[26].T = Vect(18.401133f, 0.000069f, 0.000003f);
	//	pTmp->poBone[26].Q = Quat(Quat::Rot3::XYZ, -0.024587f, 0.021555f, -0.488208f);
	//	pTmp->poBone[26].S = Vect(1.000001f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:LeftMiddle_End
	//	pTmp->poBone[27].T = Vect(13.346432f, 0.000025f, -0.000002f);
	//	pTmp->poBone[27].Q = Quat(Quat::Rot3::XYZ, 0.000003f, -0.000005f, -0.000006f);
	//	pTmp->poBone[27].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftRing1
	//	pTmp->poBone[28].T = Vect(57.439991f, 9.168867f, 2.940117f);
	//	pTmp->poBone[28].Q = Quat(Quat::Rot3::XYZ, 1.483405f, 0.852180f, 0.008604f);
	//	pTmp->poBone[28].S = Vect(1.000001f, 0.999997f, 1.000005f);

	//	//     Node Name: Humanoid:LeftRing2
	//	pTmp->poBone[29].T = Vect(30.068848f, 0.000063f, 0.000002f);
	//	pTmp->poBone[29].Q = Quat(Quat::Rot3::XYZ, -0.041307f, 0.045509f, -0.696692f);
	//	pTmp->poBone[29].S = Vect(1.000001f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftRing3
	//	pTmp->poBone[30].T = Vect(15.270657f, 0.000024f, 0.000004f);
	//	pTmp->poBone[30].Q = Quat(Quat::Rot3::XYZ, -0.041310f, 0.045508f, -0.696687f);
	//	pTmp->poBone[30].S = Vect(1.000001f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftRing_End
	//	pTmp->poBone[31].T = Vect(12.744839f, 0.000002f, -0.000001f);
	//	pTmp->poBone[31].Q = Quat(Quat::Rot3::XYZ, 0.000002f, -0.000000f, -0.000031f);
	//	pTmp->poBone[31].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftPinky1
	//	pTmp->poBone[32].T = Vect(56.621067f, 20.249598f, -1.450183f);
	//	pTmp->poBone[32].Q = Quat(Quat::Rot3::XYZ, 1.406783f, 0.858990f, 0.038222f);
	//	pTmp->poBone[32].S = Vect(1.000001f, 0.999997f, 1.000005f);

	//	//     Node Name: Humanoid:LeftPinky2
	//	pTmp->poBone[33].T = Vect(19.894529f, 0.000152f, -0.000017f);
	//	pTmp->poBone[33].Q = Quat(Quat::Rot3::XYZ, -0.070957f, 0.120430f, -0.831169f);
	//	pTmp->poBone[33].S = Vect(1.000001f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftPinky3
	//	pTmp->poBone[34].T = Vect(12.904247f, 0.000005f, 0.000009f);
	//	pTmp->poBone[34].Q = Quat(Quat::Rot3::XYZ, -0.070955f, 0.120425f, -0.831149f);
	//	pTmp->poBone[34].S = Vect(1.000002f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftPinky_End
	//	pTmp->poBone[35].T = Vect(10.892069f, -0.000096f, -0.000019f);
	//	pTmp->poBone[35].Q = Quat(Quat::Rot3::XYZ, 0.000003f, 0.000015f, 0.000012f);
	//	pTmp->poBone[35].S = Vect(0.999999f, 1.000000f, 1.000001f);

	//	//     Node Name: Humanoid:RightShoulder_Dummy
	//	pTmp->poBone[36].T = Vect(65.589691f, 2.532739f, -21.279242f);
	//	pTmp->poBone[36].Q = Quat(Quat::Rot3::XYZ, 1.436894f, 0.214624f, -0.039117f);
	//	pTmp->poBone[36].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightShoulder
	//	pTmp->poBone[37].T = Vect(36.845016f, 0.000015f, 0.000005f);
	//	pTmp->poBone[37].Q = Quat(Quat::Rot3::XYZ, -0.066969f, -0.003270f, -0.088282f);
	//	pTmp->poBone[37].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightArm_Dummy
	//	pTmp->poBone[38].T = Vect(1.681307f, -62.969696f, 0.310573f);
	//	pTmp->poBone[38].Q = Quat(Quat::Rot3::XYZ, 1.507469f, 0.072192f, -2.069909f);
	//	pTmp->poBone[38].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightArm
	//	pTmp->poBone[39].T = Vect(20.843040f, 1.709348f, -11.313787f);
	//	pTmp->poBone[39].Q = Quat(Quat::Rot3::XYZ, 0.230431f, -0.010555f, -2.758593f);
	//	pTmp->poBone[39].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightArmRoll
	//	pTmp->poBone[40].T = Vect(-78.950615f, 0.000004f, 0.000025f);
	//	pTmp->poBone[40].Q = Quat(Quat::Rot3::XYZ, 0.821399f, 0.000001f, 0.000000f);
	//	pTmp->poBone[40].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightForeArm
	//	pTmp->poBone[41].T = Vect(-65.979179f, -0.000020f, 0.000040f);
	//	pTmp->poBone[41].Q = Quat(Quat::Rot3::XYZ, 0.142007f, 0.020784f, -1.387469f);
	//	pTmp->poBone[41].S = Vect(1.000001f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:RightForeArmRoll
	//	pTmp->poBone[42].T = Vect(-101.404114f, -0.000006f, 0.000018f);
	//	pTmp->poBone[42].Q = Quat(Quat::Rot3::XYZ, 0.203280f, 0.000000f, -0.000000f);
	//	pTmp->poBone[42].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightHand
	//	pTmp->poBone[43].T = Vect(-71.691925f, -0.000011f, 0.000047f);
	//	pTmp->poBone[43].Q = Quat(Quat::Rot3::XYZ, -0.022815f, -0.457415f, 0.097938f);
	//	pTmp->poBone[43].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightThumb1
	//	pTmp->poBone[44].T = Vect(-29.257633f, 22.640385f, 5.069794f);
	//	pTmp->poBone[44].Q = Quat(Quat::Rot3::XYZ, 0.709378f, 0.537650f, -0.914868f);
	//	pTmp->poBone[44].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightThumb2
	//	pTmp->poBone[45].T = Vect(-22.746113f, 0.353250f, 0.112989f);
	//	pTmp->poBone[45].Q = Quat(Quat::Rot3::XYZ, 0.081785f, 0.342420f, 0.683621f);
	//	pTmp->poBone[45].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightThumb3
	//	pTmp->poBone[46].T = Vect(-17.496260f, -0.116582f, -0.089711f);
	//	pTmp->poBone[46].Q = Quat(Quat::Rot3::XYZ, 0.027026f, 0.198541f, 0.096615f);
	//	pTmp->poBone[46].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightThumb_End
	//	pTmp->poBone[47].T = Vect(-18.145161f, -0.286345f, -0.550298f);
	//	pTmp->poBone[47].Q = Quat(Quat::Rot3::XYZ, -0.000000f, 0.253073f, -0.000000f);
	//	pTmp->poBone[47].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightIndex1
	//	pTmp->poBone[48].T = Vect(-58.920994f, 18.573256f, -1.447088f);
	//	pTmp->poBone[48].Q = Quat(Quat::Rot3::XYZ, 1.586071f, 0.426614f, 0.025344f);
	//	pTmp->poBone[48].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightIndex2
	//	pTmp->poBone[49].T = Vect(-27.947296f, -0.000004f, -0.000010f);
	//	pTmp->poBone[49].Q = Quat(Quat::Rot3::XYZ, -0.015047f, 0.000544f, -0.279685f);
	//	pTmp->poBone[49].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightIndex3
	//	pTmp->poBone[50].T = Vect(-17.556936f, 0.000007f, 0.000005f);
	//	pTmp->poBone[50].Q = Quat(Quat::Rot3::XYZ, -0.015576f, -0.011470f, -0.281319f);
	//	pTmp->poBone[50].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightIndex_End
	//	pTmp->poBone[51].T = Vect(-12.971677f, 0.000051f, 0.000009f);
	//	pTmp->poBone[51].Q = Quat(Quat::Rot3::XYZ, 0.000005f, -0.020750f, -0.003191f);
	//	pTmp->poBone[51].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightMiddle1
	//	pTmp->poBone[52].T = Vect(-57.844231f, 4.515540f, -3.569327f);
	//	pTmp->poBone[52].Q = Quat(Quat::Rot3::XYZ, 1.525267f, 0.667962f, 0.018847f);
	//	pTmp->poBone[52].S = Vect(1.000000f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:RightMiddle2
	//	pTmp->poBone[53].T = Vect(-32.362286f, -0.000032f, 0.000014f);
	//	pTmp->poBone[53].Q = Quat(Quat::Rot3::XYZ, -0.024632f, 0.021536f, -0.488193f);
	//	pTmp->poBone[53].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightMiddle3
	//	pTmp->poBone[54].T = Vect(-18.401140f, -0.000102f, -0.000003f);
	//	pTmp->poBone[54].Q = Quat(Quat::Rot3::XYZ, -0.024626f, 0.021536f, -0.488206f);
	//	pTmp->poBone[54].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightMiddle_End
	//	pTmp->poBone[55].T = Vect(-13.346445f, -0.000007f, -0.000000f);
	//	pTmp->poBone[55].Q = Quat(Quat::Rot3::XYZ, 0.000003f, -0.000005f, -0.000006f);
	//	pTmp->poBone[55].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightRing1
	//	pTmp->poBone[56].T = Vect(-57.439896f, -9.168852f, -2.940132f);
	//	pTmp->poBone[56].Q = Quat(Quat::Rot3::XYZ, 1.483317f, 0.852179f, 0.008559f);
	//	pTmp->poBone[56].S = Vect(1.000000f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:RightRing2
	//	pTmp->poBone[57].T = Vect(-30.068869f, -0.000103f, -0.000006f);
	//	pTmp->poBone[57].Q = Quat(Quat::Rot3::XYZ, -0.041361f, 0.045479f, -0.696690f);
	//	pTmp->poBone[57].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightRing3
	//	pTmp->poBone[58].T = Vect(-15.270673f, 0.000013f, -0.000004f);
	//	pTmp->poBone[58].Q = Quat(Quat::Rot3::XYZ, -0.041363f, 0.045478f, -0.696685f);
	//	pTmp->poBone[58].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightRing_End
	//	pTmp->poBone[59].T = Vect(-12.744834f, -0.000026f, -0.000002f);
	//	pTmp->poBone[59].Q = Quat(Quat::Rot3::XYZ, 0.000002f, -0.000000f, -0.000031f);
	//	pTmp->poBone[59].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightPinky1
	//	pTmp->poBone[60].T = Vect(-56.620979f, -20.249563f, 1.450169f);
	//	pTmp->poBone[60].Q = Quat(Quat::Rot3::XYZ, 1.406680f, 0.858990f, 0.038173f);
	//	pTmp->poBone[60].S = Vect(1.000000f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:RightPinky2
	//	pTmp->poBone[61].T = Vect(-19.894527f, -0.000120f, 0.000016f);
	//	pTmp->poBone[61].Q = Quat(Quat::Rot3::XYZ, -0.071022f, 0.120399f, -0.831170f);
	//	pTmp->poBone[61].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightPinky3
	//	pTmp->poBone[62].T = Vect(-12.904246f, -0.000024f, -0.000012f);
	//	pTmp->poBone[62].Q = Quat(Quat::Rot3::XYZ, -0.071021f, 0.120393f, -0.831149f);
	//	pTmp->poBone[62].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightPinky_End
	//	pTmp->poBone[63].T = Vect(-10.892068f, 0.000086f, 0.000016f);
	//	pTmp->poBone[63].Q = Quat(Quat::Rot3::XYZ, 0.000003f, 0.000015f, 0.000012f);
	//	pTmp->poBone[63].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftHips_Dummy
	//	pTmp->poBone[64].T = Vect(26.547031f, -13.007975f, -6.758529f);
	//	pTmp->poBone[64].Q = Quat(Quat::Rot3::XYZ, 1.413518f, 1.560784f, 1.413525f);
	//	pTmp->poBone[64].S = Vect(0.999997f, 0.999997f, 0.999997f);

	//	//     Node Name: Humanoid:LeftUpLeg
	//	pTmp->poBone[65].T = Vect(0.000000f, 0.000000f, 29.999998f);
	//	pTmp->poBone[65].Q = Quat(Quat::Rot3::XYZ, -0.004171f, 0.214655f, 0.116601f);
	//	pTmp->poBone[65].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftUpLegRoll
	//	pTmp->poBone[66].T = Vect(112.510529f, -0.000004f, 0.000020f);
	//	pTmp->poBone[66].Q = Quat(Quat::Rot3::XYZ, -0.219564f, -0.000000f, -0.000000f);
	//	pTmp->poBone[66].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftLeg
	//	pTmp->poBone[67].T = Vect(129.020859f, -0.000000f, 0.000070f);
	//	pTmp->poBone[67].Q = Quat(Quat::Rot3::XYZ, 0.060560f, -0.009612f, 1.294742f);
	//	pTmp->poBone[67].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftLegRoll
	//	pTmp->poBone[68].T = Vect(183.400421f, -0.000001f, -0.000005f);
	//	pTmp->poBone[68].Q = Quat(Quat::Rot3::XYZ, 0.088785f, 0.000000f, 0.000000f);
	//	pTmp->poBone[68].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftFoot
	//	pTmp->poBone[69].T = Vect(71.066254f, 0.000009f, 0.000012f);
	//	pTmp->poBone[69].Q = Quat(Quat::Rot3::XYZ, 0.167384f, -0.191204f, -0.351182f);
	//	pTmp->poBone[69].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftToes
	//	pTmp->poBone[70].T = Vect(64.739685f, 0.000002f, -0.000000f);
	//	pTmp->poBone[70].Q = Quat(Quat::Rot3::XYZ, 0.000000f, 0.000000f, -0.619500f);
	//	pTmp->poBone[70].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftToes_End
	//	pTmp->poBone[71].T = Vect(56.363625f, 0.000004f, 0.000005f);
	//	pTmp->poBone[71].Q = Quat(Quat::Rot3::XYZ, 0.000000f, 0.000000f, 0.000000f);
	//	pTmp->poBone[71].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightHips_Dummy
	//	pTmp->poBone[72].T = Vect(-26.547031f, -13.007975f, -6.758529f);
	//	pTmp->poBone[72].Q = Quat(Quat::Rot3::XYZ, -1.728055f, -1.560784f, -1.413538f);
	//	pTmp->poBone[72].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightUpLeg
	//	pTmp->poBone[73].T = Vect(-0.000000f, 0.000000f, -29.999926f);
	//	pTmp->poBone[73].Q = Quat(Quat::Rot3::XYZ, 0.029698f, 0.138462f, -1.029297f);
	//	pTmp->poBone[73].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightUpLegRoll
	//	pTmp->poBone[74].T = Vect(-112.510269f, -0.000004f, -0.000017f);
	//	pTmp->poBone[74].Q = Quat(Quat::Rot3::XYZ, 0.043694f, -0.000000f, 0.000000f);
	//	pTmp->poBone[74].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightLeg
	//	pTmp->poBone[75].T = Vect(-129.020554f, 0.000002f, -0.000071f);
	//	pTmp->poBone[75].Q = Quat(Quat::Rot3::XYZ, 0.056139f, -0.006495f, 0.374317f);
	//	pTmp->poBone[75].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightLegRoll
	//	pTmp->poBone[76].T = Vect(-183.399948f, -0.000003f, 0.000006f);
	//	pTmp->poBone[76].Q = Quat(Quat::Rot3::XYZ, 0.083544f, -0.000000f, 0.000000f);
	//	pTmp->poBone[76].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightFoot
	//	pTmp->poBone[77].T = Vect(-71.066093f, -0.000015f, -0.000011f);
	//	pTmp->poBone[77].Q = Quat(Quat::Rot3::XYZ, -0.030037f, -0.003626f, -1.257623f);
	//	pTmp->poBone[77].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightToes
	//	pTmp->poBone[78].T = Vect(-64.739525f, 0.000005f, 0.000000f);
	//	pTmp->poBone[78].Q = Quat(Quat::Rot3::XYZ, 0.000000f, 0.000000f, -0.619500f);
	//	pTmp->poBone[78].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightToes_End
	//	pTmp->poBone[79].T = Vect(-56.363506f, -0.000007f, -0.000007f);
	//	pTmp->poBone[79].Q = Quat(Quat::Rot3::XYZ, 0.000000f, -0.000000f, 0.000000f);
	//	pTmp->poBone[79].S = Vect(1.000000f, 1.000000f, 1.000000f);


	//	// --------  Frame 10  ----------------------------

	//	FrameBucket *pTmp3 = new FrameBucket();
	//	pTmp3->prevBucket = pTmp;
	//	pTmp3->nextBucket = nullptr;
	//	pTmp3->KeyTime = 10 * Time(Time::Duration::NTSC_30_FRAME);
	//	pTmp3->poBone = new Bone[(unsigned int)this->numBones];
	//	pTmp->nextBucket = pTmp3;

	//	//------- Key Time: 10  ms: 333 ------------------ 

	//	pTmp = pTmp3;

	//	////     Node Name: RootNode
	//	//pTmp->poBone[-3].T = Vect(0.000000f, 0.000000f, 0.000000f);
	//	//pTmp->poBone[-3].Q = Quat(Quat::Rot3::XYZ, 0.000000f, -0.000000f, 0.000000f);
	//	//pTmp->poBone[-3].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	////     Node Name: Humanoid:MaxToFilmbox_Root
	//	//pTmp->poBone[-2].T = Vect(0.000000f, 0.000000f, 0.000000f);
	//	//pTmp->poBone[-2].Q = Quat(Quat::Rot3::XYZ, -1.570796f, -0.000000f, 0.000000f);
	//	//pTmp->poBone[-2].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	////     Node Name: Humanoid:Reference
	//	//pTmp->poBone[-1].T = Vect(0.000000f, 0.000000f, 0.000000f);
	//	//pTmp->poBone[-1].Q = Quat(Quat::Rot3::XYZ, 0.000000f, -0.000000f, 0.000000f);
	//	//pTmp->poBone[-1].S = Vect(0.165000f, 0.165000f, 0.165000f);

	//	//     Node Name: Humanoid:Hips
	//	pTmp->poBone[0].T = Vect(-256.619812f, 441.826447f, 501.189575f);
	//	pTmp->poBone[0].Q = Quat(Quat::Rot3::XYZ, 0.364318f, 0.149111f, -0.191151f);
	//	pTmp->poBone[0].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Spine_Dummy
	//	pTmp->poBone[1].T = Vect(0.000014f, -13.665373f, 18.144430f);
	//	pTmp->poBone[1].Q = Quat(Quat::Rot3::XYZ, -3.141593f, -1.570796f, 0.000000f);
	//	pTmp->poBone[1].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Spine
	//	pTmp->poBone[2].T = Vect(73.400002f, 0.000001f, 0.000000f);
	//	pTmp->poBone[2].Q = Quat(Quat::Rot3::XYZ, 0.020757f, 0.042474f, 0.020015f);
	//	pTmp->poBone[2].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Spine1
	//	pTmp->poBone[3].T = Vect(70.001335f, -0.000002f, -0.000000f);
	//	pTmp->poBone[3].Q = Quat(Quat::Rot3::XYZ, 0.041746f, 0.060430f, 0.040666f);
	//	pTmp->poBone[3].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Spine2
	//	pTmp->poBone[4].T = Vect(71.400032f, -0.000000f, 0.000000f);
	//	pTmp->poBone[4].Q = Quat(Quat::Rot3::XYZ, 0.041753f, 0.060429f, 0.040667f);
	//	pTmp->poBone[4].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Neck
	//	pTmp->poBone[5].T = Vect(121.576225f, -0.000001f, -0.000000f);
	//	pTmp->poBone[5].Q = Quat(Quat::Rot3::XYZ, 0.013605f, -0.085976f, 0.088848f);
	//	pTmp->poBone[5].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Head
	//	pTmp->poBone[6].T = Vect(67.855774f, 0.000039f, -0.000000f);
	//	pTmp->poBone[6].Q = Quat(Quat::Rot3::XYZ, 0.013175f, -0.082701f, -0.066379f);
	//	pTmp->poBone[6].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:Head_End
	//	pTmp->poBone[7].T = Vect(92.789192f, 0.000013f, -0.000000f);
	//	pTmp->poBone[7].Q = Quat(Quat::Rot3::XYZ, -0.000000f, 0.000003f, 0.000000f);
	//	pTmp->poBone[7].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftShoulder_Dummy
	//	pTmp->poBone[8].T = Vect(65.589684f, 2.530523f, 21.279638f);
	//	pTmp->poBone[8].Q = Quat(Quat::Rot3::XYZ, 1.436787f, 0.214620f, 3.102453f);
	//	pTmp->poBone[8].S = Vect(0.999999f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftShoulder
	//	pTmp->poBone[9].T = Vect(-36.845097f, -0.000012f, -0.000005f);
	//	pTmp->poBone[9].Q = Quat(Quat::Rot3::XYZ, -0.080324f, -0.002378f, -0.131281f);
	//	pTmp->poBone[9].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftArm_Dummy
	//	pTmp->poBone[10].T = Vect(-1.681311f, 62.969807f, -0.310574f);
	//	pTmp->poBone[10].Q = Quat(Quat::Rot3::XYZ, 1.507469f, 0.072192f, -2.069909f);
	//	pTmp->poBone[10].S = Vect(1.000002f, 0.999999f, 1.000002f);

	//	//     Node Name: Humanoid:LeftArm
	//	pTmp->poBone[11].T = Vect(-20.843035f, -1.709348f, 11.313786f);
	//	pTmp->poBone[11].Q = Quat(Quat::Rot3::XYZ, 0.235176f, 0.256876f, -2.792554f);
	//	pTmp->poBone[11].S = Vect(0.999999f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftArmRoll
	//	pTmp->poBone[12].T = Vect(78.950714f, 0.000008f, -0.000001f);
	//	pTmp->poBone[12].Q = Quat(Quat::Rot3::XYZ, 0.628308f, 0.000000f, 0.000000f);
	//	pTmp->poBone[12].S = Vect(1.000000f, 0.999999f, 1.000002f);

	//	//     Node Name: Humanoid:LeftForeArm
	//	pTmp->poBone[13].T = Vect(65.979294f, 0.000019f, -0.000010f);
	//	pTmp->poBone[13].Q = Quat(Quat::Rot3::XYZ, 0.135429f, 0.016311f, -1.134472f);
	//	pTmp->poBone[13].S = Vect(1.000002f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftForeArmRoll
	//	pTmp->poBone[14].T = Vect(101.404221f, 0.000009f, 0.000010f);
	//	pTmp->poBone[14].Q = Quat(Quat::Rot3::XYZ, 0.191644f, 0.000000f, 0.000000f);
	//	pTmp->poBone[14].S = Vect(0.999999f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftHand
	//	pTmp->poBone[15].T = Vect(71.692039f, 0.000010f, 0.000004f);
	//	pTmp->poBone[15].Q = Quat(Quat::Rot3::XYZ, -0.006825f, -0.137932f, 0.098730f);
	//	pTmp->poBone[15].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftThumb1
	//	pTmp->poBone[16].T = Vect(29.257690f, -22.640423f, -5.069802f);
	//	pTmp->poBone[16].Q = Quat(Quat::Rot3::XYZ, 0.709381f, 0.537656f, -0.914867f);
	//	pTmp->poBone[16].S = Vect(0.999999f, 1.000001f, 1.000003f);

	//	//     Node Name: Humanoid:LeftThumb2
	//	pTmp->poBone[17].T = Vect(22.746157f, -0.353276f, -0.113011f);
	//	pTmp->poBone[17].Q = Quat(Quat::Rot3::XYZ, 0.081784f, 0.342417f, 0.683623f);
	//	pTmp->poBone[17].S = Vect(0.999997f, 1.000002f, 1.000001f);

	//	//     Node Name: Humanoid:LeftThumb3
	//	pTmp->poBone[18].T = Vect(17.496328f, 0.116578f, 0.089712f);
	//	pTmp->poBone[18].Q = Quat(Quat::Rot3::XYZ, 0.027024f, 0.198537f, 0.096617f);
	//	pTmp->poBone[18].S = Vect(0.999999f, 1.000001f, 1.000000f);

	//	//     Node Name: Humanoid:LeftThumb_End
	//	pTmp->poBone[19].T = Vect(18.145250f, 0.286301f, 0.550287f);
	//	pTmp->poBone[19].Q = Quat(Quat::Rot3::XYZ, -0.000000f, 0.253071f, -0.000000f);
	//	pTmp->poBone[19].S = Vect(0.999999f, 1.000000f, 1.000001f);

	//	//     Node Name: Humanoid:LeftIndex1
	//	pTmp->poBone[20].T = Vect(58.921104f, -18.573286f, 1.447069f);
	//	pTmp->poBone[20].Q = Quat(Quat::Rot3::XYZ, 1.586099f, 0.426615f, 0.025352f);
	//	pTmp->poBone[20].S = Vect(1.000001f, 0.999997f, 1.000005f);

	//	//     Node Name: Humanoid:LeftIndex2
	//	pTmp->poBone[21].T = Vect(27.947330f, -0.000028f, 0.000011f);
	//	pTmp->poBone[21].Q = Quat(Quat::Rot3::XYZ, -0.015023f, 0.000552f, -0.279687f);
	//	pTmp->poBone[21].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftIndex3
	//	pTmp->poBone[22].T = Vect(17.556934f, -0.000062f, -0.000005f);
	//	pTmp->poBone[22].Q = Quat(Quat::Rot3::XYZ, -0.015553f, -0.011463f, -0.281321f);
	//	pTmp->poBone[22].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftIndex_End
	//	pTmp->poBone[23].T = Vect(12.971698f, -0.000033f, -0.000009f);
	//	pTmp->poBone[23].Q = Quat(Quat::Rot3::XYZ, 0.000005f, -0.020750f, -0.003191f);
	//	pTmp->poBone[23].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftMiddle1
	//	pTmp->poBone[24].T = Vect(57.844334f, -4.515547f, 3.569310f);
	//	pTmp->poBone[24].Q = Quat(Quat::Rot3::XYZ, 1.525321f, 0.667963f, 0.018870f);
	//	pTmp->poBone[24].S = Vect(1.000001f, 0.999997f, 1.000005f);

	//	//     Node Name: Humanoid:LeftMiddle2
	//	pTmp->poBone[25].T = Vect(32.362274f, -0.000006f, -0.000017f);
	//	pTmp->poBone[25].Q = Quat(Quat::Rot3::XYZ, -0.024592f, 0.021554f, -0.488195f);
	//	pTmp->poBone[25].S = Vect(1.000001f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:LeftMiddle3
	//	pTmp->poBone[26].T = Vect(18.401133f, 0.000069f, 0.000003f);
	//	pTmp->poBone[26].Q = Quat(Quat::Rot3::XYZ, -0.024587f, 0.021555f, -0.488208f);
	//	pTmp->poBone[26].S = Vect(1.000001f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:LeftMiddle_End
	//	pTmp->poBone[27].T = Vect(13.346432f, 0.000025f, -0.000002f);
	//	pTmp->poBone[27].Q = Quat(Quat::Rot3::XYZ, 0.000003f, -0.000005f, -0.000006f);
	//	pTmp->poBone[27].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftRing1
	//	pTmp->poBone[28].T = Vect(57.439991f, 9.168867f, 2.940117f);
	//	pTmp->poBone[28].Q = Quat(Quat::Rot3::XYZ, 1.483405f, 0.852180f, 0.008604f);
	//	pTmp->poBone[28].S = Vect(1.000001f, 0.999997f, 1.000005f);

	//	//     Node Name: Humanoid:LeftRing2
	//	pTmp->poBone[29].T = Vect(30.068848f, 0.000063f, 0.000002f);
	//	pTmp->poBone[29].Q = Quat(Quat::Rot3::XYZ, -0.041307f, 0.045509f, -0.696692f);
	//	pTmp->poBone[29].S = Vect(1.000001f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftRing3
	//	pTmp->poBone[30].T = Vect(15.270657f, 0.000024f, 0.000004f);
	//	pTmp->poBone[30].Q = Quat(Quat::Rot3::XYZ, -0.041310f, 0.045508f, -0.696687f);
	//	pTmp->poBone[30].S = Vect(1.000001f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftRing_End
	//	pTmp->poBone[31].T = Vect(12.744839f, 0.000002f, -0.000001f);
	//	pTmp->poBone[31].Q = Quat(Quat::Rot3::XYZ, 0.000002f, -0.000000f, -0.000031f);
	//	pTmp->poBone[31].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftPinky1
	//	pTmp->poBone[32].T = Vect(56.621067f, 20.249598f, -1.450183f);
	//	pTmp->poBone[32].Q = Quat(Quat::Rot3::XYZ, 1.406783f, 0.858990f, 0.038222f);
	//	pTmp->poBone[32].S = Vect(1.000001f, 0.999997f, 1.000005f);

	//	//     Node Name: Humanoid:LeftPinky2
	//	pTmp->poBone[33].T = Vect(19.894529f, 0.000152f, -0.000017f);
	//	pTmp->poBone[33].Q = Quat(Quat::Rot3::XYZ, -0.070957f, 0.120430f, -0.831169f);
	//	pTmp->poBone[33].S = Vect(1.000001f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftPinky3
	//	pTmp->poBone[34].T = Vect(12.904247f, 0.000005f, 0.000009f);
	//	pTmp->poBone[34].Q = Quat(Quat::Rot3::XYZ, -0.070955f, 0.120425f, -0.831149f);
	//	pTmp->poBone[34].S = Vect(1.000002f, 0.999998f, 1.000000f);

	//	//     Node Name: Humanoid:LeftPinky_End
	//	pTmp->poBone[35].T = Vect(10.892069f, -0.000096f, -0.000019f);
	//	pTmp->poBone[35].Q = Quat(Quat::Rot3::XYZ, 0.000003f, 0.000015f, 0.000012f);
	//	pTmp->poBone[35].S = Vect(0.999999f, 1.000000f, 1.000001f);

	//	//     Node Name: Humanoid:RightShoulder_Dummy
	//	pTmp->poBone[36].T = Vect(65.589691f, 2.532739f, -21.279242f);
	//	pTmp->poBone[36].Q = Quat(Quat::Rot3::XYZ, 1.436894f, 0.214624f, -0.039117f);
	//	pTmp->poBone[36].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightShoulder
	//	pTmp->poBone[37].T = Vect(36.845016f, 0.000015f, 0.000005f);
	//	pTmp->poBone[37].Q = Quat(Quat::Rot3::XYZ, -0.038851f, 0.010303f, -0.110741f);
	//	pTmp->poBone[37].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightArm_Dummy
	//	pTmp->poBone[38].T = Vect(1.681307f, -62.969696f, 0.310573f);
	//	pTmp->poBone[38].Q = Quat(Quat::Rot3::XYZ, 1.507469f, 0.072192f, -2.069909f);
	//	pTmp->poBone[38].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightArm
	//	pTmp->poBone[39].T = Vect(20.843040f, 1.709348f, -11.313787f);
	//	pTmp->poBone[39].Q = Quat(Quat::Rot3::XYZ, 0.129952f, 0.330781f, 2.608197f);
	//	pTmp->poBone[39].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightArmRoll
	//	pTmp->poBone[40].T = Vect(-78.950615f, -0.000003f, 0.000018f);
	//	pTmp->poBone[40].Q = Quat(Quat::Rot3::XYZ, 0.546464f, 0.000001f, 0.000000f);
	//	pTmp->poBone[40].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightForeArm
	//	pTmp->poBone[41].T = Vect(-65.979179f, -0.000020f, 0.000040f);
	//	pTmp->poBone[41].Q = Quat(Quat::Rot3::XYZ, 0.194680f, 0.022063f, -1.522365f);
	//	pTmp->poBone[41].S = Vect(1.000001f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:RightForeArmRoll
	//	pTmp->poBone[42].T = Vect(-101.404114f, -0.000004f, 0.000019f);
	//	pTmp->poBone[42].Q = Quat(Quat::Rot3::XYZ, 0.283913f, 0.000000f, -0.000000f);
	//	pTmp->poBone[42].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightHand
	//	pTmp->poBone[43].T = Vect(-71.691925f, -0.000011f, 0.000047f);
	//	pTmp->poBone[43].Q = Quat(Quat::Rot3::XYZ, -0.035990f, -0.407964f, 0.173566f);
	//	pTmp->poBone[43].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightThumb1
	//	pTmp->poBone[44].T = Vect(-29.257633f, 22.640385f, 5.069794f);
	//	pTmp->poBone[44].Q = Quat(Quat::Rot3::XYZ, 0.709378f, 0.537650f, -0.914868f);
	//	pTmp->poBone[44].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightThumb2
	//	pTmp->poBone[45].T = Vect(-22.746113f, 0.353250f, 0.112989f);
	//	pTmp->poBone[45].Q = Quat(Quat::Rot3::XYZ, 0.081785f, 0.342420f, 0.683621f);
	//	pTmp->poBone[45].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightThumb3
	//	pTmp->poBone[46].T = Vect(-17.496260f, -0.116582f, -0.089711f);
	//	pTmp->poBone[46].Q = Quat(Quat::Rot3::XYZ, 0.027026f, 0.198541f, 0.096615f);
	//	pTmp->poBone[46].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightThumb_End
	//	pTmp->poBone[47].T = Vect(-18.145161f, -0.286345f, -0.550298f);
	//	pTmp->poBone[47].Q = Quat(Quat::Rot3::XYZ, -0.000000f, 0.253073f, -0.000000f);
	//	pTmp->poBone[47].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightIndex1
	//	pTmp->poBone[48].T = Vect(-58.920994f, 18.573256f, -1.447088f);
	//	pTmp->poBone[48].Q = Quat(Quat::Rot3::XYZ, 1.586071f, 0.426614f, 0.025344f);
	//	pTmp->poBone[48].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightIndex2
	//	pTmp->poBone[49].T = Vect(-27.947296f, -0.000004f, -0.000010f);
	//	pTmp->poBone[49].Q = Quat(Quat::Rot3::XYZ, -0.015047f, 0.000544f, -0.279685f);
	//	pTmp->poBone[49].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightIndex3
	//	pTmp->poBone[50].T = Vect(-17.556936f, 0.000007f, 0.000005f);
	//	pTmp->poBone[50].Q = Quat(Quat::Rot3::XYZ, -0.015576f, -0.011470f, -0.281319f);
	//	pTmp->poBone[50].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightIndex_End
	//	pTmp->poBone[51].T = Vect(-12.971677f, 0.000051f, 0.000009f);
	//	pTmp->poBone[51].Q = Quat(Quat::Rot3::XYZ, 0.000005f, -0.020750f, -0.003191f);
	//	pTmp->poBone[51].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightMiddle1
	//	pTmp->poBone[52].T = Vect(-57.844231f, 4.515540f, -3.569327f);
	//	pTmp->poBone[52].Q = Quat(Quat::Rot3::XYZ, 1.525267f, 0.667962f, 0.018847f);
	//	pTmp->poBone[52].S = Vect(1.000000f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:RightMiddle2
	//	pTmp->poBone[53].T = Vect(-32.362286f, -0.000032f, 0.000014f);
	//	pTmp->poBone[53].Q = Quat(Quat::Rot3::XYZ, -0.024632f, 0.021536f, -0.488193f);
	//	pTmp->poBone[53].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightMiddle3
	//	pTmp->poBone[54].T = Vect(-18.401140f, -0.000102f, -0.000003f);
	//	pTmp->poBone[54].Q = Quat(Quat::Rot3::XYZ, -0.024626f, 0.021536f, -0.488206f);
	//	pTmp->poBone[54].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightMiddle_End
	//	pTmp->poBone[55].T = Vect(-13.346445f, -0.000007f, -0.000000f);
	//	pTmp->poBone[55].Q = Quat(Quat::Rot3::XYZ, 0.000003f, -0.000005f, -0.000006f);
	//	pTmp->poBone[55].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightRing1
	//	pTmp->poBone[56].T = Vect(-57.439896f, -9.168852f, -2.940132f);
	//	pTmp->poBone[56].Q = Quat(Quat::Rot3::XYZ, 1.483317f, 0.852179f, 0.008559f);
	//	pTmp->poBone[56].S = Vect(1.000000f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:RightRing2
	//	pTmp->poBone[57].T = Vect(-30.068869f, -0.000103f, -0.000006f);
	//	pTmp->poBone[57].Q = Quat(Quat::Rot3::XYZ, -0.041361f, 0.045479f, -0.696690f);
	//	pTmp->poBone[57].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightRing3
	//	pTmp->poBone[58].T = Vect(-15.270673f, 0.000013f, -0.000004f);
	//	pTmp->poBone[58].Q = Quat(Quat::Rot3::XYZ, -0.041363f, 0.045478f, -0.696685f);
	//	pTmp->poBone[58].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightRing_End
	//	pTmp->poBone[59].T = Vect(-12.744834f, -0.000026f, -0.000002f);
	//	pTmp->poBone[59].Q = Quat(Quat::Rot3::XYZ, 0.000002f, -0.000000f, -0.000031f);
	//	pTmp->poBone[59].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightPinky1
	//	pTmp->poBone[60].T = Vect(-56.620979f, -20.249563f, 1.450169f);
	//	pTmp->poBone[60].Q = Quat(Quat::Rot3::XYZ, 1.406680f, 0.858990f, 0.038173f);
	//	pTmp->poBone[60].S = Vect(1.000000f, 0.999999f, 1.000000f);

	//	//     Node Name: Humanoid:RightPinky2
	//	pTmp->poBone[61].T = Vect(-19.894527f, -0.000120f, 0.000016f);
	//	pTmp->poBone[61].Q = Quat(Quat::Rot3::XYZ, -0.071022f, 0.120399f, -0.831170f);
	//	pTmp->poBone[61].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightPinky3
	//	pTmp->poBone[62].T = Vect(-12.904246f, -0.000024f, -0.000012f);
	//	pTmp->poBone[62].Q = Quat(Quat::Rot3::XYZ, -0.071021f, 0.120393f, -0.831149f);
	//	pTmp->poBone[62].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightPinky_End
	//	pTmp->poBone[63].T = Vect(-10.892068f, 0.000086f, 0.000016f);
	//	pTmp->poBone[63].Q = Quat(Quat::Rot3::XYZ, 0.000003f, 0.000015f, 0.000012f);
	//	pTmp->poBone[63].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftHips_Dummy
	//	pTmp->poBone[64].T = Vect(26.547031f, -13.007975f, -6.758529f);
	//	pTmp->poBone[64].Q = Quat(Quat::Rot3::XYZ, 1.413518f, 1.560784f, 1.413525f);
	//	pTmp->poBone[64].S = Vect(0.999997f, 0.999997f, 0.999997f);

	//	//     Node Name: Humanoid:LeftUpLeg
	//	pTmp->poBone[65].T = Vect(0.000000f, 0.000000f, 29.999998f);
	//	pTmp->poBone[65].Q = Quat(Quat::Rot3::XYZ, -0.015828f, -0.264825f, -1.154203f);
	//	pTmp->poBone[65].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftUpLegRoll
	//	pTmp->poBone[66].T = Vect(112.510529f, -0.000000f, 0.000020f);
	//	pTmp->poBone[66].Q = Quat(Quat::Rot3::XYZ, -0.041067f, -0.000000f, -0.000000f);
	//	pTmp->poBone[66].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftLeg
	//	pTmp->poBone[67].T = Vect(129.020859f, -0.000000f, 0.000070f);
	//	pTmp->poBone[67].Q = Quat(Quat::Rot3::XYZ, 0.066096f, -0.007045f, 2.419231f);
	//	pTmp->poBone[67].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftLegRoll
	//	pTmp->poBone[68].T = Vect(183.400421f, -0.000001f, -0.000005f);
	//	pTmp->poBone[68].Q = Quat(Quat::Rot3::XYZ, 0.097820f, 0.000000f, 0.000000f);
	//	pTmp->poBone[68].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftFoot
	//	pTmp->poBone[69].T = Vect(71.066254f, 0.000009f, 0.000012f);
	//	pTmp->poBone[69].Q = Quat(Quat::Rot3::XYZ, 0.065816f, -0.053965f, -0.903218f);
	//	pTmp->poBone[69].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftToes
	//	pTmp->poBone[70].T = Vect(64.739685f, 0.000002f, -0.000000f);
	//	pTmp->poBone[70].Q = Quat(Quat::Rot3::XYZ, 0.000000f, 0.000000f, -0.619500f);
	//	pTmp->poBone[70].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:LeftToes_End
	//	pTmp->poBone[71].T = Vect(56.363625f, 0.000004f, 0.000005f);
	//	pTmp->poBone[71].Q = Quat(Quat::Rot3::XYZ, 0.000000f, 0.000000f, 0.000000f);
	//	pTmp->poBone[71].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightHips_Dummy
	//	pTmp->poBone[72].T = Vect(-26.547031f, -13.007975f, -6.758529f);
	//	pTmp->poBone[72].Q = Quat(Quat::Rot3::XYZ, -1.728055f, -1.560784f, -1.413538f);
	//	pTmp->poBone[72].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightUpLeg
	//	pTmp->poBone[73].T = Vect(-0.000000f, -0.000000f, -29.999926f);
	//	pTmp->poBone[73].Q = Quat(Quat::Rot3::XYZ, 0.031907f, 0.203667f, -0.537901f);
	//	pTmp->poBone[73].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightUpLegRoll
	//	pTmp->poBone[74].T = Vect(-112.510269f, -0.000002f, -0.000016f);
	//	pTmp->poBone[74].Q = Quat(Quat::Rot3::XYZ, -0.031790f, -0.000000f, 0.000000f);
	//	pTmp->poBone[74].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightLeg
	//	pTmp->poBone[75].T = Vect(-129.020554f, 0.000002f, -0.000071f);
	//	pTmp->poBone[75].Q = Quat(Quat::Rot3::XYZ, 0.078844f, -0.010243f, 0.829756f);
	//	pTmp->poBone[75].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightLegRoll
	//	pTmp->poBone[76].T = Vect(-183.399948f, -0.000003f, 0.000007f);
	//	pTmp->poBone[76].Q = Quat(Quat::Rot3::XYZ, 0.116728f, -0.000000f, 0.000000f);
	//	pTmp->poBone[76].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightFoot
	//	pTmp->poBone[77].T = Vect(-71.066093f, -0.000015f, -0.000011f);
	//	pTmp->poBone[77].Q = Quat(Quat::Rot3::XYZ, 0.192552f, -0.099209f, -1.429342f);
	//	pTmp->poBone[77].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightToes
	//	pTmp->poBone[78].T = Vect(-64.739525f, 0.000005f, 0.000000f);
	//	pTmp->poBone[78].Q = Quat(Quat::Rot3::XYZ, 0.029321f, -0.005328f, -0.715872f);
	//	pTmp->poBone[78].S = Vect(1.000000f, 1.000000f, 1.000000f);

	//	//     Node Name: Humanoid:RightToes_End
	//	pTmp->poBone[79].T = Vect(-56.363506f, -0.000007f, -0.000007f);
	//	pTmp->poBone[79].Q = Quat(Quat::Rot3::XYZ, 0.000000f, -0.000000f, 0.000000f);
	//	pTmp->poBone[79].S = Vect(1.000000f, 1.000000f, 1.000000f);


	//}
}

// --- End of File ---
