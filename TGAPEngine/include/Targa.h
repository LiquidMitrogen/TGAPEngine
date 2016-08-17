/*
 *	Targa.h
 */
#pragma once
typedef struct {
	GLuint bpp;		// iloœæ bitów na piksel
	GLuint width;	// rozmiary w pikselach
	GLuint height;
} TARGAINFO;

// Funkcje
GLubyte *LoadTGAImage(char *filename, TARGAINFO *info,  bool ignoreHeaders);
bool LoadTGATexture(char *filename);
//bool LoadTGAMipmap(char *filename);
