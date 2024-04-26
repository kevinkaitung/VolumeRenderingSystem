#pragma once
#include "definition.h"

void make_view(int type);

void make_projection(int type);

void mainSetupAndLoop(int argc, char** argv, char* vertShaderFileName, char* fragShaderFileName, int passRes[3]);