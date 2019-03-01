#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TGAPEngine/include/Utility.h"
#include <random>
#include <chrono>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace TGAPEngineNativeTests
{
	TEST_CLASS(Utility_Test)
	{
	public:

		TEST_METHOD(interpf_simple_values_range_start)
		{
			Assert::AreEqual(1.0f, engine::interpf(1.0f, 0.0f, 0.0f));
		}
		TEST_METHOD(interpf_simple_values_range_end)
		{
			Assert::AreEqual(0.0f, engine::interpf(1.0f, 0.0f, 1.0f));
		}
		TEST_METHOD(interpf_simple_values_range_middle)
		{
			Assert::AreEqual(0.5f, engine::interpf(1.0f, 0.0f, 0.5f));
		}
		TEST_METHOD(interpf_real_range_start)
		{
			const auto rangeStart = 554.51222f;
			const auto rangeEnd = 3814.46261f;

			Assert::AreEqual(rangeStart, engine::interpf(rangeStart, rangeEnd, 0.0f));
		}
		TEST_METHOD(interpf_real_range_end)
		{
			const auto rangeStart = 554.51222f;
			const auto rangeEnd = 3814.46261f;

			Assert::AreEqual(rangeEnd, engine::interpf(rangeStart, rangeEnd, 1.0f));
		}
		TEST_METHOD(interpf_real_range_middle)
		{
			const auto rangeStart = 554.51222f;
			const auto rangeEnd = 3814.46261f;
			auto rangeMiddle = ((rangeEnd - rangeStart) * 0.5f) + rangeStart;

			Assert::AreEqual(rangeMiddle, engine::interpf(rangeStart, rangeEnd, 0.5f));
		}
		TEST_METHOD(interpf_real_range_check)
		{
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::mt19937 mt(seed);
			std::uniform_real_distribution<> dist(0.0f, 1.0f);
			const auto inRangeValue = dist(mt);
			const auto rangeStart = 1.0f;
			const auto rangeEnd = 10.0f;
			auto result = engine::interpf(rangeStart, rangeEnd, inRangeValue);
			Assert::IsTrue(result >= rangeStart && result <= rangeEnd);
		}

	};
}