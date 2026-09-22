#ifndef FURYDEMANDKEYCODEC_H_
#define FURYDEMANDKEYCODEC_H_

#include <string>

#include "FuryDemandKey.h"

class FuryDemandKeyCodec {
public:
	static std::string encode(const FuryDemandKey& key);
};

#endif // FURYDEMANDKEYCODEC_H_
