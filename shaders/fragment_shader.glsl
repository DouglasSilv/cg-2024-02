#version 450

in vec2 texCoord;
in vec3 scaledNormal;
in vec3 fragPos;

//Propriedades da superficie
uniform vec3 ka, kd, ks;

//Propriedades da fonte de luz
uniform vec3 lightPos, lightColor;

//Propriedades da câmera
uniform vec3 cameraPos;

out vec4 color;
//Buffer da textura
uniform sampler2D texBuffer;

uniform float uLightIntensity;      // Intensidade da luz
uniform float uSpecularCoefficient; // Coeficiente especular

void main()
{

    //Coeficiente luz ambiente
    vec3 ambient = ka * lightColor;

    //Coeficiente reflexão difusa
    vec3 diffuse;
    vec3 N = normalize(scaledNormal);
    vec3 L = normalize(lightPos - fragPos);
    float diff = max(dot(N,L),0.0);
    diffuse = kd * diff * lightColor;

    //Coeficiente reflexão especular
    vec3 specular;
    vec3 R = normalize(reflect(-L,N));
    vec3 V = normalize(cameraPos - fragPos);
    float spec = max(dot(R,V),0.0);
    spec = pow(spec, 10);
    specular = ks * spec * lightColor;

    vec4 texColor = texture(texBuffer,texCoord);
    vec3 result = (ambient + diffuse) * vec3(texColor) + specular;

    color = vec4(result,1.0);
}