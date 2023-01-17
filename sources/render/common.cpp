#include "common.hpp"

static StringView Include = "shaders";
ShaderCompileOptions DefaultCompileOptions{ ShaderLang::GLSL, {&Include, 1} };