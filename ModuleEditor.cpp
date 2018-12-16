#include "Application.h"
#include "ModuleEditor.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleModelLoader.h"
#include "ModuleCamera.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "SDL.h"
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
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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
		if (ImGui::BeginMenu("3D objects"))
		{
			if (ImGui::MenuItem("New GO"))
			{
				if (App->scene->SelectedGO != nullptr)
				{
					App->scene->CreateGameObject("GameObject", true, App->scene->SelectedGO);
				}
				else
				{
					App->scene->CreateGameObject("GameObject", true);
				}
				++App->scene->GOcounter;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Editor"))
			{
				if (showEditorWindow)
					showEditorWindow = false;
				else
					showEditorWindow = true;
			}
			if (ImGui::MenuItem("Console"))
			{
				if (showConsoleWindow)
					showConsoleWindow = false;
				else
					showConsoleWindow = true;
			}
			if (ImGui::MenuItem("Inspector"))
			{
				if (showInspectorWindow)
					showInspectorWindow = false;
				else
					showInspectorWindow = true;
			}
			if (ImGui::MenuItem("Draw window"))
			{
				if (showDrawWindow)
					showDrawWindow = false;
				else
					showDrawWindow = true;
			}
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
				ShellExecuteA(NULL, "open", "https://github.com/AniolAndres/CITADEL", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	if (showInfoWindow)
	{
		ImGui::Begin("Information", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

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

	//order matters!!

	ImGui::ShowDemoWindow();

	DrawEditor();

	DrawConsole();

	DrawInspector();

	DrawWindow();

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

void ModuleEditor::DrawConsole()
{
	if (showConsoleWindow)
	{
		consoleHeight = 150;
		consoleWidth = App->window->windowWidth - editorWidth;

		ImGui::SetNextWindowSize({ consoleWidth, consoleHeight });
		ImGui::SetNextWindowPos({ 0 , App->window->windowHeight - consoleHeight });

		consoleApp.Draw("Console");
	}
	else
	{
		consoleHeight = 0;
		consoleWidth = 0;
	}
}

void ModuleEditor::DrawInspector()
{
	if (showInspectorWindow)
	{

		inspectorWidth = 300;
		inspectorHeight = App->window->windowHeight - consoleHeight;

		ImGui::SetNextWindowSize({ inspectorWidth, inspectorHeight - 18 });
		ImGui::SetNextWindowPos({ 0 , 18 });

		if (!ImGui::Begin("Inspector.", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
		}
		else
		{
		App->scene->Root->DrawHierarchy();
		ImGui::End();
		}

	}
	else
	{
		inspectorWidth = 0;
		inspectorHeight = 0;
	}
}

void ModuleEditor::DrawEditor()
{
	if (showEditorWindow)
	{
		editorWidth = 400;
		editorHeight = App->window->windowHeight - 20;

		ImGui::SetNextWindowSize({ editorWidth, editorHeight });
		ImGui::SetNextWindowPos({ App->window->windowWidth - editorWidth, 18 });
		if (!ImGui::Begin("Editor tools", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
		}
		else {
			ImGui::BeginTabBar("Tools");
			if (ImGui::BeginTabItem("Modules"))
			{
				if (ImGui::CollapsingHeader("Module Camera")) //It's hard to track rotations because everything turn according to vectors and not a Quaternion
				{
					ImGui::Text("Current camera position:");
					ImGui::BulletText("(%f,%f,%f)", App->camera->eye.x, App->camera->eye.y, App->camera->eye.z);
					ImGui::Text("Current target position:");
					ImGui::BulletText("(%f,%f,%f)", App->camera->target.x, App->camera->target.y, App->camera->target.z);
					ImGui::Text("Front vector:");
					ImGui::BulletText("(%f,%f,%f)", App->camera->front.x, App->camera->front.y, App->camera->front.z);
					ImGui::Text("Side vector:");
					ImGui::BulletText("(%f,%f,%f)", App->camera->side.x, App->camera->side.y, App->camera->side.z);
					ImGui::Text("Upwards vector:");
					ImGui::BulletText("(%f,%f,%f)", App->camera->upwards.x, App->camera->upwards.y, App->camera->upwards.z);
				}
				if (ImGui::CollapsingHeader("Module Editor"))
				{
					ImGui::Checkbox("Information", &showInfoWindow);
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
					ImGui::Checkbox("Textures", &App->renderer->showTextures);
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
				if (ImGui::CollapsingHeader("Properties: "))
				{
					if (App->modelLoader->modelLoaded)
					{
						if (ImGui::TreeNode("Transformation"))
						{
							ImGui::Text("Current camera position:");
							ImGui::BulletText("(%f,%f,%f)", App->camera->eye.x, App->camera->eye.y, App->camera->eye.z);
							ImGui::Text("Current target position:");
							ImGui::BulletText("(%f,%f,%f)", App->camera->target.x, App->camera->target.y, App->camera->target.z);
							ImGui::Text("Front vector:");
							ImGui::BulletText("(%f,%f,%f)", App->camera->front.x, App->camera->front.y, App->camera->front.z);
							ImGui::Text("Side vector:");
							ImGui::BulletText("(%f,%f,%f)", App->camera->side.x, App->camera->side.y, App->camera->side.z);
							ImGui::Text("Upwards vector:");
							ImGui::BulletText("(%f,%f,%f)", App->camera->upwards.x, App->camera->upwards.y, App->camera->upwards.z);
							ImGui::TreePop();
							ImGui::Separator();
						}
						if (ImGui::TreeNode("Geometry"))
						{
							ImGui::Text("Scale: "); //pending
							ImGui::TreePop();
							ImGui::Separator();
						}
						if (ImGui::TreeNode("Texture"))
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
				if (ImGui::CollapsingHeader("Configuration"))
				{

					ImGui::Text("Application Time = %d", SDL_GetTicks() / 1000);
					ImGui::Checkbox("Stop", &stopFPS);
					ImGui::Text("Current FPS = %f ", currentFPS);
					ImGui::PlotHistogram("FPS", fpsLog, 50, 0, "FPS graphic", 0.0f, 100.0f, ImVec2(350, 100));
					ImGui::Text("Current MS = %f ", currentMs);
					ImGui::PlotHistogram("MS", msLog, 50, 0, "MS graphic", 0.0f, 25.0f / 1000.0f, ImVec2(350, 100));
					ImGui::Text("Graphics card vendor: %s \n", glGetString(GL_VENDOR));
					ImGui::Text("Graphics card used: %s \n", glGetString(GL_RENDERER));
					ImGui::NewLine();
					ImGui::Text("OpenGL version: %s", glGetString(GL_VERSION));
					SDL_GetVersion(&App->window->version);
					ImGui::Text("SDL version: %d.%d.%d \n", App->window->version.major, App->window->version.minor, App->window->version.patch);
					ImGui::Text("ImGui version: %s \n", ImGui::GetVersion());
				}

				ImGui::EndTabItem();

			}
			if (ImGui::BeginTabItem("Editor"))
			{
				if (App->scene->SelectedGO == nullptr)
				{
					ImGui::Text("No object selected");
				}
				else
				{
					ImGui::Text("Selected %s #%d", App->scene->SelectedGO->name, App->scene->SelectedGO->id);
					//This will be used to modify the components of the selected GameObject
					if (ImGui::CollapsingHeader("Component Mesh"))
					{
						App->scene->SelectedGO->DrawComponents(MESH);
						if (ImGui::Button("New Component Mesh"))
						{
							App->scene->SelectedGO->CreateComponent(MESH);
						}
					}
					if (ImGui::CollapsingHeader("Component Material"))
					{
						App->scene->SelectedGO->DrawComponents(MATERIAL);
						if (ImGui::Button("New Component Material"))
						{
							App->scene->SelectedGO->CreateComponent(MATERIAL);
						}
					}
					if (ImGui::CollapsingHeader("Component Transform"))
					{
						App->scene->SelectedGO->DrawComponents(TRANSFORM);
						if (ImGui::Button("New Component Transform"))
						{
							App->scene->SelectedGO->CreateComponent(TRANSFORM);
						}
					}
					if (ImGui::CollapsingHeader("Component Light"))
					{
						App->scene->SelectedGO->DrawComponents(LIGHT);
						if (ImGui::Button("New Component Light"))
						{
							App->scene->SelectedGO->CreateComponent(LIGHT);
						}
					}
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();		
	}
	else
	{
		editorWidth = 0;
		editorHeight = 0;
	}
}

void ModuleEditor::DrawWindow()
{
	if (showDrawWindow)
	{
		drawWidth = App->window->windowWidth - inspectorWidth - editorWidth;
		drawHeight = App->window->windowHeight - 18 - consoleHeight;

		ImGui::SetNextWindowSize({ drawWidth, drawHeight });
		ImGui::SetNextWindowPos({ inspectorWidth , 18 });

		if (!ImGui::Begin("Scene", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
		}
		else
		{
			if (ImGui::BeginChild("Editor Canvas", ImVec2(0, 0), true, ImGuiWindowFlags_NoMove))
			{

				ImGui::GetWindowDrawList()->AddImage(
					(void*)App->camera->fbo.fb_tex,
					ImVec2(ImGui::GetCursorScreenPos()),
					ImVec2(ImGui::GetCursorScreenPos().x + App->camera->fbo.fb_width,
						ImGui::GetCursorScreenPos().y + App->camera->fbo.fb_height),
					ImVec2(0, 1), ImVec2(1, 0));
			}
			ImGui::EndChild();
			ImGui::End();
		}
	}
}
