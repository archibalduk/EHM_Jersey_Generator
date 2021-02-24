#ifndef DIMENSIONS_H
#define DIMENSIONS_H

// --- Dimensions of UI elements --- //
namespace Dimensions {
enum ENUM_JERSEY_DIMENSIONS {
    // Font
    FontLetterSpacing = 1,
    // Image
    JerseyImageWidth = 184,
    JerseyImageHeight = 200,
    JerseyImageVerticalPadding = 10,
    // Pen
    PenThickness = 1,
    // Text
    JerseyNameFontSize = 18,
    JerseyNameVerticalPosition = 40,
    JerseyNumberFontSize = 80,
    JerseyNumberVerticalPosition = 70,
    // Upscaled jersey name text
    JerseyNameUpscaleMultiplier = 5,
    JerseyNameUpscaledFontSize = JerseyNameFontSize * JerseyNameUpscaleMultiplier,
    JerseyNameUpscaledWidth = JerseyImageWidth * JerseyNameUpscaleMultiplier,
    JerseyNameUpscaledHeight = JerseyNameUpscaledFontSize,
};

enum ENUM_USER_INTERFACE_DIMENSIONS {
    // Preview label
    PreviewWidth = 200,
    PreviewHeight = JerseyImageHeight,
    // Widget panel
    WidgetPanelCount = 2,
    WidgetPanelWidth = 200,
    // Application window
    WindowPadding = 10,
    WindowWidth = PreviewWidth + (WidgetPanelWidth * WidgetPanelCount)
                  + (WidgetPanelCount * WindowPadding),
    WindowHeight = (WindowWidth / 16) * 9
};
} // namespace Dimensions

#endif // DIMENSIONS_H
