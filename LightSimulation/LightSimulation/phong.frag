#version 440

in vec4 Position;
in vec3 Normal;

layout(location = 0) out vec4 FragColor;

struct MaterialInfo {
	vec3 Kd;
	vec3 Ka;
	vec3 Ks;
	float Shiness;
};
uniform MaterialInfo Material;

struct LightInfo {
  vec4 LightLocation;
  vec3 LightIntensity;
};
uniform LightInfo Light[5];

uniform mat4 ViewMatrix;

void main()
{
  vec3 finalColor = vec3(0, 0, 0);
  
  vec3 ambient = Light[0].LightIntensity * Material.Ka;
  
  for (int i = 0; i < 5; i++) 
  {
    vec3 L = normalize((ViewMatrix * Light[i].LightLocation - Position).xyz);
	float dotNL = max(dot(L, Normal), 0.0);
	vec3 diffuse = Material.Kd * Light[i].LightIntensity * dotNL;

    vec3 V = normalize(-Position.xyz);
	vec3 H = normalize(V + L);
	float dotHN = max(dot(H, Normal), 0.0);
	
	vec3 R = normalize(reflect(-L, Normal));
	float dotRV = max(dot(R, V), 0.0);
	
	vec3 specular = Material.Ks * Light[i].LightIntensity * pow(dotRV, Material.Shiness);

	//calculate diffuse and spec for each light
	finalColor += diffuse + specular;
   }
   finalColor += ambient;

   FragColor = vec4(finalColor, 0.5);
}