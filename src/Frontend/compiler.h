#pragma once

#include "common.h"
#include "Chunk/chunk.h"
#include "Core/object.h"
#include "lexer.h"

bool compile(const char *source, Chunk *chunk);
