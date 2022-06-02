#include "eospch.h"
#include "Entity.h"

namespace Eos {

	Entity::Entity(entt::entity handle, Scene& scene)
		: m_EntityHandle(handle), m_Scene(&scene)
	{
	}

}
