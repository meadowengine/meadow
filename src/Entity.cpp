#include "Entity.h"

namespace Meadow {

	Component::~Component() { }
	void Component::tick(float dt) { }
	void Component::render() { }

	static std::vector<Single<Entity>> entities;

	Entity::Entity() : tag(Tag::None) { }

	Entity& Entity::setTag(int tag_) {
		tag = tag_;
		return *this;
	}

	void Entity::tick(float dt) {
		for (auto &&c : components) {
			c->tick(dt);
		}
	}

	void Entity::render() {
		for (auto &&c : components) {
			c->render();
		}
	}

	bool Entity::fulfills(Selector s) const {
		return s(*this);
	}

	Ref<Entity> Entity::first(Selector s) {
		for (auto &&e : entities) {
			if (e->fulfills(s)) {
				return e;
			}
		}
		return NullRef<Entity>();
	}

	Ref<Entity> Entity::create() {
		struct OwnerCreationEnabler : public Entity { };

		Single<Entity> e = my<OwnerCreationEnabler>();
		entities.push_back(std::move(e));
		return Ref<Entity>(entities[entities.size() - 1]);
	}

	void Entity::stream(std::function<void(Ref<Entity>)> func) {
		for (auto &&e : entities) {
			func(e);
		}
	}

	void tickEntities(float dt) {
		for (auto &&e : entities) {
			e->tick(dt);
		}
	}

	void renderEntities() {
		for (auto &&e : entities) {
			e->render();
		}
	}
}

Meadow::Selector operator!(Meadow::Selector s) {
	return [=](const Meadow::Entity& e) {
		return !s(e);
	};
}

Meadow::Selector operator||(Meadow::Selector a, Meadow::Selector b) {
	return [=](const Meadow::Entity& e) {
		return a(e) || b(e);
	};
}

Meadow::Selector operator&&(Meadow::Selector a, Meadow::Selector b) {
	return [=](const Meadow::Entity& e) {
		return a(e) && b(e);
	};
}