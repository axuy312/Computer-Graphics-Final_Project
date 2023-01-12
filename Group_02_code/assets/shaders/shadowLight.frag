#version 430

#define shininess 10

in vec3 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 LightFragPost;

out vec4 color;

uniform int isCubeTexture;

uniform sampler2D ourTexture;
uniform sampler2D shadowMap;
uniform samplerCube blockTexture;

uniform vec3 viewPos;
uniform vec3 fakeLightPos;

uniform int enableShadow;

struct DirectionLight {
    vec3 direction;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionLight dl;

float ShadowCalculation()
{
    float bias = 0.002;
    // TODO
    // perform perspective divide
    vec3 projCoords = LightFragPost.xyz / LightFragPost.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    if (currentDepth > 1.0)
        currentDepth = 1.0;
    // check whether current frag pos is in shadow
    float shadow = closestDepth + bias < currentDepth  ? 1.0 : 0.0;
    
    return shadow;
}

void main() {
    vec3 ambient = dl.ambient;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-dl.direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dl.diffuse * diff;
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = dl.specular * spec;

    float shadow = enableShadow > 0 ? ShadowCalculation() : 0.0;
    vec3 total = (ambient + (1.0 - shadow) * (diffuse + specular));
    total = clamp(total, vec3(0, 0, 0), vec3(1, 1, 1));

    vec3 TexCoord3D = vec3(TexCoord.x, TexCoord.y, TexCoord.z);
    vec3 result1 = total * texture(blockTexture, TexCoord3D).xyz * isCubeTexture;
    
    vec2 TexCoord2D = vec2(TexCoord.x, TexCoord.y);
    vec3 result2 = total * texture(ourTexture, TexCoord2D).xyz * (1 - isCubeTexture);
    vec3 result = result1 + result2;
        
    color = vec4(result, 1.0f);

}
