#ifndef MEADOW_COMPONENT_H_
#define MEADOW_COMPONENT_H_

#include <vector>
#include <memory>
#include <functional>

#include <Pointed.h>

#include "Geometry.h"
#include "Tagging.h"

namespace Meadow {

	using namespace Pointed;

	class Entity;

	typedef std::function<bool(const Entity&)> Selector;

	class Component {
	public:
		virtual ~Component();

		virtual void tick(float dt);
		virtual void render();
	};

	class Entity {
	private:
		Entity();
		std::vector<Single<Component>> components;
	public:
		//Geometry::Transform transform;

		int tag;

		template<class T>
		Ref<T> getComponent() const {
			for (auto &&c : components) {
				Ref<T> ref = RefCast<T, Component>(c);
				if (ref) {
					return ref;
				}
			}
			return Ref<T>();
		}

		template<class T>
		Entity& addComponent(Single<T> comp) {
			components.push_back(std::move(comp));
			return *this;
		}

		Entity& setTag(int);

		void tick(float dt);
		void render();

		bool fulfills(Selector) const;

		static Ref<Entity> create();
		static Ref<Entity> first(Selector);

		static void stream(std::function<void(Ref<Entity>)>);
	};

	namespace Selection {
		template<class T>
		Selector WithComponent() {
			return [](const Entity& e) {
				return (bool)e.getComponent<T>();
			};
		}

		template<int tag>
		Selector Tagged() {
			return [](const Entity& e) {
				return e.tag == tag;
			};
		}
	};
}

Meadow::Selector operator!(Meadow::Selector);
Meadow::Selector operator||(Meadow::Selector, Meadow::Selector);
Meadow::Selector operator&&(Meadow::Selector, Meadow::Selector);

#endif