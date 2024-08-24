#pragma once
#include <unordered_map>
#include "rendering/texture/texture.h"
#include <string>
#include <memory>
#include "utils/signal/signal/signal.h"
#include <vector>
#include <optional>

struct TextureInfo
{
public:
    TextureInfo(std::string file_path, std::weak_ptr<Texture> texture);
    std::string file_path;
    std::weak_ptr<Texture> texture;
};

class TextureManager
{
public:
    /// @brief load and manage a new texture
    /// @param file_path the file location of the new texture
    /// @param texture_unit the GL texture unit in shaders to associate this texture with
    /// @return information for the loaded texture
    static const TextureInfo loadNewTexture(std::string file_path, Texture::TEXTURE_USECASE usecase, unsigned int texture_unit);

    /// @brief assume control of an existing texture (useful if you need more fine control over instantiation)
    /// @param file_path the path of this texture
    /// @param old_texture the texture to take ownership of
    /// @return info for the assumed texture
    static const TextureInfo assumeTexture(std::string file_path, Texture &&old_texture);

    /// @brief returns a texture that has been previously loaded from a path
    /// @param file_path the path loaded from
    /// @return an optional object that is empty if no texture is loaded with this path or the texture info for the texture
    static const std::optional<TextureInfo> getTexture(std::string file_path);

    static Signal<const TextureInfo> getOnTextureLoadedSignal();

    // delete copy constructor
    TextureManager(TextureManager const &) = delete;
    // delete copy assignment
    void operator=(TextureManager const &) = delete;

private:
    /// @brief constructor (private because singleton)
    TextureManager();

    static TextureManager &getInstance();

    /// @brief a signal that emits a pointer to a texture upon it loading
    Signal<const TextureInfo> onTextureLoaded;

    /// @brief a map of file path to texture loaded from filepath
    std::unordered_map<std::string, std::shared_ptr<Texture>> locationToTexture;
};