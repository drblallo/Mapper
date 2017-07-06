#pragma once
#define QUIET_ALL

#ifdef SPEAK
#include <iostream>
#endif

#ifndef QUIET_ALL
#ifdef SPEAK 
#define WRITE(text) std::cout << "MAPREADER> " << text << std::endl; 
#else
#define WRITE(text) do {} while(0)
#endif
#else
#define WRITE(text) do {} while(0)
#endif


