// Implementation for Checkpoint
#include "checkpoint.h"
#include <fstream>
#include <vector>
#include <iostream> // For error messages

const size_t ZSTD_BUFFER_SIZE = ZSTD_CStreamInSize();

namespace Checkpoint {

bool saveState(const std::string& filePath /*, const GridData& grid */) {
    std::cout << "Attempting to save state to: " << filePath << std::endl;
    // TODO: Actual grid data needs to be passed and serialized.
    std::vector<float> dummyData(1024 * 1024, 1.2345f);
    const void* dataToCompress = dummyData.data();
    size_t dataSize = dummyData.size() * sizeof(float);

    std::ofstream outFile(filePath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error: Could not open file for writing: " << filePath << std::endl;
        return false;
    }

    ZSTD_CStream* cStream = ZSTD_createCStream();
    if (!cStream) {
        std::cerr << "Error: ZSTD_createCStream() failed." << std::endl;
        return false;
    }

    int compressionLevel = 1;
    size_t initResult = ZSTD_initCStream(cStream, compressionLevel);
    if (ZSTD_isError(initResult)) {
        std::cerr << "Error: ZSTD_initCStream() failed: " << ZSTD_getErrorName(initResult) << std::endl;
        ZSTD_freeCStream(cStream);
        return false;
    }

    std::vector<char> outBuffer(ZSTD_CStreamOutSize());
    ZSTD_inBuffer inBuf = { dataToCompress, dataSize, 0 };
    bool success = true;

    while (inBuf.pos < inBuf.size) {
        ZSTD_outBuffer outBuf = { outBuffer.data(), outBuffer.size(), 0 };
        size_t toRead = std::min(ZSTD_BUFFER_SIZE, inBuf.size - inBuf.pos);

        inBuf.size = dataSize;

        size_t remaining = ZSTD_compressStream(cStream, &outBuf, &inBuf);
        if (ZSTD_isError(remaining)) {
            std::cerr << "Error: ZSTD_compressStream() failed: " << ZSTD_getErrorName(remaining) << std::endl;
            success = false;
            break;
        }
        outFile.write(outBuffer.data(), outBuf.pos);
        if (!outFile) {
            std::cerr << "Error: Failed to write compressed data to file." << std::endl;
            success = false;
            break;
        }
    }

    if (success) {
        ZSTD_outBuffer outBuf = { outBuffer.data(), outBuffer.size(), 0 };
        size_t remaining = ZSTD_endStream(cStream, &outBuf);
        if (ZSTD_isError(remaining)) {
            std::cerr << "Error: ZSTD_endStream() failed: " << ZSTD_getErrorName(remaining) << std::endl;
            success = false;
        } else {
            outFile.write(outBuffer.data(), outBuf.pos);
            if (!outFile) {
                 std::cerr << "Error: Failed to write final compressed data to file." << std::endl;
                 success = false;
            }
        }
    }

    ZSTD_freeCStream(cStream);
    outFile.close();

    if (success) {
        std::cout << "State saved successfully (dummy data)." << std::endl;
    } else {
        std::cerr << "Failed to save state." << std::endl;
        // std::remove(filePath.c_str());
    }
    return success;
}

bool loadState(const std::string& filePath /*, GridData& grid */) {
    std::cout << "Attempting to load state from: " << filePath << std::endl;
    std::ifstream inFile(filePath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error: Could not open file for reading: " << filePath << std::endl;
        return false;
    }

    ZSTD_DStream* dStream = ZSTD_createDStream();
    if (!dStream) {
        std::cerr << "Error: ZSTD_createDStream() failed." << std::endl;
        return false;
    }

    size_t initResult = ZSTD_initDStream(dStream);
    if (ZSTD_isError(initResult)) {
        std::cerr << "Error: ZSTD_initDStream() failed: " << ZSTD_getErrorName(initResult) << std::endl;
        ZSTD_freeDStream(dStream);
        return false;
    }

    std::vector<char> inBuffer(ZSTD_DStreamInSize());
    std::vector<char> outBuffer(ZSTD_DStreamOutSize());
    
    // TODO: This vector will hold the decompressed data. Its size needs to be known or dynamically grown.
    std::vector<char> decompressedData;
    // size_t expectedDecompressedSize = ...;
    // decompressedData.reserve(expectedDecompressedSize);

    size_t totalRead = 0;
    bool success = true;

    while (true) {
        inFile.read(inBuffer.data(), ZSTD_DStreamInSize());
        size_t bytesRead = inFile.gcount();
        if (bytesRead == 0 && inFile.eof()) {
            break;
        }
        if (inFile.fail() && !inFile.eof()){
            std::cerr << "Error: Failed to read from file." << std::endl;
            success = false;
            break;
        }

        ZSTD_inBuffer zInBuf = { inBuffer.data(), bytesRead, 0 };

        while (zInBuf.pos < zInBuf.size) {
            ZSTD_outBuffer zOutBuf = { outBuffer.data(), outBuffer.size(), 0 };
            size_t ret = ZSTD_decompressStream(dStream, &zOutBuf, &zInBuf);

            if (ZSTD_isError(ret)) {
                std::cerr << "Error: ZSTD_decompressStream() failed: " << ZSTD_getErrorName(ret) << std::endl;
                success = false;
                break;
            }
            decompressedData.insert(decompressedData.end(), outBuffer.data(), outBuffer.data() + zOutBuf.pos);
            totalRead += zOutBuf.pos;

            if (ret == 0) {
                goto end_decompression_loop;
            }
        }
        if (!success) break;
    }

end_decompression_loop:
    ZSTD_freeDStream(dStream);
    inFile.close();

    if (success && totalRead > 0) {
        std::cout << "State loaded successfully. Total decompressed bytes: " << totalRead << std::endl;
        // TODO: Populate GridData from decompressedData
        // Example: if (totalRead == expectedDecompressedSize) { /* copy to grid */ }
    } else if (success && totalRead == 0 && !decompressedData.empty()) {
        std::cout << "State loaded, but decompressed data is empty or ZSTD_decompressStream indicated completion early." << std::endl;
    } else if (!success) {
        std::cerr << "Failed to load state." << std::endl;
    }
    return success;
}

} // namespace Checkpoint
