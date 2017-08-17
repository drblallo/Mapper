#include "whitemaptoprovincemap.h"
#include <vector>

using namespace mapreader;

//initializes the data that are usefull in the whole splitting process.
WhiteMapToProvinceMap::WhiteMapToProvinceMap(QImage& out, int pc, StringNotifiable* noto) : info(out), provinceCount(pc), notif(noto)
{
    QColor v(QColor(255, 255, 255));
    white = v.rgb();
    QColor b(QColor(0, 0, 0));
    black = b.rgb();
    QColor t(QColor(0, 0, 255));
    blue = t.rgb();
    createRegionMapFromWhiteMap();
}

//turns every pixel that is not black into white pixels.
//returns the number of turned pixels.
int WhiteMapToProvinceMap::makeBlackAndWhite()
{
    int whiteCount(0);
    for (int a = 0; a < info.width; a++)
    {
        for (int b = 0; b < info.height; b++)
        {
            if (black != getColor(a, b))
            {
                setColor(a, b, white);
                whiteCount++;
            }
        }
    }
    return whiteCount;
}

//turns the edge pixels into black pixel
//return the number of pixels turned
int WhiteMapToProvinceMap::makeBlackEdges()
{
    int whiteCount(0);

    for (int a = 0; a < info.height; a++)
    {
        if (getColor(0, a) != black)
        {
            setColor(0, a, black);
            whiteCount++;
        }
        if (getColor(info.width-1, a) != black)
        {
            setColor(info.width-1, a, black);
            whiteCount++;
        }
    }
    for (int a = 0; a < info.width; a++)
    {
        if (getColor(a, 0) != black)
        {
            setColor(a, 0, black);
            whiteCount++;
        }
        if (getColor(a, info.height - 1) != black)
        {
            setColor(a, info.height - 1, black);
            whiteCount++;
        }
    }

    return whiteCount;
}

//changes the color of of every area made of black pixel that is touching the border
//into blue pixel. Blue is never used if not to indicate areas that are not to be touched ever again.
void WhiteMapToProvinceMap::changeColorOfAreasTouchingBorders()
{
    for (int a = 0; a < info.height; a++)
    {
        changeColorOfArea(0, a, black, blue);
        changeColorOfArea(info.width - 1, a, black, blue);
    }
    for (int a = 0; a < info.width; a++)
    {
        changeColorOfArea(a, 0, black, blue);
        changeColorOfArea(a, info.height - 1, black, blue);
    }

}

//generate random pixel into the white area to be set as area centers
//returns the number of pixel turned
int WhiteMapToProvinceMap::makeProvinceCenters()
{
    int whiteCount(0);
    for (int a = 0; a < provinceCount; a++)
    {
        std::pair<int, int> pos = tryPlaceProvince(provinceCenters.size() + 1);
        if (pos.first != -1)
        {
            provinceCenters.push_back(pos);
            whiteCount++;
        }
    }
    return whiteCount;
}

//searches for black areas that are surrounded by a only one province
//those areas are then removed, since they would be displayed wrongly on the screen.
void WhiteMapToProvinceMap::removeFullyInscribedBlackAreas()
{
    for (int a = 0; a < info.width; a++)
    {
        for (int b = 0; b < info.height; b++)
        {
            if (getColor(a, b) == black)
                 removeFullyInscribedBlackArea(a, b, black);
        }
    }
}

void WhiteMapToProvinceMap::createRegionMapFromWhiteMap()
{
    if (notif)
        notif->setNotifyText(QString("eliminating not black space"));
    int whiteCount(makeBlackAndWhite()); //turns the map to black and white
    if (notif)
        notif->setNotifyText(QString("creating edges"));
    whiteCount -= makeBlackEdges(); //makes the edges black
    if (notif)
        notif->setNotifyText(QString("detecting black areas touching borders"));
    changeColorOfAreasTouchingBorders(); //set the black areas that are connected to the broder to blue
    if (notif)
        notif->setNotifyText(QString("creating province origin"));
    whiteCount -= makeProvinceCenters(); //generate random provinces center
    if (provinceCenters.size() == 0)
        return;
    if (notif)
        notif->setNotifyText(QString("expanding provinces, it may take a long time"));

    //expand the province centers until the have take all the aviable white pixels.
    int size(1);
    while (whiteCount > 0 && size < 1000)
    {
        for (unsigned a = 0; a < provinceCenters.size(); a++)
        {
            std::pair<int, int> p = provinceCenters[a];
            whiteCount -= enlarge(p.first, p.second, size, getColor(p.first, p.second));
        }
            size++;
    }

    if (notif)
        notif->setNotifyText(QString("removing black areas inside a single region"));
    removeFullyInscribedBlackAreas(); //remove the black areas inside only one province
    if (notif)
        notif->setNotifyText(QString("eliminating useless areas"));

    //set the blue pixel to black
    for (int a = 0; a < info.width; a++)
        for (int b = 0; b < info.height; b++)
        {
            if (getColor(a, b) == blue)
                setColor(a, b, black);
        }

    finalFix();

}

