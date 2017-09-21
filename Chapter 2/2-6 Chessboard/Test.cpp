//
// Created by xziyu on 2017/9/18.
//

#include "Chessboard.hpp"
#include "bitmap_image.hpp"

using byte = unsigned char;

constexpr int pixelPerBlock = 15;
constexpr int linewidth = 1;
static_assert(pixelPerBlock > 0 && linewidth > 0 &&
              pixelPerBlock > 2 * linewidth, "invalid settings");

const std::array<byte, 3> lineColor{0, 0, 255};
const std::vector<std::array<byte, 3>> puzzleColors{
        {0,   255, 255},
        {255, 0,   255},
        {255, 255, 0},
        {0, 255, 0}
};
const std::array<byte, 3> specialBlockColor{0, 0, 0};

void DrawBlock(bitmap_image &image, const PosPtr &blockPos, const std::array<byte, 3> &color) {
    int blockTopLeftX = blockPos.second * pixelPerBlock;
    int blockTopLeftY = blockPos.first * pixelPerBlock;
    for (auto i = blockTopLeftX + linewidth; i < blockTopLeftX + pixelPerBlock - linewidth; ++i)
        for (auto j = blockTopLeftY + linewidth; j < blockTopLeftY + pixelPerBlock - linewidth; ++j)
            image.set_pixel(i, j, color[0], color[1], color[2]);
}

void DrawBlock(bitmap_image &image,
               int topLeftX, int topLeftY, int botRightX, int botRightY,
               const std::array<byte, 3> &color) {
    for (auto i = topLeftX; i < botRightX; ++i)
        for (auto j = topLeftY; j < botRightY; ++j)
            image.set_pixel(i, j, color[0], color[1], color[2]);
}

void DrawBlock(bitmap_image &image, const ChessboardTessellator::LPuzzlePtr &puzzle) {
    auto puzzleType = static_cast<int>(puzzle->GetType());
    const auto &color = puzzleColors[puzzleType];
    PosPtr twoBlockLeftPos = puzzle->centerPosition;

    if (puzzleType == 1 || puzzleType == 3)twoBlockLeftPos.second--;
    auto twoBlockLeftPosX = twoBlockLeftPos.second * pixelPerBlock;
    auto twoBlockLeftPosY = twoBlockLeftPos.first * pixelPerBlock;

    // draw two consecutive blocks
    for (auto i = twoBlockLeftPosX + linewidth;
         i < twoBlockLeftPosX +  2 * pixelPerBlock - linewidth; ++i)
        for (auto j = twoBlockLeftPosY + linewidth;
             j < twoBlockLeftPosY + pixelPerBlock - linewidth; ++j)
            image.set_pixel(i, j, color[0], color[1], color[2]);


    // draw one extended block
    int alignX, alignY;
    if (puzzleType == 1 || puzzleType == 3) {
        alignX = puzzle->centerPosition.second * pixelPerBlock;
        alignY = puzzle->centerPosition.first * pixelPerBlock;
    } else {
        alignX = twoBlockLeftPosX;
        alignY = twoBlockLeftPosY;
    }
    if (puzzleType <= 1) {

        DrawBlock(image, alignX + linewidth, alignY + pixelPerBlock - linewidth,
                  alignX + pixelPerBlock - linewidth, alignY + 2 * pixelPerBlock - linewidth,
                  color);

    } else {
        DrawBlock(image, alignX + linewidth, alignY - pixelPerBlock + linewidth,
                  alignX + pixelPerBlock - linewidth, alignY + linewidth, color);
    }

}

template<typename Container>
void ShowChessboard(const std::string &filename, int size,
                    const Container &container, const PosPtr &specialBlockPosition) {
    // calculate the size of the image
    int scale = GetChessboardScale(size);
    int pixelPerSide = scale * pixelPerBlock;
    bitmap_image image(pixelPerSide, pixelPerSide);
    image.set_all_channels(lineColor[0], lineColor[1], lineColor[2]);

    // draw blocks
    for (const auto &puzzle:container)DrawBlock(image, puzzle);
    DrawBlock(image, specialBlockPosition, specialBlockColor);

    // save image
    image.save_image(filename);
}

int main() {
    ChessboardTessellator tessellator;
    int size, specialBlockX, specialBlockY;
    PosPtr specialBlockPos;
    while (true) {
        std::cout << "please enter the size of the chessboard(actual scale is 2^size):\n";
        std::cin >> size;
        if (size <= 1) {
            std::cout << "invalid size, please try again\n";
            continue;
        }
        std::cout << "please enter the special block position, delimited by a white space\n";
        std::cin >> specialBlockX >> specialBlockY;
        specialBlockPos = PosPtr{specialBlockX, specialBlockY};
        try {
            tessellator.CheckValidSpecialBlockPosition(size, specialBlockPos);
            break;
        }
        catch (std::exception &e) {
            std::cout << "invalid special block position, please try again\n";
        }
    }

    auto result = std::move(tessellator.Tessellate(size, specialBlockPos));

    // check whether the size is equal to the theoretical size
    unsigned int theoreticalSize = 1;
    theoreticalSize <<= (2 * size);
    theoreticalSize = (theoreticalSize - 1) / 3;
    AlwaysAssert(result.size() == theoreticalSize, "size mismatch");

    std::cout << result.size() << " L-shaped puzzles are needed to tessellate the chessboard.\n";
    ShowChessboard("result.bmp", size, result, specialBlockPos);
    std::cout << "result saved to result.bmp\n";

    return 0;
}