#include "RayCaster.hpp"

#define RAY_STEPSIZE 0.0005f
#define RAY_LENGTH 10.0f

#define GET_COLOR_INDEX (y*m_width*3+x*3)

void RayCaster::setColor(int x,int y, float r,float g,float b) {
	m_colors[GET_COLOR_INDEX+2] = r;
	m_colors[GET_COLOR_INDEX+1] = g;
	m_colors[GET_COLOR_INDEX] = b;
}

void RayCaster::cast(Grid3D * grid,int type,float eyeX,float eyeY,float eyeZ,float scale) {
	double modelView[16], projection[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

	GLdouble x,y,z;

	float dScale = 1/scale;

	int rayStepCount = (int)(RAY_LENGTH/RAY_STEPSIZE+0.5f);

	vec3f rayDir;
	float stepSize = RAY_STEPSIZE;

	//Filltest funcvalues
if(0)
	for(int i=0;i<=grid->getDimX();i++)
		for(int j=0;j<=grid->getDimY();j++)
			for(int k=0;k<=grid->getDimZ();k++) {
				float x = i/((float)grid->getDimX()) - 0.5f;
				float y = j/((float)grid->getDimY()) - 0.5f;
				float z = k/((float)grid->getDimZ()) - 0.5f;
				grid->getVertex(i,j,k)->setFunValue(x*x*4 + y*y*4 + z*z*4 - 1);
			}


	float maxColorVal = 0;

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
						dirX/dist,
						dirY/dist,
						dirZ/dist
					);

			float rayX = eyeX;
			float rayY = eyeY;
			float rayZ = eyeZ;
			float colorVal = 0;
			float minFuncVal = 100000;

			setColor(screen_x,screen_y,0,0,0.1f);

			for(int m=0;m<rayStepCount;m++) {
				float funcVal = grid->getImplicitFunctionValueWorldCoordinates(
									rayX*dScale,rayY*dScale,rayZ*dScale,
									0
								);
				
				float normVal = grid->getImplicitFunctionValueWorldCoordinates(
									rayX*dScale,rayY*dScale,rayZ*dScale,
									1
								);

				if(funcVal < minFuncVal)
					minFuncVal = funcVal;
				if(funcVal <= 0) {
					if(type == RC_FIRSTTOUCH)
					{
						colorVal = 1;
						setColor(screen_x,screen_y,colorVal,colorVal,colorVal);
						break;
					}else{
						colorVal += 1;
						if(colorVal>maxColorVal) {
							maxColorVal = colorVal;
						}
					}
				}else{
					if(type == RC_FIRSTTOUCH) {
						if(funcVal<OUTOFRANGE_DISTANCE && false)
							stepSize = funcVal;
						else
							stepSize = RAY_STEPSIZE;
					}
				}
				rayX += rayDir[0] * stepSize;
				rayY += rayDir[1] * stepSize;
				rayZ += rayDir[2] * stepSize;
			}

			if(type!=RC_FIRSTTOUCH) {
				if(minFuncVal <= 0) {
					//colorVal = -minFuncVal+0.9f;
					if(colorVal<0)
						colorVal = 0;
					setColor(screen_x,screen_y,colorVal,colorVal,colorVal);
				}else if(minFuncVal < OUTOFRANGE_DISTANCE){
					//colorVal = pow(4,-minFuncVal);
					colorVal = 0;
					setColor(screen_x,screen_y,colorVal,colorVal*0.8f,colorVal*0.2f);
				}
			}

		}
	}

	//Normalize
	if(type==RC_TRANSPARENT && maxColorVal>0)
		for(int i=0;i<m_width*m_height*3*sizeof(float);i++)
			m_colors[i]/=maxColorVal;

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
