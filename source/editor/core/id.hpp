#pragma once

#include <common/core/id.hpp>

namespace pspedit {

template <typename Id>
struct id_generator {
    using value_type = decltype(Id::value);

    [[nodiscard]] Id generate() noexcept
    {
        return Id { _next++ };
    }

    void reset(value_type first = 1) noexcept
    {
        _next = first;
    }

    void skip_until(value_type next) noexcept
    {
        if (_next < next) {
            _next = next;
        }
    }

    [[nodiscard]] value_type next_value() const noexcept
    {
        return _next;
    }

private:
    value_type _next = 1;
};

struct asset_ids {
    [[nodiscard]] image_id new_image_id() noexcept { return _images.generate(); }
    [[nodiscard]] mesh_id new_mesh_id() noexcept { return _meshes.generate(); }
    [[nodiscard]] material_id new_material_id() noexcept { return _materials.generate(); }
    [[nodiscard]] package_id new_package_id() noexcept { return _packages.generate(); }
    [[nodiscard]] model_id new_model_id() noexcept { return _models.generate(); }
    [[nodiscard]] transform_id new_transform_id() noexcept { return _transforms.generate(); }
    [[nodiscard]] scene_id new_scene_id() noexcept { return _scenes.generate(); }

    void reset() noexcept
    {
        _images.reset();
        _meshes.reset();
        _materials.reset();
        _packages.reset();
        _models.reset();
        _transforms.reset();
        _scenes.reset();
    }

    void sync_from_existing(
        image_id max_image,
        mesh_id max_mesh,
        material_id max_material,
        package_id max_package,
        model_id max_model,
        transform_id max_transform,
        scene_id max_scene) noexcept
    {
        _images.skip_until(max_image.value + 1);
        _meshes.skip_until(max_mesh.value + 1);
        _materials.skip_until(max_material.value + 1);
        _packages.skip_until(max_package.value + 1);
        _models.skip_until(max_model.value + 1);
        _transforms.skip_until(max_transform.value + 1);
        _scenes.skip_until(max_scene.value + 1);
    }

private:
    id_generator<image_id> _images;
    id_generator<mesh_id> _meshes;
    id_generator<material_id> _materials;
    id_generator<package_id> _packages;
    id_generator<model_id> _models;
    id_generator<transform_id> _transforms;
    id_generator<scene_id> _scenes;
};

}
