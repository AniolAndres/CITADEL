#ifndef __ModuleEditor_H_
#define  __ModuleEditor_H_

#include "GL/glew.h"
#include "Module.h"
#include "Math/float2.h"
#include "IMGUI/imgui.h"
#include "SDL.h"

class GameObject;

enum PUcase
{
	HIERARCHY=1,
	DRAW,
	EDITOR,
	MODULES,
	CONSOLE
};

class ModuleEditor :
	public Module
{
public:

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void updateFramerate();


	struct ExampleAppLog
	{
		ImGuiTextBuffer     Buf;
		bool                ScrollToBottom;

		void    Clear() { Buf.clear(); }

		void    AddLog(const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			Buf.appendf(fmt, args);
			va_end(args);
			ScrollToBottom = true;
		}

		void    Draw(const char* title, bool* p_opened = NULL)
		{
			ImGui::Begin(title, p_opened);
			ImGui::TextUnformatted(Buf.begin());
			if (ScrollToBottom)
				ImGui::SetScrollHere(1.0f);
			ScrollToBottom = false;
			ImGui::End();
		}
	};

	ExampleAppLog consoleApp;

	float drawWidth, drawHeight;
	PUcase PU;

	void DrawEditor();
	void DrawInspector();
	void DrawConsole();
	void DrawWindow();
	void DrawHierarchyPopup();
	void DrawEditorPopup();
	void DrawDrawPopup();
	void DrawConsolePopup();

	ModuleEditor();
	~ModuleEditor();

	void ProcessInputEvent(SDL_Event* event) const;

private:
	bool showConsoleWindow = true;
	bool showInfoWindow = false;
	bool showEditorWindow = true;
	bool showInspectorWindow = true;
	bool showDrawWindow = true;
	bool stopFPS = false;
	bool isModelCopied = false;
	
	float editorWidth, editorHeight;
	float consoleWidth, consoleHeight;
	float inspectorWidth, inspectorHeight;
	float currentFrame, lastFrame , currentFPS, currentMs;
	float fpsLog[50], msLog[50];
	int fpsIterator;
};

#endif // __ModuleEditor_H_