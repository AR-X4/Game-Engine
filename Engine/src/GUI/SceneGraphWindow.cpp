#include "SceneGraphWindow.h"
//#include "PCSTreeForwardIterator.h"

namespace Azul 
{

	SceneGraphWindow::SceneGraphWindow(GUIWindow::Name NameIn)
		:GUIWindow(NameIn), pTree(GameObjectManager::GetPCSTree()), pSelectedNode(nullptr)
	{

	}

	SceneGraphWindow::~SceneGraphWindow()
	{
	}

	void SceneGraphWindow::Update()
	{

		//ImGui::ShowDemoWindow();

		ImGui::Begin("Scene Graph");
		
		
		//if (pSelectedNode != nullptr) {
		//	GameObject* ptmp = (GameObject*)pSelectedNode;
		//	float x = ptmp->GetPos()->x();
		//	float y = ptmp->GetPos()->y();
		//	float z = ptmp->GetPos()->z();


		//	//ImGuiInputTextFlags flag =  ImGuiInputTextFlags_AlwaysInsertMode;
		//	ImGui::InputScalar("X: ", ImGuiDataType_Float, &x, &step);// , & flag);
		//	ImGui::InputScalar("Y: ", ImGuiDataType_Float, &y, &step);// , & flag);
		//	ImGui::InputScalar("Z: ", ImGuiDataType_Float, &z, &step);// , & flag);


		//	ptmp->SetPos(x, y, z);
		//}

		
		//ImGui::InputFloat("xyz", &data);//, 0.0f, 0.0f, flag);

		DisplayChildNodes(pTree->GetRoot());

		ImGui::End();
	}
	void SceneGraphWindow::DisplayNode(PCSNode* pNode)
	{
		char name[64];
		pNode->GetName(name, 64);
		

		ImGuiTreeNodeFlags flags;
		if (pSelectedNode == pNode) 
		{
			flags = ImGuiTreeNodeFlags_Selected |
				ImGuiTreeNodeFlags_OpenOnArrow |
				//ImGuiTreeNodeFlags_DefaultOpen |
				//ImGuiTreeNodeFlags_Framed |
				ImGuiTreeNodeFlags_SpanAvailWidth |
				ImGuiTreeNodeFlags_AllowItemOverlap |
				ImGuiTreeNodeFlags_FramePadding;
			
		}
		else 
		{
			flags = ImGuiTreeNodeFlags_OpenOnArrow |
					//ImGuiTreeNodeFlags_DefaultOpen |
					//ImGuiTreeNodeFlags_Framed |
					ImGuiTreeNodeFlags_SpanAvailWidth |
					ImGuiTreeNodeFlags_AllowItemOverlap |
					ImGuiTreeNodeFlags_FramePadding;
		}
		if (pNode->GetNumChildren() == 0) {
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		bool open = ImGui::TreeNodeEx((void*)(intptr_t)pNode, flags, name);

		if (ImGui::IsItemClicked() && 
			pNode->GetParent() == pTree->GetRoot())
		{
			pSelectedNode = pNode;
		}

		if (open)
		{
			if (pSelectedNode == pNode) {
				GameObject* ptmp = (GameObject*)pSelectedNode;
				float pos_x = ptmp->GetPos()->x();
				float pos_y = ptmp->GetPos()->y();
				float pos_z = ptmp->GetPos()->z();

				float rot_x = ptmp->GetRot()->x();
				float rot_y = ptmp->GetRot()->y();
				float rot_z = ptmp->GetRot()->z();

				float scale = ptmp->GetScale()->x();


				ImGui::InputScalar("Pos X", ImGuiDataType_Float, &pos_x, &pos_step);
				ImGui::InputScalar("Pos Y", ImGuiDataType_Float, &pos_y, &pos_step);
				ImGui::InputScalar("Pos Z", ImGuiDataType_Float, &pos_z, &pos_step);

				ptmp->SetPos(pos_x, pos_y, pos_z);

				ImGui::InputScalar("Rot X", ImGuiDataType_Float, &rot_x, &pos_step);
				ImGui::InputScalar("Rot Y", ImGuiDataType_Float, &rot_y, &pos_step);
				ImGui::InputScalar("Rot Z", ImGuiDataType_Float, &rot_z, &pos_step);

				ptmp->SetRot(rot_x, rot_y, rot_z);

				ImGui::InputScalar("Scale X, Y, Z", ImGuiDataType_Float, &scale, &scale_step);
				

				if (scale > 0)
					ptmp->SetScale(scale, scale, scale);
			}

			DisplayChildNodes(pNode);

			// no more children
			ImGui::TreePop();
		}

	}
	void SceneGraphWindow::DisplayChildNodes(const PCSNode* pNode)
	{
		//recursion
		if (pNode->GetNumChildren() > 0)
		{
			PCSNode* tmp = pNode->GetChild();

			for (int i = 0; i < pNode->GetNumChildren(); i++)
			{
				DisplayNode(tmp);
				tmp = tmp->GetNextSibling();
			}
		}
		
	}
}
