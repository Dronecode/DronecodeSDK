// WARNING: THIS FILE IS AUTOGENERATED! As such, it should not be edited.
// Edits need to be made to the proto files
// (see https://github.com/mavlink/MAVSDK-Proto/blob/master/protos/tune/tune.proto)

#include "tune/tune.grpc.pb.h"
#include "plugins/tune/tune.h"

#include "log.h"
#include <atomic>
#include <cmath>
#include <future>
#include <limits>
#include <memory>
#include <sstream>
#include <vector>

namespace mavsdk {
namespace backend {

template<typename Tune = Tune>
class TuneServiceImpl final : public rpc::tune::TuneService::Service {
public:
    TuneServiceImpl(Tune& tune) : _tune(tune) {}

    template<typename ResponseType>
    void fillResponseWithResult(ResponseType* response, mavsdk::Tune::Result& result) const
    {
        auto rpc_result = translateToRpcResult(result);

        auto* rpc_tune_result = new rpc::tune::TuneResult();
        rpc_tune_result->set_result(rpc_result);
        std::stringstream ss;
        ss << result;
        rpc_tune_result->set_result_str(ss.str());

        response->set_allocated_tune_result(rpc_tune_result);
    }

    static rpc::tune::SongElement
    translateToRpcSongElement(const mavsdk::Tune::SongElement& song_element)
    {
        switch (song_element) {
            default:
                LogErr() << "Unknown song_element enum value: " << static_cast<int>(song_element);
            // FALLTHROUGH
            case mavsdk::Tune::SongElement::StyleLegato:
                return rpc::tune::SONG_ELEMENT_STYLE_LEGATO;
            case mavsdk::Tune::SongElement::StyleNormal:
                return rpc::tune::SONG_ELEMENT_STYLE_NORMAL;
            case mavsdk::Tune::SongElement::StyleStaccato:
                return rpc::tune::SONG_ELEMENT_STYLE_STACCATO;
            case mavsdk::Tune::SongElement::Duration1:
                return rpc::tune::SONG_ELEMENT_DURATION_1;
            case mavsdk::Tune::SongElement::Duration2:
                return rpc::tune::SONG_ELEMENT_DURATION_2;
            case mavsdk::Tune::SongElement::Duration4:
                return rpc::tune::SONG_ELEMENT_DURATION_4;
            case mavsdk::Tune::SongElement::Duration8:
                return rpc::tune::SONG_ELEMENT_DURATION_8;
            case mavsdk::Tune::SongElement::Duration16:
                return rpc::tune::SONG_ELEMENT_DURATION_16;
            case mavsdk::Tune::SongElement::Duration32:
                return rpc::tune::SONG_ELEMENT_DURATION_32;
            case mavsdk::Tune::SongElement::NoteA:
                return rpc::tune::SONG_ELEMENT_NOTE_A;
            case mavsdk::Tune::SongElement::NoteB:
                return rpc::tune::SONG_ELEMENT_NOTE_B;
            case mavsdk::Tune::SongElement::NoteC:
                return rpc::tune::SONG_ELEMENT_NOTE_C;
            case mavsdk::Tune::SongElement::NoteD:
                return rpc::tune::SONG_ELEMENT_NOTE_D;
            case mavsdk::Tune::SongElement::NoteE:
                return rpc::tune::SONG_ELEMENT_NOTE_E;
            case mavsdk::Tune::SongElement::NoteF:
                return rpc::tune::SONG_ELEMENT_NOTE_F;
            case mavsdk::Tune::SongElement::NoteG:
                return rpc::tune::SONG_ELEMENT_NOTE_G;
            case mavsdk::Tune::SongElement::NotePause:
                return rpc::tune::SONG_ELEMENT_NOTE_PAUSE;
            case mavsdk::Tune::SongElement::Sharp:
                return rpc::tune::SONG_ELEMENT_SHARP;
            case mavsdk::Tune::SongElement::Flat:
                return rpc::tune::SONG_ELEMENT_FLAT;
            case mavsdk::Tune::SongElement::OctaveUp:
                return rpc::tune::SONG_ELEMENT_OCTAVE_UP;
            case mavsdk::Tune::SongElement::OctaveDown:
                return rpc::tune::SONG_ELEMENT_OCTAVE_DOWN;
        }
    }

