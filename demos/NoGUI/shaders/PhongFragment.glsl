/// @author Richard Southern

#version 410 core


// This is passed on from the vertex shader
//in vec3 LightIntensity;
in vec3 FragmentPosition;
in vec3 FragmentNormal;
in vec3 FragmentColor;
in mat4 _MV;

// This is no longer a built-in variable
layout (location=0) out vec4 FragColor;

/************************************************************************************/

// Structure for holding light parameters
struct LightInfo {
    vec4 Position; // Light position in eye coords.
    vec3 La; // Ambient light intensity
    vec3 Ld; // Diffuse light intensity
    vec3 Ls; // Specular light intensity
};

// We'll have a single light in the scene with some default values
uniform LightInfo Light = LightInfo(
            vec4(2.0, 2.0, 10.0, 1.0),   // position
            vec3(0.2, 0.2, 0.2),        // La
            vec3(1.0, 1.0, 1.0),        // Ld
            vec3(1.0, 1.0, 1.0)         // Ls
            );

// The material properties of our object
struct MaterialInfo {
    vec3 Ka; // Ambient reflectivity
    vec3 Kd; // Diffuse reflectivity
    vec3 Ks; // Specular reflectivity
    float Shininess; // Specular shininess factor
};

// The object has a material
uniform MaterialInfo Material = MaterialInfo(
            vec3(1, 1, 1),    // Ka
            vec3(1.0, 1.0, 1.0),    // Kd
            vec3(1.0, 1.0, 1.0),    // Ks
            3.0                    // Shininess
            );

/************************************************************************************/
vec3 LightIntensity;




void main() {
    // Transform your input normal
    vec3 n = normalize( FragmentNormal );

    // Calculate the light vector
    vec3 s = normalize( vec3(Light.Position) - FragmentPosition.xyz );

    // Calculate the vertex position
    vec3 v = normalize(vec3(-FragmentPosition.xyz));

    // Reflect the light about the surface normal
    vec3 r = reflect( -s, n );

    LightIntensity = (
            Light.La * FragmentColor +
            Light.Ld * FragmentColor * max( dot(s, FragmentNormal), 0.0 ) +
            Light.Ls * FragmentColor * pow( max( dot(r,v), 0.0 ), Material.Shininess ));

    FragColor = vec4(LightIntensity,1.0);

    if(FragmentNormal[0] > 1.0f)
    {
      FragColor = vec4(FragmentColor,1.0);
    }

}
