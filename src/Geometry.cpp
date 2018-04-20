#include "Geometry.h"

namespace Meadow {
	namespace Geometry {
		Degrees::Degrees(float val)   : value(val) { }
		Degrees::Degrees(Radians rad) : value(rad.value * 57.2957795131) { }
		Radians::Radians(float val)   : value(val) { }
		Radians::Radians(Degrees deg) : value(deg.value * 0.01745329251) { }
	}
}
