#include <Geode/Geode.hpp>
#include <Main.hpp>
#include <vector>

USE_GEODE_NAMESPACE();
using namespace tulip::dandelion;

static inline std::vector<std::function<void()>> s_enables;
static inline std::vector<std::function<void()>> s_disables;

void tulip::dandelion::registerEnable(std::function<void()> func) {
	s_enables.push_back(func);
}

void tulip::dandelion::registerDisable(std::function<void()> func) {
	s_disables.push_back(func);
}

GEODE_API bool GEODE_CALL geode_enable() {
	for (auto& func : s_enables) {
		func();
	}

	return true;
}

GEODE_API bool GEODE_CALL geode_disable() {
	for (auto& func : s_disables) {
		func();
	}
	return true;
}