#include <glew\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


#include <glut37\glut.h>


GLuint program;
GLuint vao;

void startup()
{
	static const char* vs_source[] =
	{
		"#version 410 core                                                 \n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
		"                                   vec4(-0.25, -0.25, 0.5, 1.0),  \n"
		"                                   vec4( 0.25,  0.25, 0.5, 1.0)); \n"
		"                                                                  \n"
		"    gl_Position = vertices[gl_VertexID];                          \n"	// ���ݵ�ǰ����Ķ��� ID Ϊ����λ�ø�ֵ
		"}                                                                 \n"
	};

	// http://www.cnblogs.com/zenny-chen/p/4280100.html
	static const char* tcs_source[] =
	{
		"#version 410 core                                                                 \n"
		"                                                                                  \n"
		"layout (vertices = 3) out;                                                        \n"	// out-patch �Ķ��������ϸ�ֿ�����ɫ�����ᱻִ��3��
		"                                                                                  \n"
		"void main(void)                                                                   \n"
		"{                                                                                 \n"	// ���ڵ�һ��ִ�У���һ�����㣩ʱ��ֵ������ϸ�̶ֳȣ�
		"    if (gl_InvocationID == 0)                                                     \n"
		"    {                                                                             \n"
		"        gl_TessLevelInner[0] = 5.0;                                               \n"	// �ڲ�����5����������4�Ŷ��㣬����ͼ��
		"        gl_TessLevelOuter[0] = 5.0;                                               \n"	// ��߻���5��
		"        gl_TessLevelOuter[1] = 5.0;                                               \n"	// �ұ߻���5��
		"        gl_TessLevelOuter[2] = 5.0;                                               \n"	// �±߻���5��
		"    }                                                                             \n"
		"    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;     \n"	// ͨ��ֱ�ӽ� in-patch ���㸳�� out-patch ���㣨Ҳ�����½����Ƴ���
		"}                                                                                 \n"
	};

	static const char* tes_source[] =
	{
		"#version 410 core                                                                 \n"
		"                                                                                  \n"
		"layout (triangles, equal_spacing, cw) in;                                         \n"	 // ָ��ͼԪ������ϸ������ռ䡢ͼԪ���泯�� 
		//"layout (triangles, equal_spacing, cw, point_mode) in;                             \n" // �����ģʽ
		"                                                                                  \n"
		"void main(void)                                                                   \n"
		"{                                                                                 \n"
		"    gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +                       \n"	// gl_TessCoord��ϸ�ֺ���������꣨��ֵ������
		"                  (gl_TessCoord.y * gl_in[1].gl_Position) +                       \n"	// ���� input-patch ������������λ��
		"                  (gl_TessCoord.z * gl_in[2].gl_Position);                        \n"  // ÿ��ϸ�����궼���� TES ��ִ��һ��
		"}                                                                                 \n"
	};

	static const char* fs_source[] =
	{
		"#version 410 core                                                 \n"
		"                                                                  \n"
		"out vec4 color;                                                   \n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    color = vec4(0.0, 0.8, 1.0, 1.0);                             \n"
		"}                                                                 \n"
	};

	program = glCreateProgram();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_source, NULL);
	glCompileShader(vs);

	// Tesselation Control Shader
	GLuint tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(tcs, 1, tcs_source, NULL);
	glCompileShader(tcs);

	// Tesselation Evaluation Shader
	GLuint tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(tes, 1, tes_source, NULL);
	glCompileShader(tes);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fs_source, NULL);
	glCompileShader(fs);

	glAttachShader(program, vs);
	glAttachShader(program, tcs);
	glAttachShader(program, tes);
	glAttachShader(program, fs);

	glLinkProgram(program);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void render()
{
	static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, green);

	glUseProgram(program);
	glDrawArrays(GL_PATCHES, 0, 3);

	glutSwapBuffers();
}

void shutdown()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	GLfloat  aspect = GLfloat(width) / height;

	//glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q': case 'Q': case 033 /* Escape key */:
		exit(EXIT_SUCCESS);
		break;

	case 'm':	// �л����ģʽ���ο�ģʽ
	{
		static GLenum mode = GL_FILL;
		mode = (mode == GL_FILL ? GL_LINE : GL_FILL);
		glPolygonMode(GL_FRONT_AND_BACK, mode);
		break;
	}

	}

	glutPostRedisplay();
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA /*| GLUT_STENCIL | GLUT_DEPTH*/);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(512, 512);
	glutCreateWindow("TesselateTri"); // get an openGL context

	GLenum err = glewInit(); // init glew
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(-2);
	}

	startup();

	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}