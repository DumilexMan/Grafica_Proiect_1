//
// ================================================
// | Grafica pe calculator                        |
// ================================================
// | Laboratorul III - 03_05_Shader.frag |
// =======================================
// 
//  Shaderul de fragment / Fragment shader - afecteaza culoarea pixelilor;
//

#version 330	//  Versiunea GLSL;

//	Variabile de intrare (dinspre Shader.vert);
in vec4 ex_Color;
in vec2 ex_Text_Coord;

//	Variabile de iesire	(spre programul principal);
out vec4 out_Color;

uniform sampler2D myTexture;

//	Actualizeaza culoarea in functie de codCol;
void main(void)
{
	out_Color = mix(texture(myTexture, ex_Text_Coord), ex_Color, 0.2);
}
