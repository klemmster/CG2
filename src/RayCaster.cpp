#include "RayCaster.hpp"

#define RAY_STEPSIZE 0.02f
#define RAY_LENGTH 10.0f

#define GET_COLOR_INDEX (y*m_width*3+x*3)

void RayCaster::setColor(int x,int y, float r,float g,float b) {
	m_colors[GET_COLOR_INDEX+2] = r;
	m_colors[GET_COLOR_INDEX+1] = g;
	m_colors[GET_COLOR_INDEX] = b;
} 

void RayCaster::cast(Grid3D grid,float eyeX,float eyeY,float eyeZ) {
	double modelView[16], projection[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
    glGetDoublev(GL_PROJECTION_MATRIX, projection); 

	GLdouble x,y,z;	

	int rayStepCount = (int)(RAY_LENGTH/RAY_STEPSIZE+0.5f);

	vec3f rayDir;

	for(int screen_x=0;screen_x<m_width;screen_x++) {
		for(int screen_y=0;screen_y<m_height;screen_y++) {
			gluUnProject(
				screen_x, screen_y, 0, 
				modelView, projection, m_viewport, 
				&x, &y, &z
			);
			//cout << x << " " << y << " " << z << "\n";
			float dirX = x - eyeX;
			float dirY = y - eyeY;
			float dirZ = z - eyeZ;
			float dist = sqrt(dirX*dirX + dirY*dirY + dirZ*dirZ);
			rayDir = vec3f(
						dirX/dist * RAY_STEPSIZE,
						dirY/dist * RAY_STEPSIZE,
						dirZ/dist * RAY_STEPSIZE
					);

			float rayX = eyeX;
			float rayY = eyeY;
			float rayZ = eyeZ;
			float colorVal = 0;
			float minFuncVal = 100000;
			for(int m=0;m<rayStepCount;m++) {
				float funcVal = grid.getImplicitFunctionValue(rayX,rayY,rayZ);
				if(funcVal < minFuncVal)
					minFuncVal = funcVal;
				if(funcVal < 0)
					colorVal += 0.02f;
				rayX += rayDir[0];
				rayY += rayDir[1];
				rayZ += rayDir[2];
			}
		
			if(minFuncVal < 0) {
				colorVal = -minFuncVal+0.2f;
				setColor(screen_x,screen_y,colorVal,colorVal,colorVal);
			}else{
				colorVal = pow(4,-minFuncVal);
				setColor(screen_x,screen_y,colorVal,colorVal*0.8f,colorVal*0.2f);
			}
			
		}
	}

	glDrawPixels(m_width,m_height,GL_RGB,GL_FLOAT,m_colors);
	
}


void RayCaster::refreshViewport() {
	glGetIntegerv( GL_VIEWPORT, m_viewport); 
	if(m_width>0)
		free(m_colors);
	glGetIntegerv(GL_VIEWPORT, m_viewport); 
	m_width = m_viewport[2];
	m_height = m_viewport[3];
	m_colors = new float[m_width*m_height * 3 * sizeof(float)];
}