void WhiteMapToProvinceMap::finalFix()
{

    QImage supp(info.width, info.height, info.image.format());
    std::vector<QRgb*> suppLines(info.height);
    for (int a = 0; a < supp.height(); a++)
        suppLines[a] = (QRgb*)supp.scanLine(a);

    for (int a = 0; a < info.width; a++)
        for (int b = 0; b < info.height; b++)
            suppLines[b][a] = white;

    if (notif)
        notif->setNotifyText(QString("eliminating provinces inside other provinces"));

    //remove every sub province that is inside a single other province
    for (int a = 0; a < info.width; a++)
    {
        for (int b = 0; b < info.height; b++)
        {
            if (getColor(a, b) != black && suppLines[b][a] != black)
                 removeFullyIscribedArea(a, b, black, suppLines);
        }
    }
}

//set the a circle of target color with center x0, y0
int WhiteMapToProvinceMap::enlarge(int x0, int y0, int radius, QRgb target)
{
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);
    int whiteCount(0);
    while (x >= y)
    {
        putpixel(x0 + x, y0 + y, &whiteCount, target);
        putpixel(x0 + y, y0 + x, &whiteCount, target);
        putpixel(x0 - y, y0 + x, &whiteCount, target);
        putpixel(x0 - x, y0 + y, &whiteCount, target);
        putpixel(x0 - x, y0 - y, &whiteCount, target);
        putpixel(x0 - y, y0 - x, &whiteCount, target);
        putpixel(x0 + y, y0 - x, &whiteCount, target);
        putpixel(x0 + x, y0 - y, &whiteCount, target);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        else //(err > 0)
        {
            x--;
            dx += 2;
            err += (-radius << 1) + dx;
        }
    }
    return whiteCount;
}

//since there can be only 1000 province, this never overlaps.
QRgb indexToCol(int index)
{
    int red(index/100);
    red = red * 25;
    index = index % 100;
    int green(index/10);
    green = green * 25;
    index = index %10;
    return QColor(red, green, index*25).rgb();
}


std::pair<int, int> WhiteMapToProvinceMap::tryPlaceProvince(int index)
{
    std::uniform_int_distribution<int> distribution(0, info.width-1);
    std::uniform_int_distribution<int> distributionY(0, info.height-1);

    int selectedLine(distribution(generator));
    int selectedY(distributionY(generator));

    std::pair<int, int> out;
    int count = 0;
    while (white != getColor(selectedLine, selectedY) && count < 50) //tries at most 50 times to pick a random pixel until that pixel is white.
    {
        selectedLine = (distribution(generator));
        selectedY = (distributionY(generator));
        count++;
    }
    out.second = selectedY;
    if (count < 50)
    {
        out.first = selectedLine;
        setColor(out.first, out.second, indexToCol(index));
    }
    else
        out.first = -1; //means that it failed to find a spot for that province.
    return out;
}


//changes the color of every pixel that is touching the current pixel to the new color,
//original color must be the one of the starting pixel.
void WhiteMapToProvinceMap::changeColorOfArea(int x, int y, QRgb originalColor, QRgb newColor)
{
    if (originalColor != getColor(x, y))
        return;

    std::stack<MarkingData> stack;
    setColor(x, y, newColor);
    stack.push(MarkingData(x, y)); //places the starting pixel in a stack

    while (!stack.empty()) //while the stack is not empty
    {
        MarkingData data(stack.top()); //set the current position to top of the stack
        setColor(data.x, data.y, newColor); //set the current pixel to the target color
        bool foundOne(false);
        for (int a = -1; a <= 1 && !foundOne; a++)
            if (isInBound(data.x + a, data.y) && originalColor == getColor(data.x+a, data.y)) //if it's near the current pixel
            { //and the color is the same
                stack.push(MarkingData(data.x + a, data.y)); //then we add that to the stack
                foundOne = true;
            }

        for (int b = -1; b <= 1 && !foundOne; b++)
            if (isInBound(data.x, data.y + b) && originalColor == getColor(data.x, data.y+b))
            {
                stack.push(MarkingData(data.x, data.y + b));
                foundOne = true;
            }

        if (!foundOne) //if there are no neighbours with the right color then we go back to a previous position to see
            stack.pop(); //if there are still neighbour of the right color
    }
}

