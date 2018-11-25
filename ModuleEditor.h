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
	bool show_demo_window = true;
	bool show_another_window = false;
};

