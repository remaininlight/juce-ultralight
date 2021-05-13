#pragma once

#include "../../dependencies/ultralight-sdk/include/AppCore/AppCore.h"

using namespace juce;
//using namespace ultralight;

namespace juce_ultralight {

class JuceImageSurface : public ultralight::Surface {
    public:

        JuceImageSurface(uint32_t width, uint32_t height) {

            juce::Logger::writeToLog("JuceImageSurface()");
            Resize(width, height);
        }

        virtual ~JuceImageSurface() {

            juce::Logger::writeToLog("~JuceImageSurface()");
            bitmapData.reset();
            image.reset();
        }

        //juce::Logger::writeToLog("JuceImageSurface width() " + juce::String(image->getWidth()));
        virtual uint32_t width() const override { return image->getWidth(); }

        //juce::Logger::writeToLog("JuceImageSurface height() " + juce::String(image->getHeight()));
        virtual uint32_t height() const override { return image->getHeight(); }

        //juce::Logger::writeToLog("JuceImageSurface row_bytes() " + juce::String(bitmapData->lineStride));
        virtual uint32_t row_bytes() const override { return bitmapData->lineStride; }

        //juce::Logger::writeToLog("JuceImageSurface size()");
        virtual size_t size() const override { return row_bytes() * height(); }

        virtual void *LockPixels() override {

            //juce::Logger::writeToLog("JuceImageSurface *LockPixels()");
            //Image::BitmapData bitmapData(display.getImage(), Image::BitmapData::ReadWriteMode::writeOnly);
            bitmapData = std::make_unique<Image::BitmapData>(*image, Image::BitmapData::ReadWriteMode::readWrite);
            return bitmapData->data;
        }

        virtual void UnlockPixels() override {

            //juce::Logger::writeToLog("JuceImageSurface UnlockLockPixels()");
            bitmapData.reset();
        }

        virtual void Resize(uint32_t width, uint32_t height) override {

            juce::Logger::writeToLog("JuceImageSurface Resize()");
            image = std::make_unique<Image>(Image::PixelFormat::ARGB, width, height, false);
        }

        /*
        virtual GLuint GetTextureAndSyncIfNeeded() {
            ///
            /// This is NOT called by Ultralight.
            ///
            /// This helper function is called when our application wants to draw this
            /// Surface to an OpenGL quad. (We return an OpenGL texture handle)
            ///
            /// We take this opportunity to upload the PBO to the texture if the
            /// pixels have changed since the last call (indicated by dirty_bounds()
            /// being non-empty)
            ///
            if (!dirty_bounds().IsEmpty()) {
                ///
                /// Update our Texture from our PBO (pixel buffer object)
                ///
                glBindTexture(GL_TEXTURE_2D, texture_id_);
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_id_);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_,
                             0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
                glBindTexture(GL_TEXTURE_2D, 0);

                ///
                /// Clear our Surface's dirty bounds to indicate we've handled any
                /// pending modifications to our pixels.
                ///
                ClearDirtyBounds();
            }

            return texture_id_;
        }
        */

        std::unique_ptr<Image> image;
        std::unique_ptr<Image::BitmapData> bitmapData;
    };


    class JuceImageSurfaceFactory : public ultralight::SurfaceFactory {
    public:
        JuceImageSurfaceFactory() {}

        virtual ~JuceImageSurfaceFactory() {}

        virtual ultralight::Surface* CreateSurface(uint32_t width, uint32_t height) override {
            ///
            /// Called by Ultralight when it wants to create a Surface.
            ///
            return new JuceImageSurface(width, height);
        }

        virtual void DestroySurface(ultralight::Surface* surface) override {
            //
            /// Called by Ultralight when it wants to destroy a Surface.
            ///
            delete static_cast<JuceImageSurface*>(surface);
        }
    };
}
