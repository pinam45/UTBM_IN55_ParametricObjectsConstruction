#ifndef PARAMETRICOBJECTSCONSTRUCTION_DRAWABLE_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_DRAWABLE_HPP

namespace poc {
	struct Drawable {
		virtual ~Drawable() = default;

		virtual void draw() = 0;
	};
}

#endif //PARAMETRICOBJECTSCONSTRUCTION_DRAWABLE_HPP
