//
// Created by Nathan Kamm on 5/28/22.
//

#include "bitstream.h"


#include <bitset>
#include <iostream>

template<typename POD>
std::ostream& serialize(std::ostream& os, std::vector<POD> const& v) {
    // this only works on built in data types (PODs)
    static_assert(std::is_trivial<POD>::value && std::is_standard_layout<POD>::value,
                  "Can only serialize POD types with this function");

    auto size = v.size();
    os.write(reinterpret_cast<char const*>(&size), sizeof(size));
    os.write(reinterpret_cast<char const*>(v.data()), v.size() * sizeof(POD));
    return os;
}

template<typename POD>
std::istream& deserialize(std::istream& is, std::vector<POD>& v) {
    static_assert(std::is_trivial<POD>::value && std::is_standard_layout<POD>::value,
                  "Can only deserialize POD types with this function");

    decltype(v.size()) size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    v.resize(size);
    is.read(reinterpret_cast<char*>(v.data()), v.size() * sizeof(POD));
    return is;
}

template<typename POD>
void deserialize(char *&reader, std::vector<POD>& v) {
    static_assert(std::is_trivial<POD>::value && std::is_standard_layout<POD>::value,
                  "Can only deserialize POD types with this function");

    decltype(v.size()) size;
    std::memcpy(&size, reader, sizeof(size));
    reader += sizeof(size);

    v.resize(size);
    std::memcpy(v.data(), reader, v.size() * sizeof(POD));
    reader += v.size() * sizeof(POD);

}

BitStream::BitStream(){
    this->byteStream->push_back(0);
}

BitStream::~BitStream(){}


void BitStream::BitReaderWrapper(int &readInt, int numberOfBits) {
    return this->BitReader(readInt, numberOfBits, this->readByteStreamBlockOffset, this->readByteStreamVectorOffset);
}

void BitStream::BitReader64Wrapper(uint64_t &readInt, int numberOfBits) {
    return this->BitReader64(readInt, numberOfBits, this->readByteStreamBlockOffset, this->readByteStreamVectorOffset);
}

void BitStream::print() {
    for (int i = 0; i < this->byteStream->size(); i++) {
        std::bitset<64> b(this->byteStream->at(i));
        std::cout << b << std::endl;
    }
}

void BitStream::bitWriter(uint_fast64_t writeInt, int numberOfBits) {
    this->byteStream->back() |= (writeInt << this->writeByteStreamStartingOffset);
    this->writeByteStreamStartingOffset += numberOfBits;
}

void BitStream::WriteBits(uint64_t writeInt, int numberOfBits) {

    int bitsOnStreamBlock = sizeof(uint_fast64_t) * 8;
    int bitsAvailableInCurrentValue = bitsOnStreamBlock - this->writeByteStreamStartingOffset;
    bool streamOverflow = numberOfBits > bitsAvailableInCurrentValue;

    if (numberOfBits > bitsAvailableInCurrentValue) {

        int bitsThatCanBeWrittenToThisBlock = bitsAvailableInCurrentValue;
        int bitsToWriteToNextBlock = numberOfBits - bitsAvailableInCurrentValue;

        uint_fast64_t writeIntCopyForNextBlock = writeInt;
        uint_fast64_t writeIntCopy = (uint_fast64_t) writeInt;

        int thisBlockBitShift = bitsOnStreamBlock - bitsThatCanBeWrittenToThisBlock;
        writeIntCopy <<= thisBlockBitShift;
        writeIntCopy >>= thisBlockBitShift;

        writeIntCopyForNextBlock >>= bitsThatCanBeWrittenToThisBlock;

        this->bitWriter(writeIntCopy, bitsThatCanBeWrittenToThisBlock);
        this->byteStream->push_back(0);
        this->writeByteStreamStartingOffset = 0;
        this->bitWriter(writeIntCopyForNextBlock, bitsToWriteToNextBlock);

        return;
    }


    this->bitWriter(writeInt, numberOfBits);

    if (this->writeByteStreamStartingOffset == 64) {
        this->byteStream->push_back(0);
        this->writeByteStreamStartingOffset = 0;
    }

}


