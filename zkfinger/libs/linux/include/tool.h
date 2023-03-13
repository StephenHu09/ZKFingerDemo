#ifndef _TOOL_H_
#define _TOOL_H_


#ifndef BYTE
#define BYTE unsigned char
#endif // !BYTE

int WriteBitmap(unsigned char *buffer, int Width, int Height, unsigned char* outBuffer);

int WriteBitmapFile(BYTE *buffer, int Width, int Height, char *file);

#endif
