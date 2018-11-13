#include "settings.h"

namespace game {

Settings::Settings() {
	
	toonModeOn_ = true;
	surface_roughness_ = 0.5f;
}


Settings::~Settings() {

}


void Settings::ToggleToonMode() {
	
	toonModeOn_ = !toonModeOn_;
};


bool Settings::IsToonModeOn() {
	
	return toonModeOn_;
}

void Settings::IncrementSurfaceRoughness() {
	
	surface_roughness_ += 0.1f;

	if (surface_roughness_ >= 1.0f) {
		surface_roughness_ = 1.0f;
	}
}

void Settings::DecrementSurfaceRoughness() {
	
	surface_roughness_ -= 0.1f;

	if (surface_roughness_ <= 0.0f) {
		surface_roughness_ = 0.0f;
	}
}

float Settings::GetRoughness() {

	return surface_roughness_;
}

}