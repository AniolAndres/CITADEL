#include "Application.h"
#include "ModuleEditor.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
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
			if (ImGui::MenuItem("Editor"))
			{
				if (showEditorWindow)
					showEditorWindow = false;
				else
					showEditorWindow = true;
			}
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

	if (show_info_window)
	{
		ImGui::Begin("Information");
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
		ImGui::End();
	}

	//ImGui::ShowDemoWindow();


	//Editor tools Window

	float editorWidth, editorHeight;

	editorWidth = 400;
	editorHeight = App->window->windowHeight - 20;

	ImGui::SetNextWindowSize({ editorWidth, editorHeight });
	ImGui::SetNextWindowPos({ App->window->windowWidth - editorWidth, 18});

	if (showEditorWindow)
	{
		if (!ImGui::Begin("Editor tools", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
		}
		else
		{
			if (ImGui::CollapsingHeader("Module Camera"))
			{
				ImGui::Text("Current camera position:");
				ImGui::BulletText("(%f,%f,%f)", App->camera->eye.x, App->camera->eye.y, App->camera->eye.z);
				ImGui::Text("Current target position:");
				ImGui::BulletText("(%f,%f,%f)", App->camera->target.x, App->camera->target.y, App->camera->target.z);
			}
			if (ImGui::CollapsingHeader("Module Editor"))
			{
				//Nothing to show yet
			}
			if (ImGui::CollapsingHeader("Module ModelLoader"))
			{

			}
			if (ImGui::CollapsingHeader("Module Program"))
			{
				//Nothing to show yet
			}
			if (ImGui::CollapsingHeader("Module Render"))
			{

			}
			if (ImGui::CollapsingHeader("Module Textures"))
			{
				//Nothing to show yet
			}
			if (ImGui::CollapsingHeader("Module Input"))
			{
				ImGui::Text("Current mouse position:");
				ImGui::BulletText(" %f , %f ", App->input->GetMousePosition().x, App->input->GetMousePosition().y);
			}
			if (ImGui::CollapsingHeader("Module Window"))
			{
				ImGui::Text("Current window size: ");
				ImGui::BulletText(" %f x %f ", App->window->windowWidth, App->window->windowHeight); //How can I get rid of the decimals?
			}

			ImGui::End();
		}
	}
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