void WhiteMapToProvinceMap::parseBlackAreaPixel(int x, int y, std::stack<MarkingData>& stack, InscribedRemovalData& data)
{
    if (!isInBound(x, y))
        return;

    if (data.originalAreaColor == getColor(x, y))
    {
        stack.push(MarkingData(x, y));
        data.foundAdjactedPixelWithSameColor = true;
        return;
    }
    if (getColor(x, y) == blue)
        return;
    if (data.oneNeighbourColor == data.originalAreaColor)
    {
        data.oneNeighbourColor = getColor(x, y);
        return;
    }

    if (data.oneNeighbourColor != getColor(x, y))
        data.areaIsTouchingMultipleColors = true;

}
void WhiteMapToProvinceMap::removeFullyInscribedBlackArea(int x, int y, QRgb originalColor)
{
    if (originalColor != getColor(x, y))
        return;
    InscribedRemovalData removalInformation;
    removalInformation.originalAreaColor = originalColor;
    removalInformation.oneNeighbourColor = originalColor;

    std::stack<MarkingData> stack;
    setColor(x, y, blue);
    stack.push(MarkingData(x, y));

    removalInformation.areaIsTouchingMultipleColors = false;

    while (!stack.empty())
    {
        MarkingData data(stack.top());
        setColor(data.x, data.y, blue);
        removalInformation.foundAdjactedPixelWithSameColor = false;
        for (int a = -1; a <= 1 && !removalInformation.foundAdjactedPixelWithSameColor; a++)
                parseBlackAreaPixel(data.x + a, data.y, stack, removalInformation);
        for (int b = -1; b <= 1 && !removalInformation.foundAdjactedPixelWithSameColor; b++)
            parseBlackAreaPixel(data.x, data.y +b, stack, removalInformation);

        if (!removalInformation.foundAdjactedPixelWithSameColor)
            stack.pop();
    }

    if (!removalInformation.areaIsTouchingMultipleColors)
        changeColorOfArea(x, y, blue, removalInformation.oneNeighbourColor);

}


void WhiteMapToProvinceMap::parsePixel(int x, int y, std::stack<MarkingData>& stack, QRgb black, InscribedRemovalData& data)
{

    if (!isInBound(x, y) || getColor(x, y) == black)
    {
        data.areaIsTouchingMultipleColors = true;
        return;
    }
    if (getColor(x, y) == data.originalAreaColor)
    {
        stack.push(MarkingData(x, y));
        data.foundAdjactedPixelWithSameColor = true;
        return;
    }
    if (getColor(x, y) == blue)
        return;

    if (data.fondZeroNeighbour)
    {
        data.fondZeroNeighbour = false;
        data.oneNeighbourColor = getColor(x, y);
        return;
    }
    if (data.oneNeighbourColor != getColor(x, y))
        data.areaIsTouchingMultipleColors = true;
}


void WhiteMapToProvinceMap::removeFullyIscribedArea(int x, int y, QRgb black, std::vector<QRgb*>& suppLines)
{
    InscribedRemovalData removalInformation;
    removalInformation.fondZeroNeighbour = true;
    removalInformation.originalAreaColor = getColor(x, y);

    std::stack<MarkingData> stack;
    setColor(x, y, blue);
    suppLines[y][x] = black;
    stack.push(MarkingData(x, y));

    removalInformation.areaIsTouchingMultipleColors = false;

    while (!stack.empty())
    {
        removalInformation.foundAdjactedPixelWithSameColor = false;
        MarkingData data(stack.top());
        setColor(data.x, data.y, blue);
        suppLines[data.y][data.x] = black;

        for (int a = -1; a <= 1 && !removalInformation.areaIsTouchingMultipleColors; a++)
            parsePixel(data.x+a, data.y, stack, black, removalInformation);
        for (int b = -1; b <= 1 && !removalInformation.areaIsTouchingMultipleColors; b++)
            parsePixel(data.x, data.y+b, stack, black, removalInformation);

        if (!removalInformation.foundAdjactedPixelWithSameColor)
            stack.pop();
    }

    if (!removalInformation.areaIsTouchingMultipleColors)
        changeColorOfArea(x, y, blue, removalInformation.oneNeighbourColor);
    else
        changeColorOfArea(x, y, blue, removalInformation.originalAreaColor);
}
