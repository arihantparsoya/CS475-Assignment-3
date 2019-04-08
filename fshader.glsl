#version 330

in vec4 color;
in vec2 tex;
in vec4 eye;
in vec3 normal;

uniform float textured;

uniform float light1_isOn;
uniform float light2_isOn;

uniform sampler2D mytexture;
uniform mat4 viewMatrix;
out vec4 frag_color;

void main () 
{
	vec4 COLOR = vec4(1.0, 1.0, 0.0, 1.0);
	if(textured > 0.5){
		COLOR = texture(mytexture, tex);  
	}
  else{
  		COLOR = color;
  }

  // Defining Materials
  vec4 diffuse = vec4(1.0, 1.0, 1.0, 1.0); 
  vec4 diffuse2 = vec4(1.0, 1.0, 1.0, 1.0); 
  vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
  vec4 specular = vec4(0.5, 0.5, 0.5, 1.0);
  float shininess = 10;
  vec4 spec = vec4(0.0); 
  vec4 spec2 = vec4(0.0); 

  // Defining Light 
  vec4 lightPos = vec4(30.0, 17.0, -40.0, 0.0);
  vec4 lightPos2 = vec4(0.0,40.0,36.0,0.0);
  vec3 lightDir = vec3(viewMatrix * lightPos);  // Transforms with camera
  vec3 lightDir2 = vec3(viewMatrix * lightPos2);
  lightDir = normalize( vec3(lightDir));  
  lightDir2 = normalize( vec3(lightDir2)); 

  //Diffuse
  vec3 n = normalize(vec3(normal));      
  float dotProduct = dot(n, lightDir);
  float dotProduct2 = dot(n, lightDir2);
  float intensity =  max( dotProduct, 0.0);
  float intensity2 =  max( dotProduct2, 0.0);

  // Compute specular component only if light falls on vertex
  if(intensity > 0.0)
  {
  vec3 e = normalize(vec3(eye));
  vec3 h = normalize(lightDir + e );
    float intSpec = max(dot(h,n), 0.0); 
        spec = specular * pow(intSpec, shininess);
  }
  if(intensity2 > 0.0)
  {
  vec3 e = normalize(vec3(eye));
  vec3 h2 = normalize(lightDir2 + e );
    float intSpec2 = max(dot(h2,n), 0.0); 
        spec2 = specular * pow(intSpec2, shininess);
  }  

  vec4 color = ambient; 
  color = color + (intensity * diffuse  + spec)*COLOR*light1_isOn;
  color = color + (intensity2 * diffuse2  + spec2)*COLOR*light2_isOn;
  frag_color = color;
}
