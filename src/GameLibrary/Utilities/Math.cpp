
#include <GameLibrary/Utilities/Math.hpp>
#include <cmath>
#include <cstdlib>

namespace fgl
{
	const long double Math::PI = 3.14159265358979323846264338328L;
	
	//Random
	double Math::random() { return ((double)std::rand())/((double)(RAND_MAX)); }
}
