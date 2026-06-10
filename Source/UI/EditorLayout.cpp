#include "EditorLayout.h"

namespace svc::ui::layout
{

int bottomSectionsTotalHeight (const SectionStackHeights& sections) noexcept
{
    return sections.histogram + sections.midiTools + sections.calibration + kBottomSectionsTrailingPad;
}

EditorLayoutResult computeEditorLayout (const EditorLayoutInputs& inputs) noexcept
{
    EditorLayoutResult result;
    auto area = inputs.editorBounds;
    area.removeFromTop (kHeaderHeight);

    auto bounds = area.reduced (kOuterPadding).withTrimmedBottom (kStatusBarTrim);

    if (inputs.hasStandaloneMidiPanel)
    {
        bounds.removeFromTop (kStandalonePanelHeight);
        bounds.removeFromTop (kStandaloneGap);
    }

    bounds.removeFromTop (kToolbarHeight);
    bounds.removeFromTop (kLiveHitsRowHeight);

    result.bottomSectionsTotal = bottomSectionsTotalHeight (inputs.bottomSections);
    bounds.removeFromBottom (result.bottomSectionsTotal);

    const int padColWidth = juce::jlimit (220, 300, bounds.getWidth() / 4);
    result.padGridBounds = bounds.removeFromLeft (padColWidth).reduced (4);

    const int settingsWidth = inputs.padSettingsExpanded
                                  ? juce::jlimit (220, 280, bounds.getWidth() / 3)
                                  : 30;
    result.padSettingsBounds = bounds.removeFromRight (settingsWidth).reduced (4);
    result.curveEditorBounds = bounds.reduced (4);
    return result;
}

} // namespace svc::ui::layout
