#pragma once

#include "animation_frame.h"

class FrameInterpolator {
public:
    AnimationFrame interpolate(const AnimationFrame& from,
        const AnimationFrame& to,
        double t) const {
        (void)t;
        return from;
    }
};
