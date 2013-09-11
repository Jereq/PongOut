#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Graphics
{
private:
	GLFWwindow* window;

	static void errorCallback(int error, const char* description);

	bool openWindow();
	void initDevIL();
	void printReport();

public:
	Graphics();
	virtual ~Graphics();

	bool init();
	void destroy();
	bool windowClosing();

	double getTime();
	void pollEvents();
	void swapBuffers();
};
