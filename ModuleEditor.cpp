#include "Application.h"
#include "ModuleEditor.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleModelLoader.h"
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

	//Variables for the FPS configuration

	currentFrame = SDL_GetTicks();
	lastFrame = SDL_GetTicks();
	currentFPS = 0;
	fpsIterator = 0;

	return ret;
}

update_status ModuleEditor::PreUpdate()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	if(stopFPS==false)
		updateFramerate();

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
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	if (show_info_window)
	{
		ImGui::Begin("Information", 0 , ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			ImGui::Text("Engine Name: Citadel");
			ImGui::Text("Author: Aniol Andres Guiu");
			ImGui::Text("Libraries used");
			ImGui::BulletText("SDL2");
			ImGui::BulletText("GLEW");
			ImGui::BulletText("IMGUI");
			ImGui::BulletText("ASSIMP");
			ImGui::BulletText("MATHGEOLIB");
			ImGui::BulletText("DEVIL");
			ImGui::BulletText("BROFILER");

		ImGui::End();
	}

	//ImGui::ShowDemoWindow();

	//Console window


	//Editor tools Window

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
				ImGui::Text("Front vector:");
				ImGui::BulletText("(%f,%f,%f)", App->camera->front.x, App->camera->front.y, App->camera->front.z);
			}
			if (ImGui::CollapsingHeader("Module Editor"))
			{
				ImGui::Checkbox("Information", &show_info_window);
			}
			if (ImGui::CollapsingHeader("Module ModelLoader"))
			{
				if (App->modelLoader->modelLoaded)
				{
					if (ImGui::TreeNode("Scene:"))
					{
						ImGui::Text("Number of meshes: %i", App->modelLoader->scene->mNumMeshes);
						ImGui::Text("Number of vertices: %i", App->modelLoader->numVerticesTotal);
						ImGui::TreePop();
						ImGui::Separator();
					}
				}
				else
				{
					ImGui::Text("No scene loaded atm");
				}
			}
			if (ImGui::CollapsingHeader("Module Program"))
			{
				//Nothing to show yet
			}
			if (ImGui::CollapsingHeader("Module Render"))
			{
				ImGui::Checkbox("Grid", &App->renderer->showGrid);
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
			if (ImGui::CollapsingHeader("Configuration"))
			{
				
				ImGui::Text("Application Time = %d", SDL_GetTicks() / 1000);
				ImGui::Checkbox("Stop", &stopFPS);
				ImGui::Text("Current FPS = %f ", currentFPS);
				ImGui::PlotHistogram("FPS", fpsLog, 50, 0, "FPS graphic", 0.0f, 100.0f, ImVec2(350, 100));
				ImGui::Text("Current MS = %f ", currentMs);
				ImGui::PlotHistogram("MS", msLog, 50, 0, "MS graphic", 0.0f, 25.0f/1000.0f, ImVec2(350, 100));
				ImGui::Text("Graphics card vendor: %s \n", glGetString(GL_VENDOR));
				ImGui::Text("Graphics card used: %s \n", glGetString(GL_RENDERER));
				ImGui::NewLine();
				ImGui::Text("OpenGL version: %s", glGetString(GL_VERSION));
				SDL_GetVersion(&App->window->version);
				ImGui::Text("SDL version: %d.%d.%d \n", App->window->version.major, App->window->version.minor, App->window->version.patch);
				ImGui::Text("ImGui version: %s \n", ImGui::GetVersion());
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

void ModuleEditor::updateFramerate()
{
	currentFrame = SDL_GetTicks();
	currentFPS = 1000 / (currentFrame - lastFrame);
	currentMs = 1 / currentFPS;
	lastFrame = currentFrame;

	if (fpsIterator < 49)
	{
		fpsLog[fpsIterator] = currentFPS;
		msLog[fpsIterator] = currentMs;
		++fpsIterator;
	}
	else
		fpsIterator = 0;

	

}

ModuleEditor::ModuleEditor()
{
}


ModuleEditor::~ModuleEditor()
{
}

