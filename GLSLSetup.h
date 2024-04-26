#pragma once
/*------------------------------------------------------------------------------------------
 * Procedure to set up the vertex and fragment shader.
 * This procedure also create a program object containing the shader.
 */

void setGLSLshaders(char* VertexShaderFileName, char* FragmentShaderFileName);

/*---------------------------------------------------------------------------------------------
 * Return the program object handle (ID).
 */
int ReturnProgramID(int ProgramIndex);
