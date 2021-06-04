#ifndef EXPORTER_H
#define EXPORTER_H

#include "Model.h"

class Exporter {
public:

	Exporter() = delete;

	static bool ExportStaticModelPackage(Model& pModelIn, const std::string& pFilename, std::vector<std::string>& TextureNames);

	static bool ExportAnimModelPackage(std::vector<Model::VBO_VERTS>& VERTS_LIST, 
										std::vector<Model::TRILIST>& TRI_LIST,
										std::vector<Model::Bone>& skel, 
										std::vector<Model::BonePos>& BoneAnims,
										std::vector< Model::InvBonePose >& InvPoseTable,
										std::vector< Model::FinalVertEntry >& FinalBoneWeightTable,
										std::vector< int >& HierarchyTable,
										unsigned int BoneDepth,
										const std::string& pFilenameOut, 
										const std::string& pkgName, 
										const std::string& versionName);

	static bool ExportAnimPackage(std::vector<Model::BonePos>& BoneAnims,
								const std::string& pFilenameOut,
								const std::string& pkgName,
								const std::string& versionName);
};


#endif