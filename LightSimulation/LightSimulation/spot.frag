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

struct Spot {
  vec4 LightPosition;
  vec3 LightIntensity;
  vec3 LightDirection;
  float LightExponent;
  float LightCutoff;
  float LightInnerCutoff;
};
uniform Spot SpotLight;

uniform mat4 ViewMatrix;

void main()
{
  vec3 finalColor = vec3(0, 0, 0);

  float SpotAttenuation;

  float D = distance(Position, SpotLight.LightPosition);
  float attenuation = 1 / (1.0 + (0.01 * D) + (0.001 * pow(D, 2)));

  vec3 L = normalize((SpotLight.LightPosition - Position).xyz);

  float teta = dot(normalize(SpotLight.LightDirection), -L);
  float tetaDeg = degrees(acos(teta));

  SpotAttenuation = 1.0;
  if (tetaDeg >= SpotLight.LightInnerCutoff)
  {
  	float Spot = smoothstep(cos(radians(SpotLight.LightCutoff)), cos(radians(SpotLight.LightInnerCutoff)), teta);
	SpotAttenuation = pow(Spot, SpotLight.LightExponent);
  }
  
  attenuation *= SpotAttenuation;
  
  vec3 ambient = SpotLight.LightIntensity * Material.Ka * attenuation;
  
  float dotNL = max(dot(L, Normal), 0.0);
  vec3 diffuse = Material.Kd * SpotLight.LightIntensity * dotNL * attenuation;
  
  vec3 V = normalize(-Position.xyz);
  vec3 H = normalize(V + L);
  float dotHN = max(dot(H, Normal), 0.0);
  
  vec3 R = normalize(reflect(-L, Normal));
  float dotRV = max(dot(R, V), 0.0);
  
  vec3 specular = Material.Ks * SpotLight.LightIntensity * pow(dotRV, Material.Shiness) * attenuation;
  
  finalColor = diffuse + specular + ambient;
  
  FragColor = vec4(finalColor, 0.5);
}