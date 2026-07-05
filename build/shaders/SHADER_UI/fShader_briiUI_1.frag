#version 460 core

in vec2 coordTex;
in float layer;
out vec4 FragColor;

uniform sampler2DArray batch_textures;

void main()
{
 vec4 tex_Batch =  texture(batch_textures, vec3(coordTex, layer));
 
 if(tex_Batch.a < 0.1)
 {
  discard;
 }

 else
 {

  tex_Batch = vec4(1.0,1.0,1.0, tex_Batch.a);
  FragColor = tex_Batch;

 }
// FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}

