#version 330    //  Versiunea GLSL;

//  Variabile de intrare (dinspre programul principal);
layout (location = 0) in vec4 in_Position;     //  Se preia din buffer de pe prima pozitie (0) atributul care contine coordonatele;
layout (location = 1) in vec4 in_Color;        //  Se preia din buffer de pe a doua pozitie (1) atributul care contine culoarea;
layout (location = 2) in vec2 in_Text_Coord;

//  Variabile de iesire;
out vec4 gl_Position;   //  Transmite pozitia actualizata spre programul principal;
out vec4 ex_Color;      //  Transmite culoarea (de modificat in Shader.frag); 
out vec2 ex_Text_Coord;

//  Variabile uniforme;
uniform mat4 myMatrix;

void main(void)
{
    gl_Position = myMatrix*in_Position;
    ex_Color = in_Color;
    ex_Text_Coord = vec2(in_Text_Coord.x, 1 - in_Text_Coord.y);
} 
