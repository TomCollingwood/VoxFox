/// @author Richard Southern

#version 410 core

// The modelview and projection matrices are no longer given in OpenGL 4.2
uniform mat4 MV;
uniform mat4 MVP;
uniform mat3 N; // This is the inverse transpose of the MV matrix

/* NOTES
        -uniform is the same for every one of million verticies / parallel processes
        -attributes "in" are different for each vertex / parallel process
*/

// The vertex position attribute
layout (location=0) in vec3 VertexPosition;

// The texture coordinate attribute
layout (location=1) in vec2 TexCoord;

// The vertex normal attribute
layout (location=2) in vec3 VertexNormal;

// The vertex color attribute
layout (location=3) in vec3 VertexColor;

out vec3 FragmentPosition;
out vec3 FragmentNormal;
out vec2 FragmentTexCoord;
out vec3 FragmentColor;

out mat4 _MV;

/************************************************************************************/
void main() {
    // Set the position of the current vertex
    gl_Position = MVP * vec4(VertexPosition, 1.0);

    FragmentPosition = vec3(MV * vec4(VertexPosition,1.0));
    FragmentNormal = N * VertexNormal;
    FragmentTexCoord = TexCoord;
    FragmentColor = VertexColor;
    _MV = MV;
}

