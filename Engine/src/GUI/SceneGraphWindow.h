#ifndef SCENE_GRAPH_WINDOW
#define SCENE_GRAPH_WINDOW

#include "GUIWindow.h"
#include "GameObject/GameObjectManager.h"

namespace Azul {

	class SceneGraphWindow : public GUIWindow
	{

	public:
		SceneGraphWindow() = delete;
		SceneGraphWindow(const SceneGraphWindow&) = delete;
		SceneGraphWindow& operator = (SceneGraphWindow&) = delete;

		SceneGraphWindow(GUIWindow::Name NameIn);
		~SceneGraphWindow();


		void Update();

	private:
		void DisplayNode(PCSNode* pNode);
		void DisplayChildNodes(const PCSNode* pNode);

		PCSTree* pTree;
		PCSNode* pSelectedNode;
		//float data = 5.f;
		const float pos_step = 0.1f;
		const float scale_step = 0.001f;


	};
}

#endif