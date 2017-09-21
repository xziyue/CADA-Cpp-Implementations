//
// Created by xziyu on 2017/9/18.
//

#ifndef CADA_CPP_IMPLEMENTATIONS_CHESSBOARD_HPP
#define CADA_CPP_IMPLEMENTATIONS_CHESSBOARD_HPP

#include "CommonInclude.hpp"

// [row, column]
using PosPtr = std::pair<int, int>;

static constexpr int GetChessboardScale(int size) {
    return 1 << size;
}

enum class LPuzzleType : char {
    TopLeftType, TopRightType, BotLeftType, BotRightType
};

// four types of L-shaped puzzles
class LPuzzleBase {
public:
    using PosContainerType = std::array<PosPtr, 3>;

    virtual PosContainerType GetPositions() const = 0;

    virtual LPuzzleType GetType() const = 0;

    virtual ~LPuzzleBase() {}

    PosPtr centerPosition{0, 0};
};

template<LPuzzleType TypeIndicator>
class LPuzzle : public LPuzzleBase {
    LPuzzle() { ThrowInvalid(); }

    virtual PosContainerType GetPositions() const { ThrowInvalid(); }

    virtual LPuzzleType GetType() const { ThrowInvalid(); }

private:
    void ThrowInvalid() const {
        throw std::runtime_error{"invalid L-shaped puzzle type"};
    }
};

// type 0: center at top left
template<>
class LPuzzle<LPuzzleType::TopLeftType> : public LPuzzleBase {
    virtual PosContainerType GetPositions() const {
        PosContainerType ret;
        ret[0] = centerPosition;
        ret[1] = PosPtr{centerPosition.first + 1, centerPosition.second};
        ret[2] = PosPtr{centerPosition.first, centerPosition.second + 1};
        return ret;
    }

    virtual LPuzzleType GetType() const { return LPuzzleType::TopLeftType; }
};

// type 1: center at top right
template<>
class LPuzzle<LPuzzleType::TopRightType> : public LPuzzleBase {
    virtual PosContainerType GetPositions() const {
        PosContainerType ret;
        ret[0] = centerPosition;
        ret[1] = PosPtr{centerPosition.first + 1, centerPosition.second};
        ret[2] = PosPtr{centerPosition.first, centerPosition.second - 1};
        return ret;
    }

    virtual LPuzzleType GetType() const { return LPuzzleType::TopRightType; }
};

// type 2: center at bottom left
template<>
class LPuzzle<LPuzzleType::BotLeftType> : public LPuzzleBase {
    virtual PosContainerType GetPositions() const {
        PosContainerType ret;
        ret[0] = centerPosition;
        ret[1] = PosPtr{centerPosition.first - 1, centerPosition.second};
        ret[2] = PosPtr{centerPosition.first, centerPosition.second + 1};
        return ret;
    }

    virtual LPuzzleType GetType() const { return LPuzzleType::BotLeftType; }
};

// type 3: center at bottom right
template<>
class LPuzzle<LPuzzleType::BotRightType> : public LPuzzleBase {
    virtual PosContainerType GetPositions() const {
        PosContainerType ret;
        ret[0] = centerPosition;
        ret[1] = PosPtr{centerPosition.first - 1, centerPosition.second};
        ret[2] = PosPtr{centerPosition.first, centerPosition.second - 1};
        return ret;
    }

    virtual LPuzzleType GetType() const { return LPuzzleType::BotRightType; }
};


class ChessboardTessellator {
private:
    enum class ChessboardSegmentType : char {
        TopLeftSegment, TopRightSegment, BotLeftSegment, BotRightSegment
    };

    using SegmentSpanMultiplier = std::pair<int, int>;
    using PuzzleOffset = std::pair<int, int>;

    // L-shaped puzzle type, top left multiplier, puzzle center multiplier, puzzle center offset
    using SegmentInfoType = std::tuple<LPuzzleType, SegmentSpanMultiplier, SegmentSpanMultiplier, PuzzleOffset>;
    using SegmentCorrespondenceType = std::pair<SegmentSpanMultiplier, ChessboardSegmentType>;
public:
    using LPuzzlePtr = std::unique_ptr<LPuzzleBase>;

    // initialize data structures
    ChessboardTessellator() {
        segmentInfo[ChessboardSegmentType::TopLeftSegment] =
                SegmentInfoType{LPuzzleType::BotRightType, SegmentSpanMultiplier{0, 0},
                                SegmentSpanMultiplier{1, 1}, PuzzleOffset{0, 0}};

        segmentInfo[ChessboardSegmentType::TopRightSegment] =
                SegmentInfoType{LPuzzleType::BotLeftType, SegmentSpanMultiplier{0, 1},
                                SegmentSpanMultiplier{1, 0}, PuzzleOffset{0, -1}};

        segmentInfo[ChessboardSegmentType::BotLeftSegment] =
                SegmentInfoType{LPuzzleType::TopRightType, SegmentSpanMultiplier{1, 0},
                                SegmentSpanMultiplier{0, 1}, PuzzleOffset{-1, 0}};

        segmentInfo[ChessboardSegmentType::BotRightSegment] =
                SegmentInfoType{LPuzzleType::TopLeftType, SegmentSpanMultiplier{1, 1},
                                SegmentSpanMultiplier{0, 0}, PuzzleOffset{-1, -1}};
    }

