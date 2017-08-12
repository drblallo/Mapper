#include "mapreader.h"
#include <QImage>
#define SPEAK
#include "mapreaderdefines.h"
#include <stack>
#include <vector>

using namespace mapreader;

short VectorNext::getX() 
{
	if (c == 0)
		return -1;
	
	if (c == 2)
		return 1;	
	
	return 0;
}

short VectorNext::getY()
{
	if (c == 0 || c == 2)
		return 0;
	
	if (c == 3)
		return 1;

	return -1;
}

void VectorNext::invert()
{
	c += 2;
	c = c % 4;
}

void VectorNext::rotate() 
{
	c++;
	if (c > 3)
	   c = 0;	
}

MapReader::MapReader(QImage& source)
{

	WRITE("Creating a map reader");

	QImage manipulatedImage(
			source.width(),
		   	source.height(),
		   	QImage::Format_RGB32
			);

	cleanSource(source, manipulatedImage);
	parseSource(manipulatedImage);

//	manipulatedImage.save("out.png");
}

void MapReader::cleanSource(const QImage& image, QImage& manipulated) const
{
	int imageWidth(image.width());
	int imageHeight(image.height());

	const QRgb* source[image.height()];
	QRgb* modified[image.height()];
	QColor black(0, 0, 0);

	for (int a = 0; a < imageHeight; a++)
	{
		source[a] = (const QRgb*)image.scanLine(a);
		modified[a] = (QRgb*)manipulated.scanLine(a);
	}

	WRITE("copying edges");

	for (int a = 0; a < imageWidth; a++)
	{
		modified[0][a] = source[0][a];	
		modified[imageHeight - 1][a] = source[imageHeight - 1][a];	
	}

	for (int a = 1; a < image.height() - 1; a++)
	{
		modified[a][0] = source[a][0];
		modified[a][imageWidth - 1] = source[a][imageWidth - 1];
	}

	WRITE("copying edges done");

	for (int y = 1; y < imageHeight - 1; y++)
	{
		for (int x = 1; x < imageWidth - 1; x++)
		{
			if (
					!(source[y][x] == source[y - 1][x] &&
					source[y][x] == source[y + 1][x] &&
					source[y][x] == source[y][x - 1] &&
					source[y][x] == source[y][x + 1] &&
					source[y][x] == source[y + 1][x + 1] &&
					source[y][x] == source[y + 1][x - 1] &&
					source[y][x] == source[y - 1][x + 1] &&
					source[y][x] == source[y - 1][x - 1])
				)	
			{
				modified[y][x] = source[y][x];	
			}
			else
				modified[y][x] = black.rgb();
				
		}
	}

	WRITE("copying content done");
}


bool MapReader::parseGetNext(QImage& image, QRgb** map, ParsingData& data, QRgb target) const
{
	data.next.invert();
	data.next.rotate();
	int t(0);

	while (
				data.x + data.next.getX() >= image.width() ||
				data.x + data.next.getX() < 0 ||
				data.y + data.next.getY() >= image.height() ||
				data.y + data.next.getY() < 0 ||
				map[data.y + data.next.getY()][data.x + data.next.getX()] != target
			)
	{
		t++;
		data.next.rotate();

		if (t == 4)
			return false;
	}

	return true;
}

void MapReader::parseZone(QImage& image, QRgb** map, int x, int y)
{
	WRITE("Parsing for a new zone");
	std::stack<ParsingData> s;
	QRgb col(map[y][x]);
	QColor black(0, 0, 0);

	ParsingData data;
	data.x = x;
	data.y = y;
	
	do
	{
		if (parseGetNext(image, map, data, col))
		{
			WRITE("Comming From");
			WRITE(data.x);
			WRITE(data.y);
			data.x += data.next.getX();
			data.y += data.next.getY();
			s.push(data);
			map[data.y][data.x] = black.rgb();		
			WRITE("Inserting Vertex");
			WRITE(data.x);
			WRITE(data.y);
		}
		else
		{
			if (s.empty())
				return;

			WRITE("Poping vertex");
			WRITE(data.x);
			WRITE(data.y);
			s.pop();
			data = s.top();
		}
	}
	while (data.x != x || data.y != y);


	if (s.size() < 3)
		return;
		
	allRegions.push_back(RegionPair(col, Region()));
	Region* ls(&allRegions.back().second);
	while (!s.empty())
	{
		data = s.top();
		s.pop();
		ls->push_back(std::pair<int, int>(data.x, data.y));	
		WRITE("Vertex At");
		WRITE(data.x);
		WRITE(data.y);
	}

	WRITE("Created a new area");
	WRITE(ls->size());
}

void MapReader::parseSource(QImage& image)
{
	QRgb* modified[image.height()];

	int imageWidth(image.width());
	int imageHeight(image.height());

	for (int a = 0; a < image.height(); a++)
		modified[a] = (QRgb*)image.scanLine(a);

	QColor black(0, 0, 0);
	for (int y = 0; y < imageHeight - 1; y++)
		for (int x = 0; x < imageWidth - 1; x++)
		{
			int count(0);
			
                        if (y >= imageHeight - 1 || modified[y][x] != modified[y+1][x])
				count++;
                        if (y < 1 || modified[y][x] != modified[y-1][x])
				count++;
                        if (x >= imageWidth - 1 || modified[y][x] != modified[y][x+1])
				count++;
                        if (x < 1 || modified[y][x] != modified[y][x-1])
				count++;

			if (modified[y][x] != black.rgb() && count > 1) 		
				parseZone(image, modified, x, y);
		}
}

