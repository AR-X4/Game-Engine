#include "ViewportWindow.h"

ViewportWindow::ViewportWindow(GUIWindow::Name NameIn)
	:GUIWindow(NameIn)
{
}

ViewportWindow::~ViewportWindow()
{
    //glDeleteFramebuffers(1, &RenderID);
   //glDeleteTextures(1, &ColorAttachment);
    //glDeleteTextures(1, &DepthAttachment);
}

void ViewportWindow::Update()
{


    GLsizei w = 1920;//change
    GLsizei h = 1080;//change


    if (RenderID)
    {
        glDeleteFramebuffers(1, &RenderID);
        glDeleteTextures(1, &ColorAttachment);
        glDeleteTextures(1, &DepthAttachment);
    }

    //Frame Buffer

    glCreateFramebuffers(1, &this->RenderID);
    glBindFramebuffer(GL_FRAMEBUFFER, this->RenderID);

    glCreateTextures(GL_TEXTURE_2D, 1, &this->ColorAttachment);
    glBindTexture(GL_TEXTURE_2D, this->ColorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->ColorAttachment, 0);


    //Depth Buffer

    glCreateTextures(GL_TEXTURE_2D, 1, &this->DepthAttachment);
    glBindTexture(GL_TEXTURE_2D, this->DepthAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, w, h, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthAttachment, 0);


    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);


    ImGui::Begin("Viewport");
    ImGui::Image((void*)ColorAttachment, ImVec2(1920.0, 1080.0));
    ImGui::End();

   
}
