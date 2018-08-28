#include "mesh.h"
#include "glew.h"
//#include "glaux.h"
#include "glut.h"
#include "FreeImage.h"
#include "ShaderLoader.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4996 )
using namespace std;
#define TEXTURE_NUM 100
unsigned int texObject[TEXTURE_NUM];
int program;
int shaderr;
int select=0;

void loadshader()
{
	shaderr = glCreateProgram();
	if (shaderr != 0)
	{
		ShaderLoad(shaderr, "MeshExample.vert", GL_VERTEX_SHADER);
		ShaderLoad(shaderr, "MeshExample.geom", GL_GEOMETRY_SHADER);
		ShaderLoad(shaderr, "MeshExample.frag", GL_FRAGMENT_SHADER);
		printf("Shader Loaded Complete.\n");
	}
}

class sc//not yet done
{
public:
	sc::sc()
	{

	}
	mesh *object;
	int get_texture_type()
	{
		return texture_type;
	}
	void set_texture_type(int i)
	{
		texture_type = i;
	}
	string get_obj_file_name()
	{
		return obj_file_name;
	}
	double get_num(int j)
	{
		return num[j];
	}
	void set_num(int i, double j)
	{
		num[i] = j;
	}
	int get_object_num()
	{
		return object_num;
	}
	void set_object_num(int i)
	{
		object_num = i;
	}
	void set_object_name(string a)
	{
		obj_file_name = a;
	}
	void change_num_tx(int j)
	{
		num[7] = j;
	}
	void change_num_ty(int j)
	{
		num[8] = j;
	}
	void set_start_x_y(int i, int j)
	{
		start_x = i;
		start_y = j;
	}
	int get_start_x()
	{
		return start_x;
	}
	int get_start_y()
	{
		return start_y;
	}
	void set_key_num(int k)
	{
		key_num = k;
	}
	int get_key_num()
	{
		return key_num;
	}
	void move(int a, int b, int w, int h)
	{
		if (key_num >= 0)
		{
			num[7] += (a - start_x) / (double)13;
			num[8] -= (b - start_y) / (double)13;
		}

	}
private:
	string obj_file_name;//obj_file_name
	double num[10];//�U��scene�ƭ�
	int object_num;//�ĴX��object  �q0�}�l
	int key_num;
	int start_x;
	int start_y;
	int texture_type;
};

