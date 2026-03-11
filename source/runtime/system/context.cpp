#include <runtime/system/context.hpp>

namespace pspedit {
namespace {


	// void _load_scene_recursive(entt::registry& components, scene& runtime_asset, const scene_asset& asset)
	// {
	// 	for (const scene_entity& _entity : asset.entities) { // in fact iterate over runtime entities provided by scene
	// 		const entt::entity _entt_entity = {}; // get runtime entity from scene 
	// 		if (_entity.model) {
	// 			const std::filesystem::path _model_path; // resolve path from flat context and model_id
	// 			model_asset _asset;
	// 			// load_asset(_model_path, _asset);				
	// 			model& _model = components.get<model>(_entt_entity);
	// 			// modify from model_asset
	// 			// _model._resolved_mesh =  // load mesh if not loaded and resolve
	// 			// _model._resolved_material =  // load material if not loaded and resolve
	// 		}
	// 	}

	// }

	// void _load_scene_recursive_async(entt::registry& components, scene& runtime_asset, const scene_asset& asset)
	// {
	// 	for (const scene_entity& _entity : asset.entities) {
	// 		const entt::entity _entt_entity = components.create();
	// 		if (_entity.model) {
	// 			components.emplace<model>(_entt_entity);
	// 		}
	// 		if (_entity.transform) {
	// 			components.emplace<transform>(_entt_entity);
	// 		}
	// 	}

	// }


}








}