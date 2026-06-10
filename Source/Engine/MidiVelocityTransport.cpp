#include "MidiVelocityTransport.h"

namespace svc
{

void appendMidi2NoteOnUmp (std::vector<std::uint32_t>& storage,
                           int channel,
                           int note,
                           int midi2Velocity) noexcept
{
    using Factory = juce::universal_midi_packets::Factory;
    const auto packet = Factory::makeNoteOnV2 (0,
                                               static_cast<uint8_t> (std::clamp (channel, 1, 16) - 1),
                                               static_cast<uint8_t> (std::clamp (note, 0, 127)),
                                               Factory::NoteAttributeKind::none,
                                               static_cast<uint16_t> (std::clamp (midi2Velocity, 0, midi2Max)),
                                               0);

    storage.push_back (packet[0]);
    storage.push_back (packet[1]);
}

int readMidi2VelocityFromUmpWords (const std::vector<std::uint32_t>& storage,
                                   std::size_t wordOffset) noexcept
{
    if (storage.size() < wordOffset + 2)
        return 0;

    const juce::universal_midi_packets::PacketX2 packet { storage[wordOffset], storage[wordOffset + 1] };
    return static_cast<int> (packet.getU16<2>());
}

} // namespace svc
