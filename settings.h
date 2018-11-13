#ifndef SETTINGS_H_
#define SETTINGS_H_

namespace game {
	class Settings {

		private:
			bool toonModeOn_;
			float surface_roughness_;

		public:

			Settings(void);
			~Settings();

			// Manage toon mode setting
			bool IsToonModeOn(void);
			void ToggleToonMode(void);
			void IncrementSurfaceRoughness(void);
			void DecrementSurfaceRoughness(void);
			float GetRoughness(void);


	}; // class Globals

}; // namespace game

#endif // SETTINGS_H_
