#pragma once

#include <optional>
#include <memory>
#include <functional>

#include <entt/entity/registry.hpp>

#include <common/asset/scene.hpp>

namespace pspedit {

struct scene {
    scene(const scene_asset& asset);
    scene(const scene& other) = delete;
    scene& operator=(const scene& other) = delete;
    scene(scene&& other) noexcept = default;
    scene& operator=(scene&& other) noexcept = default;

    [[nodiscard]] entity_id create_entity();
	void destroy_entity(const entity_id entity);

	[[nodiscard]] model_id create_model(const entity_id entity);
	[[nodiscard]] transform_id create_transform(const entity_id entity);

	[[nodiscard]] std::optional<model_id> get_model(const entity_id entity);
	[[nodiscard]] std::optional<transform_id> get_transform(const entity_id entity);

	// void foreach_models(const std::function<void(model&)>& callback);
	// void foreach_transforms(const std::function<void(transform&)>& callback);

	void destroy_model(const entity_id entity);
	void destroy_transform(const entity_id entity);

private:
    entt::registry _registry;
};

}