//
// Created by Nathan Kamm on 5/28/22.
//

#ifndef TIMESERIES_DB_TIMESERIES_H
#define TIMESERIES_DB_TIMESERIES_H

#include "../stream/blockbuilder.h"

#include <mutex>
#include <cstdint>
#include <shared_mutex>

class TimeSeries {
public:

    explicit TimeSeries(int64_t metricID);
    ~TimeSeries();

    void WriteSeries(int timestamp, double dataValue);
    std::vector<std::pair<int, double>> Query(int startingTimestamp, int endpointTimestamp);


private:

    int64_t metricID;

    std::unique_ptr<BlockBuilder> openBlock;

    int storedBlocks{0};
    int blockSizeBytes{0};
    char* closedBlocks{nullptr};

    std::unique_lock<std::shared_mutex>writeLock;
    std::shared_lock<std::shared_mutex>readLock;
};

#endif //TIMESERIES_DB_TIMESERIES_H
