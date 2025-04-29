#pragma once

#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <atomic>
#include <format>

#include "config.h"
#include "../util/atomic_queue.h"

template <
    template <typename, std::size_t> class BufferType, 
    typename LogType, 
    std::size_t BufferSize
>
class Logger
{

    private:

        static void writeLogLoop(BufferType<LogType, BufferSize>& logBuffer, std::atomic_flag& mainAlive)
        {
            auto logfile = std::ofstream(LOG_FILE_PATH.data());
            system(START_DEBUG_MONITOR_CMD_STR.data());

            while(true)
            {   
                if (!mainAlive.test(std::memory_order_acquire)) { break; }
                logBuffer.wait();
                        logfile << logBuffer.pop() << std::endl;
                        logfile.flush();
            }
        }

        BufferType<LogType, BufferSize> logBuffer = BufferType<LogType, BufferSize>{};
        std::atomic_flag mainAlive = std::atomic_flag{true};

    public:

        explicit Logger()
        {  
            std::thread writingThread = std::thread(writeLogLoop, std::ref(logBuffer), std::ref(mainAlive));
            writingThread.detach();
        }
        
        template<typename... Args>
        void log(std::string_view fmt, Args&&... args)
        {
            logBuffer.push(std::vformat(fmt, std::make_format_args(args...)));
        }

        ~Logger()
        {   
            mainAlive.clear(std::memory_order_release);
            system(KILL_DEBUG_MONITOR_CMD_STR.data());
        }
}; // class Logger

auto logger = Logger<AtomicQueue, std::string, 10>{};