#include <GL/glew.h>

class Graphics
{
private:
	void printReport();

public:
	Graphics();
	virtual ~Graphics();

	bool init();
	void destroy();
};
