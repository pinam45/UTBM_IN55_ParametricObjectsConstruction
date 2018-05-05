#ifndef PARAMETRICOBJECTSCONSTRUCTION_VIDEOMODE_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_VIDEOMODE_HPP

namespace poc {
	class VideoMode {
	public:
		explicit VideoMode(int width, int height,
		                   int redBits = 8, int greenBits = 8,
		                   int blueBits = 8, int refreshRate = 30);

		static VideoMode getPrimaryMonitorVideoMode();

		int width;
		int height;
		int redBits;
		int greenBits;
		int blueBits;
		int refreshRate;
	};
}

#endif //PARAMETRICOBJECTSCONSTRUCTION_VIDEOMODE_HPP
