#include "root/graphics/interface/resource_manager.h"
#include "utils/logging/logging.h"

template <typename T>
template <typename... Args>
typename Wombat::Graphics::Loader<T>::result_type Wombat::Graphics::Loader<T>::operator()(Args &&...args) const
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

Wombat::Graphics::ResourceManager::ResourceManager()
    : shader_cache(), texture_cache(), material_cache(), mesh_cache()
{
    LOG("ResourceManager initialised", Logging::LOG_TYPE::INFO, Logging::LOG_PRIORITY::MEDIUM);
}

Wombat::Graphics::ResourceManager::~ResourceManager()
{
    shader_cache.clear();
    texture_cache.clear();
    material_cache.clear();
    mesh_cache.clear();
    LOG("ResourceManager destroyed", Logging::LOG_TYPE::INFO, Logging::LOG_PRIORITY::MEDIUM);
}

Wombat::Graphics::ResourceManager::ResourceManager(ResourceManager &&other)
    :shader_cache(std::move(other.shader_cache)), texture_cache(std::move(other.texture_cache)), 
    material_cache(std::move(other.material_cache)), mesh_cache(std::move(mesh_cache))
{}

Wombat::Graphics::ResourceManager &Wombat::Graphics::ResourceManager::operator=(ResourceManager &&other) noexcept
{
    this->shader_cache = std::move(other.shader_cache);
    this->texture_cache = std::move(other.texture_cache);
    this->material_cache = std::move(other.material_cache);
    this->mesh_cache = std::move(other.mesh_cache);
    return *this;
}