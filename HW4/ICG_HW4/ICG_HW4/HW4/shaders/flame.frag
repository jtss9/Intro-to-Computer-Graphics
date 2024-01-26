#version 330 core

uniform sampler2D ourTexture;
uniform float time;
uniform float auraWidth;

in GS_OUT {
    vec3 fragposGS;
    vec3 normalGS;
    vec2 texCoordGS;
    vec3 viewDirGS;
} fs_in;

//in vec3 color;

out vec4 FragColor;

vec3 flameColor = vec3(0.6, 0.3, 0.0);

void main()
{   
    //float alpha = smoothstep(0.0, 0.2, 1.0 - length(fs_in.fragposGS - gl_FragCoord.xyz) / 500.0);
    //vec3 color = mix(vec3(1.0, 0.5, 0.0), flameColor, alpha);
    //FragColor = vec4(color, alpha);

    vec2 uv = fs_in.texCoordGS;
    float y = sin(uv.x*10.0)*0.2+cos((uv.y+time)*10.0)*0.3;
    float t = sin(time) * 0.1 + sin(time*0.5) * 0.05;
    y+=t;
    vec3 color = vec3(1.0, 0.5-auraWidth*5, 0.0) * (1+y);
    vec3 color2 = texture(ourTexture, uv).xyz;
    FragColor = vec4(color2, 0.1+auraWidth*5);
}