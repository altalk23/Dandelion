
#include <Geode/modify/PlayLayer.hpp>

class LowFPSKick : Modify<LowFPSKick, PlayLayer> {
	void update(float dt) {
		m_shouldTryToKick = false;
		PlayLayer::update(dt);
	}
};
