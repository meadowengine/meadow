#ifndef MEADOW_TRANSFORM_H_
#define MEADOW_TRANSFORM_H_

namespace Meadow {
	namespace Geometry {
		class Vector {
		public:
			float x, y, z;
		};

		class Radians;

		class Degrees {
		public:
			float value;
			explicit Degrees(float);
			Degrees(Radians rad);
		};

		class Radians {
		public:
			float value;
			explicit Radians(float);
			Radians(Degrees deg);
		};

		class Transform {
		public:
			float matrix[12];

			Transform(float, float, float, float,
				      float, float, float, float,
				      float, float, float, float
		         	/*    0,     0,     0,     1*/);

			static Transform translation(Vector);
			static Transform scale(Vector);
			static Transform rotation(Vector,Radians);
		};
	}
}
#endif