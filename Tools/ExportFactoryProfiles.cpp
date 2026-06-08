#include "../Source/Profiles/ControllerProfile.h"
#include <JuceHeader.h>
#include <iostream>

namespace
{
bool exportProfile (const svc::ControllerProfile& profile, const juce::File& file)
{
    const auto tree = profile.toValueTree();
    if (auto xml = tree.createXml())
    {
        file.getParentDirectory().createDirectory();
        return xml->writeTo (file);
    }
    return false;
}
} // namespace

int main()
{
    const juce::File outDir = juce::File::getCurrentWorkingDirectory()
                                  .getChildFile ("Resources")
                                  .getChildFile ("FactoryProfiles");

    struct Entry { const char* filename; svc::ControllerProfile (*factory)(); };
    const Entry entries[] = {
        { "GM-Standard.svcp", svc::ControllerProfile::createGMStandard },
        { "Blank-Custom.svcp", svc::ControllerProfile::createBlank },
        { "Launchpad-Drum-Rack.svcp", svc::ControllerProfile::createLaunchpadDrumRack },
        { "Maschine-Group.svcp", svc::ControllerProfile::createMaschineGroup },
        { "SPD-SX.svcp", svc::ControllerProfile::createSpdSx },
        { "Yamaha-FGDP.svcp", svc::ControllerProfile::createFgdp }
    };

    for (const auto& entry : entries)
    {
        const auto profile = entry.factory();
        const auto file = outDir.getChildFile (entry.filename);
        if (! exportProfile (profile, file))
        {
            std::cerr << "Failed to export " << entry.filename << '\n';
            return 1;
        }
        std::cout << "Exported " << file.getFullPathName() << '\n';
    }

    return 0;
}
