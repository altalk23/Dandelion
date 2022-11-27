
#include <Geode/modify/PlayLayer.hpp>

struct PracticeModeLag : Modify<PracticeModeLag, PlayLayer> {
	int resetDelta;

	bool init(GJGameLevel* level) {
		m_fields->resetDelta = 0;
		return PlayLayer::init(level);
	}

	void update(float dt) override {
		if (m_fields->resetDelta == 0) {
			return PlayLayer::update(dt);
		}

		float time = 0.0001;
		if (m_fields->resetDelta != 0 && dt - time < 1) { // if close enough to normal speed
			m_fields->resetDelta -= 1;
		}

		return PlayLayer::update(time);
	}

	void resetLevel() {
		if (m_isPracticeMode || m_isTestMode) {
			m_fields->resetDelta = 2; // Account for 1 extra frame respawn
		}
		PlayLayer::resetLevel();
	}
};