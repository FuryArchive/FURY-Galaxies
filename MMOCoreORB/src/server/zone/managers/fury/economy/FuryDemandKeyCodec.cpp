#include "FuryDemandKeyCodec.h"

#include <sstream>

std::string FuryDemandKeyCodec::encode(const FuryDemandKey& key) {
	std::ostringstream out;
	out << key.planetCrc << ':' << key.regionId << ':' << key.productKey;
	return out.str();
}
