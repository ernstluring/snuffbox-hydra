#pragma once

#include "engine/assets/asset.h"

namespace snuffbox
{
  namespace engine
  {
    /**
    * @brief Used to load vertex, pixel and geometry shaders from
    *        SPIRV-Cross generated bytecode
    *
    * @see ShaderCompiler
    *
    * @author Daniel Konings
    */
    class ShaderAsset : public IAsset
    {

    public:

      /**
      * @see IAsset::IAsset
      *
      * @param[in] type The shader type, valid arguments are:
      *                 - AssetTypes::kVertexShader
      *                 - AssetTypes::kPixelShader
      *                 - AssetTypes::kGeometryShader
      */
      ShaderAsset(
        const foundation::Path& path,
        compilers::AssetTypes type);

    protected:

      /**
      * @see IAsset::LoadImpl
      */
      bool LoadImpl(const foundation::Path& path) override;

      /**
      * @see IAsset::UnloadImpl
      */
      void UnloadImpl() override;
    };
  }
}