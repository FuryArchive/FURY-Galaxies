#ifndef FURYDEMANDKEY_H_
#define FURYDEMANDKEY_H_

#include <cstddef>
#include <cstdint>
#include <functional>

struct FuryDemandKey {
	std::uint32_t planetCrc = 0;
	std::uint64_t regionId = 0;
	std::uint32_t productKey = 0;

	bool operator==(const FuryDemandKey& other) const {
		return planetCrc == other.planetCrc &&
			regionId == other.regionId &&
			productKey == other.productKey;
	}
};

struct FuryDemandKeyHash {
	std::size_t operator()(const FuryDemandKey& key) const {
		std::size_t seed = std::hash<std::uint32_t>()(key.planetCrc);
		seed ^= std::hash<std::uint64_t>()(key.regionId) + 0x9e3779b9u + (seed << 6) + (seed >> 2);
		seed ^= std::hash<std::uint32_t>()(key.productKey) + 0x9e3779b9u + (seed << 6) + (seed >> 2);
		return seed;
	}
};

#endif // FURYDEMANDKEY_H_
