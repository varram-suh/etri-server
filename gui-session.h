
#pragma once
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp> 
#include <boost/shared_ptr.hpp> 
#include <exception>
#include <map>
#include "gui.h"


using boost::asio::ip::tcp;
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
//namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

class GuiSession_depre {
    public:
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



        GLubyte texture_data[width*height * 3];
        GuiSession_depre(boost::asio::io_service& io_service, GLFWwindow* window)
            : m_ioService(io_service), m_window(window) { 
                start();
        } 
        void start()
        { 
            // Main loop

            if (!glfwWindowShouldClose(m_window))
            {
                // Poll and handle events (inputs, window resize, etc.)
                // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
                // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
                // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
                // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
                glfwPollEvents();

                // Start the ImGui frame
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                // 1. Show a simple window.
                // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
                {
                    static float f = 0.0f;
                    static int counter = 0;
                    ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
                    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
                    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
                    ImGui::Checkbox("Another Window", &show_another_window);

                    if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
                        counter++;
                    ImGui::SameLine();
                    ImGui::Text("counter = %d", counter);

                    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                }

                for(int i=0; i<GUI::get()->m_windows.size(); ) {
                    if(GUI::get()->m_windows[i]->clientWindow) {
                        GUI::get()->m_windows[i]->draw();
                        i++;
                    }
                    else {
                        GUI::get()->m_windows.erase(GUI::get()->m_windows.begin() + i);
                    }
                }
                // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
                if (show_another_window)
                {
                    const int W = 640;
                    const int H = 480;
                    ImGui::SetNextWindowSize(ImVec2(W + 10, H + 10),
                                             ImGuiSetCond_FirstUseEver);
                    ImGui::Begin("Another Window", &show_another_window);
                    {
                        ImGui::Text("Hello from another window!");

                    }
                    if (ImGui::Button("Close Me"))
                        show_another_window = false;
                    ImGui::End();
                }

                // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
                if (show_demo_window)
                {
                    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
                    ImGui::ShowDemoWindow(&show_demo_window);
                }

                // Rendering
                ImGui::Render();
                int display_w, display_h;
                glfwMakeContextCurrent(m_window);
                glfwGetFramebufferSize(m_window, &display_w, &display_h);
                glViewport(0, 0, display_w, display_h);
                glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
                glClear(GL_COLOR_BUFFER_BIT);
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                glfwMakeContextCurrent(m_window);
                glfwSwapBuffers(m_window);
                m_ioService.post(boost::bind(&GuiSession_depre::start, this));
            }

            // infinite loop
        }

    private:
        boost::asio::io_service& m_ioService; 
        GLFWwindow* m_window;
};



