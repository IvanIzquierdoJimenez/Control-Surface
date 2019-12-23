#pragma once

#include <AH/Settings/Warnings.hpp>
#include <Settings/NamespaceSettings.hpp>

#include <MIDI_Parsers/MIDI_MessageTypes.hpp>

AH_DIAGNOSTIC_WERROR()

BEGIN_CS_NAMESPACE

class MIDI_Pipe;

class MIDI_Sink {
  public:
    /// Accept an incoming MIDI Channel message.
    virtual void sinkMIDI(ChannelMessage) = 0;
    /// Accept an incoming MIDI System Exclusive message.
    virtual void sinkMIDI(SysExMessage) = 0;
    /// Accept an incoming MIDI Real-Time message.
    virtual void sinkMIDI(RealTimeMessage) = 0;

    virtual ~MIDI_Sink();

  private:
    bool connectSourcePipe(MIDI_Pipe *source);
    void disconnectSourcePipe();

  protected:
    MIDI_Pipe *sourcePipe = nullptr;

    friend class MIDI_Pipe;
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

class MIDI_Source {
  public:
    /// Send a MIDI Channel Message.
    void sendMIDI(ChannelMessage);
    /// Send a MIDI System Exclusive message.
    void sendMIDI(SysExMessage);
    /// Send a MIDI Real-Time message.
    void sendMIDI(RealTimeMessage);

    virtual ~MIDI_Source();

  private:
    bool connectSinkPipe(MIDI_Pipe *sink);
    void disconnectSinkPipe();

  protected:
    MIDI_Pipe *sinkPipe = nullptr;

    friend class MIDI_Pipe;
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

class MIDI_Pipe : private MIDI_Sink, private MIDI_Source {
  public:
    void connectSink(MIDI_Sink *sink);
    void disconnectSink();
    void connectSource(MIDI_Source *source);
    void disconnectSource();

    bool hasSink() const { return sink != nullptr; }
    bool hasSource() const { return source != nullptr; }
    bool hasThroughOut() const { return throughOut != nullptr; }
    bool hasThroughIn() const { return throughIn != nullptr; }

    virtual void pipeMIDI(ChannelMessage msg) {
        if (hasThroughOut())
            throughOut->pipeMIDI(msg);
        // map(msg)
        sinkMIDI(msg);
    }
    virtual void pipeMIDI(SysExMessage msg) {
        if (hasThroughOut())
            throughOut->pipeMIDI(msg);
        // map(msg)
        sinkMIDI(msg);
    }
    virtual void pipeMIDI(RealTimeMessage msg) {
        if (hasThroughOut())
            throughOut->pipeMIDI(msg);
        // map(msg)
        sinkMIDI(msg);
    }

    /// Forward from source through to output without processing;
    void sinkMIDI(ChannelMessage msg) final override {
        if (hasSink())
            sink->sinkMIDI(msg);
    }
    /// Forward froum source through to output without processing;
    void sinkMIDI(SysExMessage msg) final override {
        if (hasSink())
            sink->sinkMIDI(msg);
    }
    /// Forward froum source through to output without processing;
    void sinkMIDI(RealTimeMessage msg) final override {
        if (hasSink())
            sink->sinkMIDI(msg);
    }

    virtual ~MIDI_Pipe();

  private:
    MIDI_Sink *sink = nullptr;
    MIDI_Source *source = nullptr;
    MIDI_Pipe *&throughOut = MIDI_Source::sinkPipe;
    MIDI_Pipe *&throughIn = MIDI_Sink::sourcePipe;
};

struct TrueMIDI_Sink : MIDI_Sink {};
struct TrueMIDI_Source : MIDI_Source {};

inline MIDI_Pipe &operator>>(MIDI_Source &source, MIDI_Pipe &pipe) {
    pipe.connectSource(&source);
    return pipe;
}

inline MIDI_Sink &operator>>(MIDI_Pipe &pipe, MIDI_Sink &sink) {
    pipe.connectSink(&sink);
    return sink;
}

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()