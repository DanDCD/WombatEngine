#include "rendering/texture/texture_manager.h"

TextureInfo::TextureInfo(std::string file_path, std::weak_ptr<Texture> texture)
    : file_path(file_path), texture(texture)
{
}

const TextureInfo TextureManager::loadNewTexture(std::string file_path, unsigned int texture_unit)
{
    // if this path already has a texture, skip loading and return original texture info
    if (auto queried_info = getInstance().getTexture(file_path); queried_info.has_value())
        return queried_info.value();

    // load and manage texture
    getInstance().locationToTexture[file_path] = std::make_shared<Texture>(
        GL_TEXTURE_2D,
        std::vector<TextureParam>{
            TextureParam(GL_TEXTURE_WRAP_S, GL_REPEAT),
            TextureParam(GL_TEXTURE_WRAP_T, GL_REPEAT),
            TextureParam(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR),
            TextureParam(GL_TEXTURE_MAG_FILTER, GL_LINEAR)},
        file_path,
        GL_TEXTURE0 + texture_unit);

    // build and output info struct
    TextureInfo textureInfo(file_path, std::weak_ptr(getInstance().locationToTexture[file_path]));
    getInstance().onTextureLoaded.emit(textureInfo);

    return textureInfo;
}

const TextureInfo TextureManager::assumeTexture(std::string file_path, Texture &&old_texture)
{
    // load and manage texture
    getInstance().locationToTexture[file_path] = std::make_shared<Texture>(std::move(old_texture));

    // build and output info struct
    TextureInfo textureInfo(file_path, std::weak_ptr(getInstance().locationToTexture[file_path]));
    getInstance().onTextureLoaded.emit(textureInfo);

    return textureInfo;
}

const std::optional<TextureInfo> TextureManager::getTexture(std::string file_path)
{
    auto search = getInstance().locationToTexture.find(file_path);

    if (search != getInstance().locationToTexture.end())
        return TextureInfo(search->first, std::weak_ptr(search->second));

    return std::nullopt;
}

Signal<const TextureInfo> TextureManager::getOnTextureLoadedSignal()
{
    return getInstance().onTextureLoaded;
}

TextureManager &TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

TextureManager::TextureManager()
    : locationToTexture(), onTextureLoaded()
{
}
