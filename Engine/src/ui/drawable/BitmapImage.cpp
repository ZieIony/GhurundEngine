#include "BitmapImage.h"
#include "resource/ResourceContext.h"
#include "ui/Graphics2D.h"

namespace Ghurund::UI {
    Status BitmapImage::loadInternal(ResourceContext& context, const DirectoryPath& workingDir, MemoryInputStream& stream, LoadOption options) {
        Status result;
        image = (Ghurund::Image*)context.ResourceManager.load(context, workingDir, stream, &result, options);
        if (filterStatus(result, options) != Status::OK)
            return result;
        return init(context, *image);
    }

    Status BitmapImage::saveInternal(ResourceContext& context, const DirectoryPath& workingDir, MemoryOutputStream& stream, SaveOption options) const {
        return context.ResourceManager.save(*image, context, workingDir, stream, options);
    }

    Status BitmapImage::init(ResourceContext& context, Ghurund::Image& image) {
        Ghurund::UI::Graphics2D& graphics2d = context.Graphics2D;
        setPointer(this->image, &image);

        D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_NONE,
            D2D1::PixelFormat(image.Format, D2D1_ALPHA_MODE_PREMULTIPLIED)
        );

        if (FAILED(graphics2d.DeviceContext->CreateBitmap(D2D1_SIZE_U{ image.Width, image.Height }, image.Data.Data, image.RowPitch, bitmapProperties, &bitmapImage)))
            return Status::CALL_FAIL;

        Valid = true;

        return Status::OK;
    }
}