/*struct sc
{
string obj_file_name;
double num[10];
mesh* object;
int object_num;
int texture_type;
};*/
sc s;
vector<sc> sc_vec;
struct type
{
	int texture_t;
	string texture_name[6];
};
type tt;
vector<type> tt_vec;
class vi
{
public:
	vi::vi()
	{
		fstream view_file;//Ūview_file
						  //char view_name[] = "view.view";
		char view_name[] = "as3.view";
		view_file.open(view_name, ios::in);
		char temp[100];
		string view[23];
		float view_num[23];
		if (!view_file) {//�p�G�}���ɮץ��ѡAfp��0�F���\�Afp���D0
			cout << "Fail to open file: " << view_name << endl;
		}
		for (int i = 0; i < 23; i++)
		{
			view_file >> temp;
			if (i == 0 || i == 4 || i == 8 || i == 12 || i == 14 || i == 16 || i == 18)
			{
				view[i] = temp;
			}
			else
			{
				view[i] = temp;
				view_num[i] = atof(temp);
			}
		}
		eye[0] = view_num[1];
		eye[1] = view_num[2];
		eye[2] = view_num[3];
		vat[0] = view_num[5];
		vat[1] = view_num[6];
		vat[2] = view_num[7];
		vup[0] = view_num[9];
		vup[1] = view_num[10];
		vup[2] = view_num[11];
		fovy = view_num[13];
		dnear = view_num[15];
		dfar = view_num[17];
		viewpoint[0] = view_num[19];
		viewpoint[1] = view_num[20];
		viewpoint[2] = view_num[21];
		viewpoint[3] = view_num[22];
		view_file.close();//Ū��view_file
	}
	void change_fovy(int i)
	{
		fovy = i;
	}
	void change_width(GLsizei w)
	{
		viewpoint[2] = w;
	}
	void change_height(GLsizei h)
	{
		viewpoint[3] = h;
	}
	int get_viewpoint(int i)
	{
		return viewpoint[i];
	}
	double get_eye(int i)
	{
		return eye[i];
	}
	int get_vat(int i)
	{
		return vat[i];
	}
	int get_vup(int i)
	{
		return vup[i];
	}
	int get_fovy()
	{
		return fovy;
	}
	int get_dnear()
	{
		return dnear;
	}
	int get_dfar()
	{
		return dfar;
	}
	void change_eye(double x, double y, double z)
	{
		eye[0] = x;
		eye[1] = y;
		eye[2] = z;
	}
	void change_vat(int x, int y, int z)
	{
		vat[0] = x;
		vat[1] = y;
		vat[2] = z;
	}
private:
	int viewpoint[4];
	double eye[3];
	double vat[3];
	int vup[3];
	int fovy;
	int dnear;
	int dfar;
};
vi v_i;
class li
{
public:
	li::li()
	{
		for (int i = 0; i < 100; i++)
		{
			for (int j = 0; j < 12; j++)
			{
				light[i][j] = 0;
			}
		}
		fstream light_file;//Ūlight_file
						   //char light_name[] = "light.light";
		char light_name[] = "as3.light";
		light_file.open(light_name, ios::in);
		char line[100];
		char* te;
		light_counter = 0;
		while (light_file.getline(line, sizeof(line), '\n'))
		{
			if (line[0] == 'l')
			{
				te = strtok(line, " ");
				te = strtok(NULL, " ");
				int j = 0;
				while (te != NULL)
				{
					light[light_counter][j] = atof(te);
					j++;
					te = strtok(NULL, " ");
				}
				light_counter++;
			}
			else if (line[0] == 'a')
			{
				te = strtok(line, " ");
				int i = 0;
				while (te != NULL)
				{

					te = strtok(NULL, " ");
					if (te != NULL)
						ambient[i] = atof(te);
					else
					{
						break;
					}
					i++;
				}
			}
		}
		light_file.close();//Ū��light_file
	}
	float set_light(int i, int j,int x)
	{
		light[i][j] =  light[i][j] + x;
		return light[i][j];
	}
	float get_light(int i, int j)
	{
		return light[i][j];
	}
	float* get_ambient()
	{
		return ambient;
	}
	void set_ambient(int i, float j)
	{
		ambient[i] = j;
	}
	int get_light_counter()
	{
		return light_counter;
	}
private:
	float light[100][12];
	float ambient[4];
	int light_counter;//light_counter�N�O�{�b���X���O
};
li l;
void Loadtexture(char* pFilename, int n)
{
	FIBITMAP* pImage = FreeImage_Load(FreeImage_GetFileType(pFilename, 0), pFilename);
	FIBITMAP* p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
	int iWidth = FreeImage_GetWidth(p32BitsImage);
	int	iHeight = FreeImage_GetHeight(p32BitsImage);

	glBindTexture(GL_TEXTURE_2D, texObject[n]);//2D,3D,cube_map
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iWidth, iHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));
	glGenerateMipmap(GL_TEXTURE_2D);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	FreeImage_Unload(p32BitsImage);
	FreeImage_Unload(pImage);
}
void Load_cube_texture(char *  pFilename, int  n, int  index)
{
	FIBITMAP* pImage = FreeImage_Load(FreeImage_GetFileType(pFilename, 0), pFilename);
	FIBITMAP* p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
	int iWidth = FreeImage_GetWidth(p32BitsImage);
	int	iHeight = FreeImage_GetHeight(p32BitsImage);

	if (index == 0)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, texObject[n]);//2D,3D,cube_map
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGBA8, iWidth, iHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));
	if (index == 5)glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	FreeImage_Unload(p32BitsImage);
	FreeImage_Unload(pImage);
}
void lighting()  //not sure maybe
{
	glShadeModel(GL_SMOOTH);

	// z buffer enable
	glEnable(GL_DEPTH_TEST);

	// enable lighting
	glEnable(GL_LIGHTING);

	for (int i = 0; i < l.get_light_counter(); i++)
	{
		GLfloat light_position[] = { l.get_light(i, 0), l.get_light(i, 1), l.get_light(i, 2), 1.0f };
		GLfloat light_ambient[] = { l.get_light(i, 3), l.get_light(i, 4), l.get_light(i, 5), 1.0f };
		GLfloat light_diffuse[] = { l.get_light(i, 6), l.get_light(i, 7), l.get_light(i, 8), 1.0f };
		GLfloat light_specular[] = { l.get_light(i, 9), l.get_light(i, 10), l.get_light(i, 11), 1.0f };
		// set light property

		glEnable(GL_LIGHT0 + i);
		glLightfv(GL_LIGHT0 + i, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0 + i, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0 + i, GL_AMBIENT, light_ambient);
	}
	//l.set_ambient(3, 1.0);   // ???
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, l.get_ambient());�@�@// ??
}
void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//clear the buffer
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);//ambient
	glAlphaFunc(GL_GREATER, 0.5f);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//viewing and modeling transformation
	glViewport(v_i.get_viewpoint(0), v_i.get_viewpoint(1), (GLsizei)v_i.get_viewpoint(2), (GLsizei)v_i.get_viewpoint(3));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(v_i.get_fovy(), (float)(v_i.get_viewpoint(2) / v_i.get_viewpoint(3)), v_i.get_dnear(), v_i.get_dfar());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(v_i.get_eye(0), v_i.get_eye(1), v_i.get_eye(2),//eye
		v_i.get_vat(0), v_i.get_vat(1), v_i.get_vat(2),//center
		v_i.get_vup(0), v_i.get_vup(1), v_i.get_vup(2));//up

	lighting();
	glUseProgram(shaderr);

	//glRotated(s.get_num(s.get_key_num(), 3), v_i.get_vup(0), v_i.get_vup(1), v_i.get_vup(2));
	
	GLint location;
	location = glGetUniformLocation(shaderr, "colorTexture");
	if (location == -1)
		printf("Cant find texture name: colorTexture\n");
	else
		glUniform1i(location, 0);
	
	location = glGetUniformLocation(shaderr, "level");
	if (location == -1)
		printf("Cant find  name: level\n");
	else
		glUniform1i(location, select);  //select ??
	location = glGetUniformLocation(shaderr, "Radius");
	if (location == -1)
		printf("Cant find name: Radius\n");
	else
		glUniform1f(location, 1.0);
	location = glGetUniformLocation(shaderr, "Centroid");
	if (location == -1)
		printf("Cant find name: Centroid\n");
	else {
		float arr[4] = { 0.0, 1.0, 0.0, 1.0 };
		glUniform4fv(location, 1, arr);
	}

	for (int j = 0; j < sc_vec.size(); j++)
	{
		glPushMatrix();
		glTranslated(sc_vec[j].get_num(7), sc_vec[j].get_num(8), sc_vec[j].get_num(9));
		glRotated(sc_vec[j].get_num(3), sc_vec[j].get_num(4), sc_vec[j].get_num(5), sc_vec[j].get_num(6));
		glScaled(sc_vec[j].get_num(0), sc_vec[j].get_num(1), sc_vec[j].get_num(2));
		//bind texture
		if (sc_vec[j].get_texture_type() == 1)
		{
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texObject[sc_vec[j].get_object_num()]);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}
		else if (sc_vec[j].get_texture_type() == 2)
		{
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texObject[sc_vec[j].get_object_num() - 1]);///MAYBE WRONG
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

			glActiveTexture(GL_TEXTURE1);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texObject[sc_vec[j].get_object_num()]);///MAYBE WRONG
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
		}
		else if (sc_vec[j].get_texture_type() == 6)
		{
			glActiveTexture(GL_TEXTURE0);
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);
			glEnable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texObject[sc_vec[j].get_object_num()]);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}

		int lastMaterial = -1;
		for (size_t i = 0; i < sc_vec[j].object->fTotal; ++i)
		{
			// set material property if this face used different material
			if (lastMaterial != sc_vec[j].object->faceList[i].m)
			{
				lastMaterial = (int)sc_vec[j].object->faceList[i].m;
				glMaterialfv(GL_FRONT, GL_AMBIENT, sc_vec[j].object->mList[lastMaterial].Ka);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, sc_vec[j].object->mList[lastMaterial].Kd);
				glMaterialfv(GL_FRONT, GL_SPECULAR, sc_vec[j].object->mList[lastMaterial].Ks);
				glMaterialfv(GL_FRONT, GL_SHININESS, &sc_vec[j].object->mList[lastMaterial].Ns);
				//you can obtain the texture name by object->mList[lastMaterial].map_Kd
				//load them once in the main function before mainloop
				//bind them in display function here
			}
			glBegin(GL_TRIANGLES);
			for (size_t tt = 0; tt<3; ++tt)
			{
				//textex corrd. s.get_object(j)->tList[s.get_object(j)->faceList[i][tt].t].ptr;
				//glTexCoord2fv(s.get_object(j)->tList[s.get_object(j)->faceList[i][tt].t].ptr);
				//textex corrd. object->tList[object->faceList[i][j].t].ptr
				if (sc_vec[j].get_texture_type() == 1) {
					glTexCoord2fv(sc_vec[j].object->tList[sc_vec[j].object->faceList[i][tt].t].ptr);
				}
				else if (sc_vec[j].get_texture_type() == 2) {
					glMultiTexCoord2fv(GL_TEXTURE0, sc_vec[j].object->tList[sc_vec[j].object->faceList[i][tt].t].ptr);
					glMultiTexCoord2fv(GL_TEXTURE1, sc_vec[j].object->tList[sc_vec[j].object->faceList[i][tt].t].ptr);
				}
				else if (sc_vec[j].get_texture_type() == 6) {
					glTexCoord2fv(sc_vec[j].object->tList[sc_vec[j].object->faceList[i][tt].t].ptr);
				}
				glNormal3fv(sc_vec[j].object->nList[sc_vec[j].object->faceList[i][tt].n].ptr);
				glVertex3fv(sc_vec[j].object->vList[sc_vec[j].object->faceList[i][tt].v].ptr);
			}
			glEnd();
		}
		if (sc_vec[j].get_texture_type() == 1) {
			glActiveTexture(GL_TEXTURE0);
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else if (sc_vec[j].get_texture_type() == 2) {
			glActiveTexture(GL_TEXTURE1);
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE0);
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else if (sc_vec[j].get_texture_type() == 6) {
			glActiveTexture(GL_TEXTURE0);
			glDisable(GL_TEXTURE_CUBE_MAP);
			glDisable(GL_TEXTURE_GEN_R);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_S);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		glPopMatrix();
	}
	glutSwapBuffers();
	glutPostRedisplay();
}
void reshape(GLsizei w, GLsizei h)
{
	v_i.change_width(w);
	v_i.change_height(h);
}
float normalize(float x, float y, float z)
{

	return sqrtf(pow(x, 2) + pow(y, 2) + pow(z, 2));
}
/*void mouse(int button, int state, int x, int y)
{
int ke = s.get_key_num();
s.set_start_x_y(s.get_num(ke, 7), s.get_num(ke, 8));
if (button == GLUT_LEFT_BUTTON)
{
if (state == GLUT_DOWN)
{
s.set_start_x_y(x, y);
}
}
else if (button == GLUT_RIGHT_BUTTON)
{
if (state == GLUT_DOWN)
{
s.set_start_x_y(x, y);
}
}
}*/
/*void mouse_motion(int x, int y)
{
int ke = s.get_key_num();
s.move(x, y, v_i.get_viewpoint(2), v_i.get_viewpoint(3));
s.set_start_x_y(x, y);
//glutPostRedisplay();//display did
}*/
#define pi 3.1415926
void keyboard(unsigned char key, int x, int y)
{
	double theta = 30 * pi / 180;
	double x_i, y_i, z_i;
	switch (key)
	{
	case 'w'://zoom in
		x_i = v_i.get_vat(0) - v_i.get_eye(0);
		y_i = v_i.get_vat(1) - v_i.get_eye(1);
		z_i = v_i.get_vat(2) - v_i.get_eye(2);
		v_i.change_eye(v_i.get_eye(0) + x_i / 4, v_i.get_eye(1) + y_i / 4, v_i.get_eye(2) + z_i / 4);
		break;
	case 'a'://move left
		x_i = (v_i.get_eye(0)-v_i.get_vat(0))*cos(theta)  - ((v_i.get_eye(2)- v_i.get_vat(2))*sin(theta)) + v_i.get_vat(0);
		y_i = v_i.get_eye(1);
		z_i = ((v_i.get_eye(0)- v_i.get_vat(0))*sin(theta)) + (((v_i.get_eye(2)-v_i.get_vat(2))*cos(theta))) + v_i.get_vat(2);
		v_i.change_eye(x_i, y_i, z_i);
		break;
	case 's'://zoom out
		x_i = v_i.get_vat(0) - v_i.get_eye(0);
		y_i = v_i.get_vat(1) - v_i.get_eye(1);
		z_i = v_i.get_vat(2) - v_i.get_eye(2);
		v_i.change_eye(v_i.get_eye(0) - x_i / 4, v_i.get_eye(1) - y_i / 4, v_i.get_eye(2) - z_i / 4);
		break;
	case 'd'://move right
		x_i = ((v_i.get_eye(0)- v_i.get_vat(0))*cos((-1)*theta)) - (v_i.get_eye(2)- v_i.get_vat(2))*sin((-1)*theta) + v_i.get_vat(0);
		y_i = v_i.get_eye(1);
		z_i = (v_i.get_eye(0)- v_i.get_vat(0))*sin((-1)*theta) + (v_i.get_eye(2)- v_i.get_vat(2))*cos((-1)*theta)+ v_i.get_vat(2);
		v_i.change_eye(x_i, y_i, z_i);
		break;
	case '1':
		select = 0;
		break;
	case '2':
		select = 1;
		break;
	case '3':
		select = 2 ;
		break;
	case 'f':
		//l.get_light(0, 1) -= 1;
		l.set_light(0,1,-1);
		break;
	case 'r':
		//l.get_light(0, 1) += 1;
		l.set_light(0,1,1);
		break;
		
	
	default:
		break;
	}
	glutPostRedisplay();
}
int main(int argc, char** argv)
{
	fstream scene_file;//Ūscene_file
	char scene_name[] = "as3.scene";
	scene_file.open(scene_name);
	char scene[1000];
	if (!scene_file) {//�p�G�}���ɮץ��ѡAfp��0�F���\�Afp���D0
		cout << "Fail to open file: " << scene_name << endl;
	}
	string s_str;
	int num = -1;
	int type;
	while (scene_file >> s_str)
	{

		if (s_str == "no-texture")
		{
			tt.texture_t = 0;
			for (int i = 0; i < 6; i++)
			{
				tt.texture_name[i] = "";
			}
			tt_vec.push_back(tt);
			type = 0;
		}
		else if (s_str == "single-texture")
		{
			tt.texture_t = 1;
			for (int i = 0; i < 1; i++)
			{
				scene_file >> s_str;
				tt.texture_name[i] = s_str;
			}
			for (int i = 1; i < 6; i++)
			{
				tt.texture_name[i] = "";
			}
			tt_vec.push_back(tt);
			num++;
			type = 1;
		}
		else if (s_str == "multi-texture")
		{
			tt.texture_t = 2;
			for (int i = 0; i < 2; i++)
			{
				scene_file >> s_str;
				tt.texture_name[i] = s_str;
			}
			for (int i = 2; i < 6; i++)
			{
				tt.texture_name[i] = "";
			}
			tt_vec.push_back(tt);
			num += 2;
			type = 2;
		}
		else if (s_str == "cube-map")
		{
			tt.texture_t = 6;
			for (int i = 0; i < 6; i++)
			{
				scene_file >> s_str;
				tt.texture_name[i] = s_str;
			}
			tt_vec.push_back(tt);
			num++;
			type = 6;
		}
		else if (s_str == "model")
		{
			scene_file >> s_str;
			s.set_object_name(s_str);
			for (int i = 0; i < 10; i++)
			{
				double n;
				scene_file >> n;
				s.set_num(i, n);
			}
			s.set_texture_type(type);
			s.object = new mesh(s.get_obj_file_name().c_str());
			s.set_object_num(num);
			sc_vec.push_back(s);
		}
	}
	scene_file.close();
	glutInit(&argc, argv);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(0, 0);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("Subdivision and Phong Shading");
	glewInit();
	FreeImage_Initialise();
	int n = 0;
	glGenTextures(TEXTURE_NUM, texObject);
	for (int i = 0; i < tt_vec.size(); i++)
	{
		if (tt_vec[i].texture_t == 1)
		{
			Loadtexture((char *)tt_vec[i].texture_name[0].c_str(), n);
			n++;
		}
		else if (tt_vec[i].texture_t == 2)
		{
			Loadtexture((char *)tt_vec[i].texture_name[0].c_str(), n);
			n++;
			Loadtexture((char *)tt_vec[i].texture_name[1].c_str(), n);
			n++;
		}
		else if (tt_vec[i].texture_t == 6)
		{
			for (int j = 0; j < 6; j++)
			{
				Load_cube_texture((char *)tt_vec[i].texture_name[j].c_str(), n, j);
			}
			n++;
		}
	}
	FreeImage_DeInitialise();
	
	loadshader();
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	//glutMouseFunc(mouse);
	//glutMotionFunc(mouse_motion);
	glutMainLoop();
	return 0;
	system("pause");
}