    static mavsdk::Tune::SongElement
    translateFromRpcSongElement(const rpc::tune::SongElement song_element)
    {
        switch (song_element) {
            default:
                LogErr() << "Unknown song_element enum value: " << static_cast<int>(song_element);
            // FALLTHROUGH
            case rpc::tune::SONG_ELEMENT_STYLE_LEGATO:
                return mavsdk::Tune::SongElement::StyleLegato;
            case rpc::tune::SONG_ELEMENT_STYLE_NORMAL:
                return mavsdk::Tune::SongElement::StyleNormal;
            case rpc::tune::SONG_ELEMENT_STYLE_STACCATO:
                return mavsdk::Tune::SongElement::StyleStaccato;
            case rpc::tune::SONG_ELEMENT_DURATION_1:
                return mavsdk::Tune::SongElement::Duration1;
            case rpc::tune::SONG_ELEMENT_DURATION_2:
                return mavsdk::Tune::SongElement::Duration2;
            case rpc::tune::SONG_ELEMENT_DURATION_4:
                return mavsdk::Tune::SongElement::Duration4;
            case rpc::tune::SONG_ELEMENT_DURATION_8:
                return mavsdk::Tune::SongElement::Duration8;
            case rpc::tune::SONG_ELEMENT_DURATION_16:
                return mavsdk::Tune::SongElement::Duration16;
            case rpc::tune::SONG_ELEMENT_DURATION_32:
                return mavsdk::Tune::SongElement::Duration32;
            case rpc::tune::SONG_ELEMENT_NOTE_A:
                return mavsdk::Tune::SongElement::NoteA;
            case rpc::tune::SONG_ELEMENT_NOTE_B:
                return mavsdk::Tune::SongElement::NoteB;
            case rpc::tune::SONG_ELEMENT_NOTE_C:
                return mavsdk::Tune::SongElement::NoteC;
            case rpc::tune::SONG_ELEMENT_NOTE_D:
                return mavsdk::Tune::SongElement::NoteD;
            case rpc::tune::SONG_ELEMENT_NOTE_E:
                return mavsdk::Tune::SongElement::NoteE;
            case rpc::tune::SONG_ELEMENT_NOTE_F:
                return mavsdk::Tune::SongElement::NoteF;
            case rpc::tune::SONG_ELEMENT_NOTE_G:
                return mavsdk::Tune::SongElement::NoteG;
            case rpc::tune::SONG_ELEMENT_NOTE_PAUSE:
                return mavsdk::Tune::SongElement::NotePause;
            case rpc::tune::SONG_ELEMENT_SHARP:
                return mavsdk::Tune::SongElement::Sharp;
            case rpc::tune::SONG_ELEMENT_FLAT:
                return mavsdk::Tune::SongElement::Flat;
            case rpc::tune::SONG_ELEMENT_OCTAVE_UP:
                return mavsdk::Tune::SongElement::OctaveUp;
            case rpc::tune::SONG_ELEMENT_OCTAVE_DOWN:
                return mavsdk::Tune::SongElement::OctaveDown;
        }
    }

    static std::unique_ptr<rpc::tune::TuneDescription>
    translateToRpcTuneDescription(const mavsdk::Tune::TuneDescription& tune_description)
    {
        std::unique_ptr<rpc::tune::TuneDescription> rpc_obj(new rpc::tune::TuneDescription());

        for (const auto& elem : tune_description.song_elements) {
            rpc_obj->add_song_elements(translateToRpcSongElement(elem));
        }

        rpc_obj->set_tempo(tune_description.tempo);

        return rpc_obj;
    }

    static mavsdk::Tune::TuneDescription
    translateFromRpcTuneDescription(const rpc::tune::TuneDescription& tune_description)
    {
        mavsdk::Tune::TuneDescription obj;

        for (const auto& elem : tune_description.song_elements()) {
            obj.song_elements.push_back(
                translateFromRpcSongElement(static_cast<mavsdk::rpc::tune::SongElement>(elem)));
        }

        obj.tempo = tune_description.tempo();

        return obj;
    }

    static rpc::tune::TuneResult::Result translateToRpcResult(const mavsdk::Tune::Result& result)
    {
        switch (result) {
            default:
                LogErr() << "Unknown result enum value: " << static_cast<int>(result);
            // FALLTHROUGH
            case mavsdk::Tune::Result::Success:
                return rpc::tune::TuneResult_Result_RESULT_SUCCESS;
            case mavsdk::Tune::Result::InvalidTempo:
                return rpc::tune::TuneResult_Result_RESULT_INVALID_TEMPO;
            case mavsdk::Tune::Result::TuneTooLong:
                return rpc::tune::TuneResult_Result_RESULT_TUNE_TOO_LONG;
            case mavsdk::Tune::Result::Error:
                return rpc::tune::TuneResult_Result_RESULT_ERROR;
        }
    }

    static mavsdk::Tune::Result translateFromRpcResult(const rpc::tune::TuneResult::Result result)
    {
        switch (result) {
            default:
                LogErr() << "Unknown result enum value: " << static_cast<int>(result);
            // FALLTHROUGH
            case rpc::tune::TuneResult_Result_RESULT_SUCCESS:
                return mavsdk::Tune::Result::Success;
            case rpc::tune::TuneResult_Result_RESULT_INVALID_TEMPO:
                return mavsdk::Tune::Result::InvalidTempo;
            case rpc::tune::TuneResult_Result_RESULT_TUNE_TOO_LONG:
                return mavsdk::Tune::Result::TuneTooLong;
            case rpc::tune::TuneResult_Result_RESULT_ERROR:
                return mavsdk::Tune::Result::Error;
        }
    }

    grpc::Status PlayTune(
        grpc::ServerContext* /* context */,
        const rpc::tune::PlayTuneRequest* request,
        rpc::tune::PlayTuneResponse* response) override
    {
        if (request == nullptr) {
            LogWarn() << "PlayTune sent with a null request! Ignoring...";
            return grpc::Status::OK;
        }

        auto result = _tune.play_tune(translateFromRpcTuneDescription(request->description()));

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    void stop()
    {
        _stopped.store(true);
        for (auto& prom : _stream_stop_promises) {
            if (auto handle = prom.lock()) {
                handle->set_value();
            }
        }
    }

private:
    void register_stream_stop_promise(std::weak_ptr<std::promise<void>> prom)
    {
        // If we have already stopped, set promise immediately and don't add it to list.
        if (_stopped.load()) {
            if (auto handle = prom.lock()) {
                handle->set_value();
            }
        } else {
            _stream_stop_promises.push_back(prom);
        }
    }

    void unregister_stream_stop_promise(std::shared_ptr<std::promise<void>> prom)
    {
        for (auto it = _stream_stop_promises.begin(); it != _stream_stop_promises.end();
             /* ++it */) {
            if (it->lock() == prom) {
                it = _stream_stop_promises.erase(it);
            } else {
                ++it;
            }
        }
    }

    Tune& _tune;
    std::atomic<bool> _stopped{false};
    std::vector<std::weak_ptr<std::promise<void>>> _stream_stop_promises{};
};

} // namespace backend
} // namespace mavsdk