void BitStream::WriteBits(int writeInt, int numberOfBits) {

    int bitsOnStreamBlock = sizeof(uint_fast64_t) * 8;
    int bitsAvailableInCurrentValue = bitsOnStreamBlock - this->writeByteStreamStartingOffset;
    bool streamOverflow = numberOfBits > bitsAvailableInCurrentValue;

    if (numberOfBits > bitsAvailableInCurrentValue) {

        int bitsThatCanBeWrittenToThisBlock = bitsAvailableInCurrentValue;
        int bitsToWriteToNextBlock = numberOfBits - bitsAvailableInCurrentValue;

        uint_fast64_t writeIntCopyForNextBlock = writeInt;
        uint_fast64_t writeIntCopy = (uint_fast64_t) writeInt;

        int thisBlockBitShift = bitsOnStreamBlock - bitsThatCanBeWrittenToThisBlock;
        writeIntCopy <<= thisBlockBitShift;
        writeIntCopy >>= thisBlockBitShift;

        writeIntCopyForNextBlock >>= bitsThatCanBeWrittenToThisBlock;

        this->bitWriter(writeIntCopy, bitsThatCanBeWrittenToThisBlock);
        this->byteStream->push_back(0);
        this->writeByteStreamStartingOffset = 0;
        this->bitWriter(writeIntCopyForNextBlock, bitsToWriteToNextBlock);

        return;
    }


    this->bitWriter(writeInt, numberOfBits);

    if (this->writeByteStreamStartingOffset == 64) {
        this->byteStream->push_back(0);
        this->writeByteStreamStartingOffset = 0;
    }

}

void BitStream::bitReader(int &readInt, int numberOfBits, int &blockOffset, int &vectorOffset) {

    int bitsOnStreamBlock = sizeof(uint_fast64_t) * 8;

    uint_fast64_t extractor = 0;
    extractor |= this->byteStream->at(vectorOffset);

    extractor <<= (bitsOnStreamBlock - (blockOffset + numberOfBits));
    extractor >>= (bitsOnStreamBlock - numberOfBits);


    readInt = extractor;
    blockOffset += numberOfBits;
    if (blockOffset == 64) {
        blockOffset = 0;
        vectorOffset += 1;
    }
}

void BitStream::bitReader64(uint64_t &readInt, int numberOfBits, int &blockOffset, int &vectorOffset) {

    int bitsOnStreamBlock = sizeof(uint_fast64_t) * 8;

    uint_fast64_t extractor = 0;
    extractor |= this->byteStream->at(vectorOffset);

    extractor <<= (bitsOnStreamBlock - (blockOffset + numberOfBits));
    extractor >>= (bitsOnStreamBlock - numberOfBits);


    readInt = extractor;
    blockOffset += numberOfBits;
    if (blockOffset == 64) {
        blockOffset = 0;
        vectorOffset += 1;
    }
}

void BitStream::BitReader(int &readInt, int numberOfBits, int &blockOffset, int &vectorOffset) {

    int bitsOnStreamBlock = sizeof(uint_fast64_t) * 8;
    int bitsAvailableFromCurrentStream = bitsOnStreamBlock - blockOffset;

    if (bitsAvailableFromCurrentStream >= numberOfBits) {
        return this->bitReader(readInt, numberOfBits, blockOffset, vectorOffset);
    }

    int numberOfBitsFirstRead = bitsAvailableFromCurrentStream;
    int numberOfBitsSecondRead = numberOfBits - numberOfBitsFirstRead;

    int firstReadInt = 0;
    this->bitReader(firstReadInt, numberOfBitsFirstRead, blockOffset, vectorOffset);
    this->bitReader(readInt, numberOfBitsSecondRead, blockOffset, vectorOffset);

    readInt <<= numberOfBitsFirstRead;
    readInt |= firstReadInt;
}

void BitStream::BitReader64(uint64_t &readInt, int numberOfBits, int &blockOffset, int &vectorOffset) {

    int bitsOnStreamBlock = sizeof(uint_fast64_t) * 8;
    int bitsAvailableFromCurrentStream = bitsOnStreamBlock - blockOffset;

    if (bitsAvailableFromCurrentStream >= numberOfBits) {
        return this->bitReader64(readInt, numberOfBits, blockOffset, vectorOffset);
    }

    int numberOfBitsFirstRead = bitsAvailableFromCurrentStream;
    int numberOfBitsSecondRead = numberOfBits - numberOfBitsFirstRead;

    uint64_t firstReadInt = 0;
    this->bitReader64(firstReadInt, numberOfBitsFirstRead, blockOffset, vectorOffset);
    this->bitReader64(readInt, numberOfBitsSecondRead, blockOffset, vectorOffset);

    readInt <<= numberOfBitsFirstRead;
    readInt |= firstReadInt;
}

void BitStream::Serialize(std::ostream &writer) const {
    serialize(writer, *this->byteStream);
}

void BitStream::Serialize(char *&writer) {

    uint_fast64_t size = this->byteStream->size();

    std::memcpy(writer, &size, sizeof(size));
    writer += sizeof(size);

    std::memcpy(writer, this->byteStream->data(), size * sizeof(uint_fast64_t));
    writer += (size * sizeof(uint_fast64_t));
}

void BitStream::Deserialize(std::istream &reader) const {
    deserialize(reader, *this->byteStream);
}

void BitStream::Deserialize(char *&reader) const {
    deserialize(reader, *this->byteStream);
}




