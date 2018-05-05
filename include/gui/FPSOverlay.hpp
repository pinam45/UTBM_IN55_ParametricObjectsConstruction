#ifndef PARAMETRICOBJECTSCONSTRUCTION_FPSOVERLAY_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_FPSOVERLAY_HPP

#include <list>

#include "Drawable.hpp"

namespace poc {
    class FPSOverlay : public Drawable {
    public:
        FPSOverlay() = default;

        void draw() override;

        bool isOpen() const;

        void open();

        void close();

        void setOpen(bool open);
    private:
        bool m_open = true;
        unsigned int m_corner = 0;
    };
}

#endif //PARAMETRICOBJECTSCONSTRUCTION_FPSOVERLAY_HPP
