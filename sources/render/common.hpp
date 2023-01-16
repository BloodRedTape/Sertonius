#pragma once

#include <graphics/api/shader.hpp>


#define DefaultCompileOptions {ShaderLang::GLSL, ConstSpan<StringView>{ StringView("shaders") }}
