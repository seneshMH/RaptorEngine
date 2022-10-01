#include "rtpch.h"
#include "UUID.h"

#include <random>

namespace Raptor {

	static std::random_device s_RandomDevive;
	static std::mt19937_64 s_Engine(s_RandomDevive());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;


	UUID::UUID()
		:m_UUID(s_UniformDistribution(s_Engine))
	{

	}

	UUID::UUID(uint64_t uuid)
		:m_UUID(uuid)
	{

	}

}