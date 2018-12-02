#ifndef __ModuleEditor_H_
#define  __ModuleEditor_H_

#include "GL/glew.h"
#include "Module.h"

class ModuleEditor :
	public Module
{
public:

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	ModuleEditor();
	~ModuleEditor();

private:
	bool show_console_window = false;
	bool show_info_window = false;
	bool showEditorWindow = true;
};

#endif // __ModuleEditor_H_