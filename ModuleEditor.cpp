#include "Application.h"
#include "ModuleEditor.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleProgram.h"
#include "GL/glew.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_sdl.h"
#include "IMGUI/imgui_impl_opengl2.h"

bool ModuleEditor::Init()
{
	bool ret = true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = "Settings/imgui.ini";
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL2_Init();
	ImGui::StyleColorsDark();

	return ret;
}

update_status ModuleEditor::PreUpdate()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
	
	static float f = 0.0f;
	static int counter = 0;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	
	//Main Menu

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Save");
			ImGui::MenuItem("Load");
			if (ImGui::MenuItem("Exit"))
			{
				ImGui::EndMenu();
				ImGui::EndMainMenuBar();
				ImGui::EndFrame(); 
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("3D Tools"))
		{
			if (ImGui::MenuItem("Grid"))
			{
				if (App->renderer->showGrid == true)
					App->renderer->showGrid = false;
				else
					App->renderer->showGrid = true;
			}
			ImGui::MenuItem("Option 2");
			ImGui::MenuItem("Option 3");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Debug"))
		{
			ImGui::MenuItem("Option A");
			ImGui::MenuItem("Option B");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Misc"))
		{
			if (ImGui::MenuItem("Github"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/AniolAndres/CITADEL", NULL , NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("About"))
			{
				show_info_window = true;
			}
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	ImGui::Spacing();

	if (show_info_window)
	{
		if (ImGui::CollapsingHeader("About"))
		{
			ImGui::Text("Engine Name: Citadel");
			ImGui::Text("Author: Aniol Andres Guiu");
			ImGui::Text("Libraries used");
			ImGui::BulletText("SDL");
			ImGui::BulletText("GLEW");
			ImGui::BulletText("IMGUI");
			ImGui::BulletText("ASSIMP");
			ImGui::BulletText("MATHGEOLIB");
			ImGui::BulletText("DEVIL");
			ImGui::BulletText("BROFILER");
		}
	}

	ImGui::ShowDemoWindow();

	ImGui::Begin("Information", NULL, 0);

	if(ImGui::BeginMenuBar())
		{
			ImGui::MenuItem("Item1");
			ImGui::MenuItem("Item2");
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	bool ret = true;

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return ret;
}

ModuleEditor::ModuleEditor()
{
}


ModuleEditor::~ModuleEditor()
{
}

