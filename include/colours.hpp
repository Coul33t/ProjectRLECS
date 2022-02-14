#pragma once
#include "libtcod_no_warnings.hpp"
#include "BearLibTerminal.h"

static const color_t default_bg = color_from_argb(255, 0, 0, 0); 

static const color_t black = color_from_argb(255, 0, 0, 0); 
static const color_t white = color_from_argb(255, 255, 255, 255); 

static const color_t light_wall = color_from_argb(255, 130, 110, 50);
static const color_t light_ground = color_from_argb(255, 200, 180, 50);

static const color_t dark_wall = color_from_argb(255, 32, 22, 12);
static const color_t dark_ground = color_from_argb(255, 50, 45, 12);

static const color_t green = color_from_argb(255, 0, 255, 0);
static const color_t dark_green = color_from_argb(255, 0, 100, 0);

static const color_t red = color_from_argb(255, 255, 0, 0);
static const color_t dark_red = color_from_argb(255, 100, 0, 0);

static const color_t blue = color_from_argb(255, 0, 0, 255);
static const color_t dark_blue = color_from_argb(255, 0, 0, 100);