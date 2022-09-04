#version 450 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;


//uniform float Size2;   // Half the width of the quad

in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out vec3 fs_Normal;
out vec3 fs_FragPos;
out vec4 fs_FragPosLightSpace;

void main()
 {
	gl_Position = gl_Position;
	EmitVertex();
	//EndPrimitive();

	/*
	mat4 m = projection;
	float Size2 = 0.5;
	gl_Position = m * (vec4(-Size2,-Size2,0.0,0.0) + gl_in[0].gl_Position);
    //TexCoord = vec2(0.0,0.0);
    EmitVertex();

    gl_Position = m * (vec4(Size2,-Size2,0.0,0.0) + gl_in[0].gl_Position);
    //TexCoord = vec2(1.0,0.0);
    EmitVertex();

    gl_Position = m * (vec4(-Size2,Size2,0.0,0.0) + gl_in[0].gl_Position);
    //TexCoord = vec2(0.0,1.0);
    EmitVertex();

    gl_Position = m * (vec4(Size2,Size2,0.0,0.0) + gl_in[0].gl_Position);
    //TexCoord = vec2(1.0,1.0);
    EmitVertex();

    EndPrimitive();
    */

    /////////////////////////////

	fs_FragPos = FragPos;
	fs_Normal = Normal;
	fs_FragPosLightSpace = FragPosLightSpace;
}