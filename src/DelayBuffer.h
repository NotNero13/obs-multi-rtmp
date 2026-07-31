#pragma once

#include <obs.h>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>

struct DelayedPacket {
    obs_encoder_packet packet;
    uint64_t send_time_ns;
};

class DelayBuffer {
public:
    DelayBuffer(uint32_t delay_seconds, obs_output_t* output);
    ~DelayBuffer();

    void PushPacket(obs_encoder_packet* packet);

private:
    void WorkerLoop();

    std::queue<DelayedPacket> packet_queue;
    std::mutex queue_mutex;
    std::thread worker_thread;
    std::atomic<bool> active;
    
    uint64_t delay_ns;
    obs_output_t* target_output;
};