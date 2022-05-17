#ifndef __IG_AUXILIAR__
#define __IG_AUXILIAR__

//Carga una textura y devuelve un puntero a su ubicacion en memoria principal
//tambi�n devuelve el tama�o de la textura (w,h)
//!!Ya implementada
unsigned char *loadTexture(const char* fileName, unsigned int &w, unsigned int &h);

//Carga un fichero en una cadena de caracteres
char *loadStringFromFile(const char *fileName, unsigned int &fileLen);


#endif