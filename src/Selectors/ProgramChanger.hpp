#pragma once

#include <Control_Surface/Control_Surface_Class.hpp>
#include <Def/Def.hpp>
#include <Selectors/Selectable.hpp>

BEGIN_CS_NAMESPACE

/// TODO: move to different folder?
template <setting_t N>
class ProgramChanger : public Selectable<N> {
  public:
    ProgramChanger(const Array<uint8_t, N> &programs, MIDIChannelCN channelCN)
        : programs(programs), channelCN(channelCN) {}

    void select(setting_t setting) override {
        setting = this->validateSetting(setting);
        Control_Surface.sendPC(channelCN, programs[setting]);
    }

  private:
    Array<uint8_t, N> programs;
    MIDIChannelCN channelCN;
};

END_CS_NAMESPACE