#ifndef MEADOW_TAGGING_H_
#define MEADOW_TAGGING_H_

namespace Meadow {
	namespace Tag {
		const int None = 0;
	}
}

#define MeadowUserTags(first, ...)\
namespace Meadow { namespace Tag {\
	enum UserTags : int {         \
		first = None + 1,         \
		__VA_ARGS__               \
	};                            \
} }

#endif