    void CheckValidSpecialBlockPosition(int power, PosPtr specialBlockPos) const {
        AlwaysAssert(power > 1, "invalid chessboard size");
        // calculate the actual size of the chessboard
        auto scale = GetChessboardScale(power);
        AlwaysAssert(specialBlockPos.first >= 0 && specialBlockPos.first < scale, "invalid position");
        AlwaysAssert(specialBlockPos.second >= 0 && specialBlockPos.second < scale, "invalid position");
    }

    // the actual scale of the chessboard is 2^size
    std::vector<LPuzzlePtr> Tessellate(int power, PosPtr specialBlockPos) const {
        AlwaysAssert(power > 1, "invalid chessboard size");
        // calculate the actual size of the chessboard
        auto scale = GetChessboardScale(power);
        AlwaysAssert(specialBlockPos.first >= 0 && specialBlockPos.first < scale, "Tessellate: invalid position");
        AlwaysAssert(specialBlockPos.second >= 0 && specialBlockPos.second < scale, "Tessellate: invalid position");

        std::vector<LPuzzlePtr> container;
        Tessellate(PosPtr{0, 0}, specialBlockPos, scale, container);
        return container;
    }

private:

    std::map<ChessboardSegmentType, SegmentInfoType> segmentInfo;

    ChessboardSegmentType LocateBlock(const PosPtr &blockTopLeftPos, const PosPtr &blockPos, int segmentSpan) const {
        auto relativeX = blockPos.first - blockTopLeftPos.first;
        auto relativeY = blockPos.second - blockTopLeftPos.second;
        AlwaysAssert(relativeX >= 0 && relativeX < 2 * segmentSpan, "LocateBlock: invalid position");
        AlwaysAssert(relativeY >= 0 && relativeY < 2 * segmentSpan, "LocateBlock: invalid position");
        if (relativeX < segmentSpan) {
            if (relativeY < segmentSpan)return ChessboardSegmentType::TopLeftSegment;
            else return ChessboardSegmentType::TopRightSegment;
        } else {
            if (relativeY < segmentSpan)return ChessboardSegmentType::BotLeftSegment;
            else return ChessboardSegmentType::BotRightSegment;
        }
    }

    LPuzzlePtr LPuzzleCreator(LPuzzleType puzzleType) const {
        LPuzzleBase *rawPointer;
        switch (puzzleType) {
            case LPuzzleType::TopLeftType:
                rawPointer = new LPuzzle<LPuzzleType::TopLeftType>;
                break;
            case LPuzzleType::TopRightType:
                rawPointer = new LPuzzle<LPuzzleType::TopRightType>;
                break;
            case LPuzzleType::BotLeftType:
                rawPointer = new LPuzzle<LPuzzleType::BotLeftType>;
                break;
            case LPuzzleType::BotRightType:
                rawPointer = new LPuzzle<LPuzzleType::BotRightType>;
                break;
            default:
                throw std::runtime_error{"invalid LPuzzle type"};
        }
        LPuzzlePtr pointer{rawPointer};
        return pointer;
    }

    // Container must support emplace back operation
    template<typename Container>
    void Tessellate(PosPtr topLeftPos, PosPtr specialBlockPos, int size, Container &container) const {
        if (size == 1)return;
        auto segmentSpan = size / 2;

        // locate where the special block is
        auto specialBlockSegment = LocateBlock(topLeftPos, specialBlockPos, segmentSpan);

        const auto &info = segmentInfo.at(specialBlockSegment);
        // calculate top left position of this segment
        auto topLeftPositionX = topLeftPos.first + std::get<1>(info).first * segmentSpan;
        auto topLeftPositionY = topLeftPos.second + std::get<1>(info).second * segmentSpan;
        PosPtr topLeftPosition{topLeftPositionX, topLeftPositionY};
        // calculate L-shaped puzzle center position
        auto puzzleCenterX = topLeftPositionX + std::get<2>(info).first * segmentSpan + std::get<3>(info).first;
        auto puzzleCenterY = topLeftPositionY + std::get<2>(info).second * segmentSpan + std::get<3>(info).second;
        PosPtr puzzleCenterPosition{puzzleCenterX, puzzleCenterY};
        // create new L-shaped puzzle
        auto pointer = std::move(LPuzzleCreator(std::get<0>(info)));
        pointer->centerPosition = puzzleCenterPosition;
        // recursively tessellate the rest of the chessboard
        auto puzzlePositions = pointer->GetPositions();
        for (const auto &blockPosition:puzzlePositions) {
            auto blockSegment = LocateBlock(topLeftPos, blockPosition, segmentSpan);
            const auto &blockInfo = segmentInfo.at(blockSegment);
            auto blockTopLeftPositionX = topLeftPos.first + std::get<1>(blockInfo).first * segmentSpan;
            auto blockTopLeftPositionY = topLeftPos.second + std::get<1>(blockInfo).second * segmentSpan;
            PosPtr blockTopLeftPosition{blockTopLeftPositionX, blockTopLeftPositionY};
            Tessellate(blockTopLeftPosition, blockPosition, segmentSpan, container);
        }
        container.emplace_back(std::move(pointer));
        // tessellate this chessboard
        Tessellate(topLeftPosition, specialBlockPos, segmentSpan, container);
    }


};

#endif //CADA_CPP_IMPLEMENTATIONS_CHESSBOARD_HPP
