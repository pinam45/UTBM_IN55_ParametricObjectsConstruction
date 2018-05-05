#ifndef PARAMETRICOBJECTSCONSTRUCTION_MOUSE_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_MOUSE_HPP

namespace poc {
	class Mouse {
	public:
		enum class Button : int {
			Button1 = 0,
			Button2,
			Button3,
			Button4,
			Button5,
			Button6,
			Button7,
			Button8,
			ButtonLeft = Button1,
			ButtonRight = Button2,
			ButtonMiddle = Button3
		};
	};
}

#endif //PARAMETRICOBJECTSCONSTRUCTION_MOUSE_HPP
