#include "vec.hpp"
#include "normal.hpp"
#include "vertex.hpp"
#include "tree.hpp"
#include "Grid3D.hpp"

#ifdef __APPLE__
  #include <GL/glew.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/glut.h>
#endif

class RayCaster {

public:
	RayCaster() {
		initialized = false;
	};

	~RayCaster() {
		
	}

	vec3f m_rays[];
	float * m_colors;

	void refreshViewport();
	void cast(Grid3D grid,float eyeX,float eyeY,float eyeZ);

private:
	bool initialized;
	int m_width;
	int m_height;
	GLint m_viewport[4];
	void setColor(int x,int y, float r,float g,float b);

};
