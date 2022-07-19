//
// Created by Nathan Kamm on 6/18/22.
//

#include <locale>
#include "indexed_schema.h"

int indexed_schema::ObtainRowIndex(uint64_t metadataUID) {
    if (!this->rowIndex.contains(metadataUID)) {
        this->rowIndex[metadataUID] = this->rowIndexIncrementor++;
    }
    return this->rowIndex[metadataUID];
}

void indexed_schema::Index(ts_proto::MetricWriter *metric) {

    /*
     * What is the metadata offset in the packed storage bin?
     */

    int metadataRowOffset = this->ObtainRowIndex(metric->metadatauid());

    for (auto const& [dimensionKey, dimensionValue] : metric->dimensions()) {
        if (!this->namespacedMetricIndex.contains(dimensionKey)) {
            this->namespacedMetricIndex[dimensionKey] = std::map<std::string, roaring::Roaring>();
        }

        if (!this->namespacedMetricIndex[dimensionKey].contains(dimensionValue)) {
            this->namespacedMetricIndex[dimensionKey][dimensionValue] = roaring::Roaring();
        }

        this->namespacedMetricIndex[dimensionKey][dimensionValue].add(metadataRowOffset);


    }


}

void indexed_schema::Query(std::map<std::string, std::string> tagQuery) {

}

roaring::Roaring indexed_schema::QueryFromAST(const std::string& tagK, const std::string& tagV) {
    return this->namespacedMetricIndex[tagK][tagV];
}

