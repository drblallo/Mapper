#pragma once
#include "region.h"
#include <vector>
#include <mutex>
#include <algorithm>
#include <QImage>

using namespace std;

namespace renderer
{
    class Map
    {
        public:
            Map(QImage *image);
            unsigned int createRegion(QColor color);
            inline QColor getFirstColor(unsigned int a)
            {
                lock_guard<mutex> l(m);
                if (regions.size() <= a)
                    return QColor::fromRgba64(1, 1, 1, 1);
                return regions[a].getFirstColor();
            }

            inline QColor getSecondColor(unsigned int a)
            {
                lock_guard<mutex> l(m);
                if (regions.size() <= a)
                    return QColor::fromRgba64(1, 1, 1, 1);
                return regions[a].getSecondColor();
            }

            inline QColor getThirdColor(unsigned int a)
            {
                lock_guard<mutex> l(m);
                if (regions.size() <= a)
                    return QColor::fromRgba64(1, 1, 1, 1);
                return regions[a].getThirdColor();
            }

            inline void setFirstColor(unsigned int a, QColor col)
            {
                lock_guard<mutex> l(m);
                if (regions.size() <= a)
                    return;
                regions[a].setFirstColor(col);
            }

            inline void setSecondColor(unsigned int a, QColor col)
            {
                lock_guard<mutex> l(m);
                if (regions.size() <= a)
                    return;
                regions[a].setSecondColor(col);
            }

            inline void setThirdColor(unsigned int a, QColor col)
            {
                lock_guard<mutex> l(m);
                if (regions.size() <= a)
                    return;
                regions[a].setThirdColor(col);
            }

            inline unsigned int getRegionCount()
            {
                lock_guard<mutex> l(m);
                return regions.size();
            }

            inline bool areConnected(unsigned int p, unsigned int r)
            {
                return (find(connections[p].begin(), connections[p].end(), r) != connections[p].end());
            }

            inline void setConnected(unsigned int p, unsigned int r)
            {
                if (areConnected(p, r))
                    return;
                connections[p].push_back(r);
                connections[r].push_back(p);
            }

        private:
            void loadFromImage(QImage* image);

            QImage generatedImage;
            mutex m;
            vector<Region> regions;
            vector<vector<unsigned int> > connections;


    };
}
