#ifndef __BOXFILE__
#define __BOXFILE__

//Describimos un tetraedro
//*******************
//Se replican vértices para que cada cara 
//tenga una normal distinta.

//Número de vértices
const int cubeNVertex = 12; // 3 vértices x 4 caras
const int cubeNTriangleIndex = 4; //  triángulos por cara x  4 cara;


const unsigned int cubeTriangleIndex[] = {
	//Cara 1
	0,1,2,
	//Cara 2
	3,4,5,
	//Cara 3
	6,7,8,
	//Cara 4
	9,10,11,
};

//Posicíon de los vertices
const float cubeVertexPos[] = {
	//Cara 1
	0.0f,0.0f,2.0f,
	-0.943f,-1.6330f,-0.6666f,
	-0.943f,1.6330f,-0.6666f,

	//Cara 2	   
	0.0f,0.0f,2.0f,
	1.885f,0.0f,-0.666f,
	-0.943f,-1.6330f,-0.6666f,
	
	//Cara 3		   
	0.0f,0.0f,2.0f,
	-0.943f,1.6330f,-0.6666f,
	1.885f,0.0f,-0.666f,
	

	//Cara 4
	-0.943f,1.6330f,-0.6666f,
	-0.943f,-1.6330f,-0.6666f,
	1.885f,0.0f,-0.666f,
};


//Normal de los vertices
const float cubeVertexNormal[] = {
	//Cara 1
	0.9428f,0.0f,-0.3334,
	0.9428f,0.0f,-0.3334,
	0.9428f,0.0f,-0.3334,

	//Cara 2	   
	-0.4933f,0.8543,-0.1638,
	-0.4933f,0.8543,-0.1638,
	-0.4933f,0.8543,-0.1638,

	//Cara 3		   
	-0.4933f,-0.8543,-0.1638,
	-0.4933f,-0.8543,-0.1638,
	-0.4933f,-0.8543,-0.1638,

	//Cara 4	   
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,
};


//Color de los vertices
const float cubeVertexColor[] = {

	//Cara z = 1
	0.0f,	0.0f,	 1.0f,
	0.0f,	0.0f,	 1.0f,
	0.0f,	0.0f,	 1.0f,

	//Cara z = -1		   
	0.6f,	0.6f,	 1.0f,
	0.6f,	0.6f,	 1.0f,
	0.6f,	0.6f,	 1.0f,

	//Cara x = 1		   
	1.0f,	0.0f,	 0.0f,
	1.0f,	0.0f,	 0.0f,
	1.0f,	0.0f,	 0.0f,

	//Cara x = -1		   
	1.0f,	0.6f,	 0.6f,
	1.0f,	0.6f,	 0.6f,
	1.0f,	0.6f,	 0.6f,
};

//Cood. de textura de los vertices
const float cubeVertexTexCoord[] = {

	//Cara 1
	 0.5f,1.0f,
	 0.0f, 0.0f,
	 1.0f, 0.0f,

	 //Cara 2
	 0.5f,1.0f,
	 1.0f, 0.0f,
	 0.0f, 0.0f,

	 //Cara 3	
	 0.5f,1.0f,
	 1.0f, 0.0f,
	 0.0f, 0.0f,

	 //Cara 4
	 1.0f,	0.0f,
	 0.0f,0.0f,
	 0.5f,1.0f,
};


const float cubeVertexTangent[] = {

	//Cara 1
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,

	//Cara 2	   
	0.9608f,0.2774f,0.0f,
	0.9608f,0.2774f,0.0f,
	0.9608f,0.2774f,0.0f,

	//Cara 3		   
	-0.9608f,-0.2774f,0.0f,
	-0.9608f,-0.2774f,0.0f,
	-0.9608f,-0.2774f,0.0f,

	//Cara 4		   
	0.0f,-1.0f,0.0f,
	0.0f,-1.0f,0.0f,
	0.0f,-1.0f,0.0f,
};

#endif


