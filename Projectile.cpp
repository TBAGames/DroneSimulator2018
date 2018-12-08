#include "Projectile.h"

namespace game {

	void Projectile::DestroyProjectile(bool condition)
	{
		if (condition)
		{
			delete this;
		}
	}
}