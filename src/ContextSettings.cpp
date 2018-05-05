#include "ContextSettings.hpp"

poc::ContextSettings::ContextSettings(int depthBits_, int stencilBits_, int antialiasingLevel_,
                                      int major_, int minor_, poc::Mode mode_) noexcept
	: depthBits{depthBits_}
	, stencilBits{stencilBits_}
	, antialiasingLevel{antialiasingLevel_}
	, major{major_}
	, minor{minor_}
	, mode(mode_)
{
}