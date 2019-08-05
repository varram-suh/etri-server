#pragma once

#include <mutex>
//#include "spdlog/spdlog.h"
#include <fstream>
#include <chrono>
#include "ks19937.h"
#include "qc-session.h"
#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions. You may freely use any other OpenGL loader such as: glew, glad, glLoadGen, etc.
//#include <glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "stb_image.h"

static const int width = 640;
static const int height = 480;
class ClientWindow {
public:
    GLuint texname;
    GLubyte* texture_data = nullptr;
    bool clientWindow = false;
    int videoWidth, videoHeight;
    int windowId = 0;
    bool requestSerialKey = false;
    bool requestActivateAll = false;

    ClientWindow() {
        clientWindow = true;
        static int ccc = 0;
        ccc++;
        windowId = ccc;
    }

    void changeImage(uint8_t* buffer, size_t length) {
        //{
            if(texture_data) {
                stbi_image_free(texture_data);
            }
            int width, height, channels;
            texture_data = stbi_load_from_memory(buffer, length, 
                                                &width,
                                                &height,
                                                &channels,
                                                STBI_rgb);
            videoWidth = width;
            videoHeight = height;
            glBindTexture(GL_TEXTURE_2D, texname);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, texture_data);
        //}
    }
    void initTexture() {
        glGenTextures(1, &texname); //Gen a new texture and store the handle in texname 
        glBindTexture(GL_TEXTURE_2D, texname);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     //GL_UNSIGNED_BYTE, texture_data);
    }

    void draw() {
        const int W = 1280;
        const int H = 720;
        ImGui::SetNextWindowSize(ImVec2(W + 10, H + 10),
                                 ImGuiSetCond_FirstUseEver); 

        ImGui::Begin((std::to_string(windowId) + "Another Window").c_str() , &clientWindow);
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImGui::Image((void*)texname, ImVec2( W,  H), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));
        //ImGui::GetWindowDrawList()->AddImage(
            //(void *)texname, ImVec2(pos.x, pos.y),
            //ImVec2(pos.x + W, pos.y + H), ImVec2(0, 0), ImVec2(1, 1));
        //{
            //ImVec2 pos = ImGui::GetCursorPos();
            //pos.y += 360;
            ////float line_height = ImGui::GetTextLineHeight();
            ////int lines_skipped = (int)((clip_rect.y - pos.y) / line_height) - 1;
            ////pos.y += lines_skipped * line_height;
            //ImGui::SetCursorPos(pos);
        //}
        ImGui::Text("Request License");
        if (ImGui::Button("REQUEST LICENSE")) {
            //clientWindow = false;
            requestSerialKey = true;
        }
        ImGui::Text("*Request ACTIVATE All*\n");
        if (ImGui::Button("ACTIVATE!")) {
            requestActivateAll = true;
        }
        static float progress = 0.0f, progress_dir = 1.0f;
        if (true)
        {
            progress += progress_dir * 0.4f * ImGui::GetIO().DeltaTime;
            if (progress >= +1.1f) { progress = +1.1f; progress_dir *= -1.0f; }
            if (progress <= -0.1f) { progress = -0.1f; progress_dir *= -1.0f; }
        }

        // Typically we would use ImVec2(-1.0f,0.0f) to use all available width, or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
        ImGui::ProgressBar(progress, ImVec2(0.0f,0.0f));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("Sound Bar");

        float progress_saturated = (progress < 0.0f) ? 0.0f : (progress > 1.0f) ? 1.0f : progress;
        char buf[32];
        sprintf(buf, "%d/%d", (int)(progress_saturated*1753), 1753);
        ImGui::ProgressBar(progress, ImVec2(0.f,0.f), buf);
        ImGui::End(); 
    }

    ~ClientWindow() {
        printf(".. th window is closed\n");
        if(texture_data != nullptr) {
            stbi_image_free(texture_data);
            texture_data = nullptr;
        }
        glDeleteTextures(1, &texname);
    }
};

class GUI {
    private:
        GUI() {
        }

    public:
        std::vector<std::shared_ptr<ClientWindow>> m_windows;
        static GUI* get() {
            static GUI* instance = nullptr;
            if(!instance){
                instance = new GUI();
            }
            return instance; 
        }

        std::shared_ptr<ClientWindow> addClient() {
            printf("%d\n", __LINE__);
            std::shared_ptr<ClientWindow> cw(new ClientWindow(), [](ClientWindow* p) { delete p; });
            cw->initTexture();
            m_windows.push_back(cw); 
            printf("%d\n", __LINE__);
            return cw;
        }
        ~GUI() { }
};
