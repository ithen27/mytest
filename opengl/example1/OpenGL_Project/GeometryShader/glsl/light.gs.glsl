#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform mat4 projection;

void main() {
	
	/*
	//mat4 m = projection;
	float Size2 = 0.5;

    gl_Position = projection * (vec4(-Size2,-Size2,0.0,0.0) + gl_in[0].gl_Position);    
    EmitVertex();

    gl_Position = projection * (vec4(Size2,-Size2,0.0,0.0) + gl_in[0].gl_Position);
    EmitVertex();

    gl_Position = projection * (vec4(-Size2,Size2,0.0,0.0) + gl_in[0].gl_Position);
    EmitVertex();

    gl_Position = projection * (vec4(Size2,Size2,0.0,0.0) + gl_in[0].gl_Position);
    EmitVertex();

    EndPrimitive();
    */

    /*
    for(int i=0; i<gl_in.length(); i++)
  	{
    	vec3 P = gl_in[i].gl_Position.xyz;
    	//vec3 N = vertex[i].normal.xyz;
    	vec3 N = vec3(0.5, 0.5, 0.5);
    	float normal_length = 1.0;
    
    	gl_Position = projection * vec4(P, 1.0);
    	//vertex_color = vertex[i].color;
    	EmitVertex();
    
    	gl_Position = projection * vec4(P + N * normal_length, 1.0);
    	//vertex_color = vertex[i].color;
    	EmitVertex();
    
    	EndPrimitive();
  	}
  	*/

  	gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[1].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[2].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

}