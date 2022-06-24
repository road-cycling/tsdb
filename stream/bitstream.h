//
// Created by Nathan Kamm on 5/28/22.
//

#ifndef TIMESERIES_DB_BITSTREAM_H
#define TIMESERIES_DB_BITSTREAM_H


#include <vector>

class BitStream {
public:
    BitStream();
    ~BitStream();

    void print();

    void WriteBits(int writeInt, int numberOfBits);
    void WriteBits(uint64_t writeInt, int numberOfBits);

    void BitReaderWrapper(int &readInt, int numberOfBits);
    void BitReader64Wrapper(uint64_t &readInt, int numberOfBits);

    void BitReader(int &readInt, int numberOfBits, int &blockOffset, int &vectorOffset);
    void BitReader64(uint64_t &readInt, int numberOfBits, int &blockOffset, int &vectorOffset);


    static void WriteToFlipPotentialNegative(int &inputInt, int numberOfBits);

    void Serialize(std::ostream &writer) const;
    void Deserialize(std::istream &reader) const;

    void Serialize(char *&writer);
    void Deserialize(char *&reader) const;

    [[nodiscard]] int SizeInBytes() const {
        return this->byteStream->size() * sizeof(uint_fast64_t);
    }

    void resetReadOffsets() {
        this->readByteStreamBlockOffset = 0;
        this->readByteStreamVectorOffset = 0;
    }

private:
    void bitReader64(uint64_t &readInt, int numberOfBits, int &blockOffset, int &vectorOffset);
    void bitReader(int &readInt, int numberOfBits, int &blockOffset, int &vectorOffset);

    void bitWriter(uint_fast64_t writeInt, int numberOfBits);

public:
    int writeByteStreamStartingOffset{0};
    std::unique_ptr<std::vector<uint_fast64_t>> byteStream{
            std::make_unique<std::vector<uint_fast64_t>>()
    };

    int readByteStreamBlockOffset{0};
    int readByteStreamVectorOffset{0};
};

#endif //TIMESERIES_DB_BITSTREAM_H
