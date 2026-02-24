#pragma once

#include "animation_frame.h"

// FrameInterpolator is responsible for producing intermediate frames
// between two AnimationFrame states. The default implementation is a
// no-op that returns the 'from' frame; a richer implementation can
// interpolate positions/colors over parameter t in [0,1].
class FrameInterpolator {
public:
    AnimationFrame interpolate(const AnimationFrame& from,
        const AnimationFrame& to,
        double t) const {
        (void)t;
        return from;
    }
};
